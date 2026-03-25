#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <array>
#include <cstdint>
#include <utility>

using namespace std;

// =============================================================
// 方針（全体）
// =============================================================
// - 盤面が小さい（N<=10）ので「候補手を絞る + モンテカルロ（ロールアウト）で評価」する。
// - 相手の行動は「重み付き評価（セル価値×係数）+ εでランダム」という仮定で近似し、
//   粒子フィルタ（パーティクルフィルタ）で係数分布を更新する。
// - 1手目は候補集合から選び、UCB（Upper Confidence Bound）でシミュレーション回数を配分する。
// - 乱数の比較を安定させるため、候補間で「同じ乱数系列」を使う（Common Random Numbers; CRN）。

// -----------------------------
// RNG (Xorshift64)
// -----------------------------
// 目的：高速な疑似乱数生成（シミュレーション用）。
struct Xorshift {
    uint64_t x;
    Xorshift(uint64_t seed=123456789ULL) : x(seed ? seed : 123456789ULL) {}

    // 目的：64bit乱数を1つ生成する。
    inline uint64_t next_u64() {
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        return x;
    }

    // 目的：[0,1) の一様乱数を生成する（double）。
    inline double next_double() {
        return (next_u64() >> 11) * (1.0 / 9007199254740992.0);
    }
};

// 目的：seedを拡散して別のseedを得る（CRN用の種生成などに使う）。
static inline uint64_t splitmix64(uint64_t &x) {
    uint64_t z = (x += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

constexpr int MAX_N = 10;
constexpr int MAX_M = 10;

// 候補手の上限（このあと時間に応じてさらに減らす）。
constexpr int MAX_CAND = 20;

// 1プレイヤーの合法手が最大100程度という前提の固定配列用。
constexpr int MAX_MOVES = 100;

int N, M, T, U;
int V[MAX_N][MAX_N];
int VMAX = 1;

// -----------------------------
// Timer
// -----------------------------
// 目的：全体時間（1.94秒）内で、各ターンの探索時間を配分するための計測。
struct Timer {
    chrono::high_resolution_clock::time_point start;
    Timer() { reset(); }
    void reset() { start = chrono::high_resolution_clock::now(); }

    // 目的：開始からの経過秒数（double）を返す。
    double get_sec() const {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now - start).count() / 1000.0;
    }
} global_timer;

// -----------------------------
// FastQueue (fixed capacity)
// -----------------------------
// 目的：小さい盤面上のBFSで、動的確保を避けて高速化する。
template<typename Tt, int CAP>
struct FastQueue {
    Tt data[CAP];
    int head = 0, tail = 0;
    inline void clear() { head = 0; tail = 0; }
    inline void push(const Tt &v) { data[tail++] = v; }
    inline Tt pop() { return data[head++]; }
    inline bool empty() const { return head == tail; }
};

// -----------------------------
// State
// -----------------------------
// 目的：ゲーム状態（各プレイヤー位置、セル所有者、セルレベル）を保持する。
struct State {
    int pos_r[MAX_M];
    int pos_c[MAX_M];
    int owner[MAX_N][MAX_N];
    int level[MAX_N][MAX_N];

    // 目的：盤面を空に初期化（owner=-1, level=0）。
    void init_empty() {
        for (int r=0; r<N; ++r) for (int c=0; c<N; ++c) {
            owner[r][c] = -1;
            level[r][c] = 0;
        }
    }

    // 目的：各プレイヤーのスコア（Σ V[r][c]*level）を計算して返す。
    inline void compute_scores(long long scores_out[MAX_M]) const {
        for (int i=0; i<MAX_M; ++i) scores_out[i] = 0;
        for (int r=0; r<N; ++r) for (int c=0; c<N; ++c) {
            int o = owner[r][c];
            if (o != -1) scores_out[o] += 1LL * V[r][c] * level[r][c];
        }
    }

    // 目的：探索の評価関数（最終スコアに寄せた近似）を返す。
    // 方針：
    // - 自分のスコアを上げる（log(s0+1)）
    // - 最強相手を下げる（log(max_other+1)を引く）
    // - Mが小さいほど「相手抑制」を少し強くする（gamma）。
    // - 微小な補助項として、領域数とレベル合計を足す（同点の微差に使う）。
    double get_score_objective() const {
        long long scores[MAX_M];
        compute_scores(scores);

        long long s0 = scores[0];
        long long max_other = 1;
        for (int i=1; i<M; ++i) max_other = max(max_other, scores[i]);

        double gamma = 1.0 + 0.3 / max(1, (M - 1));

        int territory_count0 = 0;
        int level_sum0 = 0;
        for (int r=0; r<N; ++r) for (int c=0; c<N; ++c) {
            if (owner[r][c] == 0) { territory_count0++; level_sum0 += level[r][c]; }
        }

        double score = log((double)s0 + 1.0) - gamma * log((double)max_other + 1.0);
        score += territory_count0 * 0.0001;
        score += level_sum0 * 0.00001;
        return score;
    }
};

// 目的：出力する行動（移動先セル座標）。
struct Move { int r, c; };

// -----------------------------
// Safe input
// -----------------------------
// 目的：標準入力からintを読む（失敗ならfalse）。
static inline bool read_int(int &x) {
    return (cin >> x) ? true : false;
}

// -----------------------------
// Valid moves
// -----------------------------
// 目的：合法手を列挙する。
// 方針（この実装が前提としている合法手）
// - 自分が所有する連結成分（現在位置から4近傍でowner==自分）上のセル
// - その連結成分に隣接する4近傍セル
// - ただし他プレイヤーの現在位置は除外（is_occupied）。
static uint32_t visited_token[MAX_N][MAX_N];
static uint32_t current_token = 1;
static uint32_t added_token[MAX_N][MAX_N];
static uint32_t current_added_token = 1;

static FastQueue<pair<int,int>, MAX_N*MAX_N> bfs_q;
static vector<pair<int,int>> reachable_territory;

// 目的：visited配列等を毎回memsetしないための「世代管理」。
// tokが0に回ったら全消去して安全に戻す。
static inline void token_bump(uint32_t &tok, uint32_t grid[MAX_N][MAX_N]) {
    tok++;
    if (tok == 0) {
        memset(grid, 0, sizeof(uint32_t) * MAX_N * MAX_N);
        tok = 1;
    }
}

// 目的：プレイヤーp_idの合法手一覧を out_moves に詰める。
static inline void get_valid_moves(const State& s, int p_id, vector<Move>& out_moves) {
    out_moves.clear();

    token_bump(current_token, visited_token);
    const uint32_t tok = current_token;

    int start_r = s.pos_r[p_id];
    int start_c = s.pos_c[p_id];
    if (start_r < 0 || start_r >= N || start_c < 0 || start_c >= N) {
        out_moves.push_back({0,0});
        return;
    }

    // 目的：他プレイヤーが居るセルを避けるための占有表。
    bool is_occupied[MAX_N][MAX_N];
    memset(is_occupied, 0, sizeof(is_occupied));
    for (int i=0; i<M; ++i) {
        if (i == p_id) continue;
        int r = s.pos_r[i], c = s.pos_c[i];
        if (0 <= r && r < N && 0 <= c && c < N) is_occupied[r][c] = true;
    }

    static const int dr[4] = {0, 0, 1, -1};
    static const int dc[4] = {1, -1, 0, 0};

    // 目的：自分の連結成分（owner==p_id）をBFSで列挙する。
    bfs_q.clear();
    bfs_q.push({start_r, start_c});
    visited_token[start_r][start_c] = tok;

    reachable_territory.clear();
    reachable_territory.push_back({start_r, start_c});

    while (!bfs_q.empty()) {
        auto cur = bfs_q.pop();
        for (int k=0; k<4; ++k) {
            int nr = cur.first + dr[k];
            int nc = cur.second + dc[k];
            if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                if (visited_token[nr][nc] != tok && s.owner[nr][nc] == p_id) {
                    visited_token[nr][nc] = tok;
                    reachable_territory.push_back({nr, nc});
                    bfs_q.push({nr, nc});
                }
            }
        }
    }

    token_bump(current_added_token, added_token);
    const uint32_t atok = current_added_token;

    // 目的：連結成分内部 + その隣接セルを候補として追加（重複はadded_tokenで除去）。
    for (auto &t : reachable_territory) {
        int r = t.first, c = t.second;
        if (added_token[r][c] != atok && !is_occupied[r][c]) {
            added_token[r][c] = atok;
            out_moves.push_back({r,c});
        }
        for (int k=0; k<4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                if (added_token[nr][nc] != atok && !is_occupied[nr][nc]) {
                    added_token[nr][nc] = atok;
                    out_moves.push_back({nr,nc});
                }
            }
        }
    }

    if (out_moves.empty()) out_moves.push_back({start_r, start_c});
}

// -----------------------------
// Exact advance (rules-correct)
// -----------------------------
// 目的：ルール通りに1ターン進める（衝突処理→所有/レベル更新→位置確定）。
// 方針：
// - 同一セルに複数が移動：そのセルのownerが含まれていればownerが勝者、それ以外は全員失敗。
// - 失敗（collected=true）したプレイヤーは位置が戻る。
// - 成功者は移動先で占領/強化/攻撃（レベル減少、0なら奪取）を行う。
// 注意：movesの座標が盤外の場合の防御は、この版では行っていない（入力が合法前提）。
static inline void advance_exact(State& s, const array<Move, MAX_M>& moves) {
    int start_r[MAX_M], start_c[MAX_M];
    for (int p=0; p<M; ++p) { start_r[p] = s.pos_r[p]; start_c[p] = s.pos_c[p]; }

    static int target_count[MAX_N][MAX_N];
    static int target_list[MAX_N][MAX_N][MAX_M];
    for (int r=0; r<N; ++r) memset(target_count[r], 0, sizeof(int)*N);

    // 目的：各セルに「誰が来ようとしているか」を集計。
    for (int p=0; p<M; ++p) {
        int r = moves[p].r, c = moves[p].c;
        target_list[r][c][target_count[r][c]++] = p;
    }

    bool collected[MAX_M];
    memset(collected, 0, sizeof(collected));

    int new_r[MAX_M], new_c[MAX_M];
    for (int p=0; p<M; ++p) { new_r[p] = start_r[p]; new_c[p] = start_c[p]; }

    // 目的：衝突の勝者判定と、失敗者のマーキング。
    for (int r=0; r<N; ++r) for (int c=0; c<N; ++c) {
        int cnt = target_count[r][c];
        if (cnt == 0) continue;

        int cell_owner = s.owner[r][c];
        int winner = -1;

        if (cnt == 1) {
            winner = target_list[r][c][0];
        } else {
            if (cell_owner != -1) {
                for (int k=0; k<cnt; ++k) {
                    int p = target_list[r][c][k];
                    if (p == cell_owner) { winner = p; break; }
                }
            }
        }

        if (winner == -1) {
            for (int k=0; k<cnt; ++k) collected[target_list[r][c][k]] = true;
        } else {
            for (int k=0; k<cnt; ++k) {
                int p = target_list[r][c][k];
                if (p != winner) collected[p] = true;
            }
            new_r[winner] = r;
            new_c[winner] = c;
        }
    }

    // 目的：成功者のみ、移動先セルで占領/強化/攻撃を適用。
    for (int p=0; p<M; ++p) {
        if (collected[p]) continue;

        int r = new_r[p], c = new_c[p];
        int o = s.owner[r][c];

        if (o == -1) {
            s.owner[r][c] = p;
            s.level[r][c] = 1;
        } else if (o == p) {
            if (s.level[r][c] < U) s.level[r][c]++;
        } else {
            s.level[r][c]--;
            if (s.level[r][c] == 0) {
                s.owner[r][c] = p;
                s.level[r][c] = 1;
            } else {
                collected[p] = true;
            }
        }
    }

    // 目的：最終的な位置確定（失敗者は元位置に戻す）。
    for (int p=0; p<M; ++p) {
        if (collected[p]) { s.pos_r[p] = start_r[p]; s.pos_c[p] = start_c[p]; }
        else { s.pos_r[p] = new_r[p]; s.pos_c[p] = new_c[p]; }
    }
}

// -----------------------------
// AI model (particle filter)
// -----------------------------
// 目的：相手が「どのセルを好むか」を係数パラメータで表し、観測から更新する。
// - wa: 空きセルの魅力度
// - wb: 自セルの強化（レベル<U）の魅力度
// - wc: 敵セル（レベル==1）の魅力度（奪いやすい）
// - wd: 敵セル（レベル>=2）の魅力度（削り）
// - eps: ランダム行動率（探索やぶれの再現）
struct AIParam { double wa, wb, wc, wd, eps; };
struct AIParticle { AIParam par; double w; };

struct AIModel {
    static constexpr int P = 24;
    array<AIParticle, P> ps;

    // 目的：粒子（パラメータ）を一様重みでランダム初期化。
    void init(Xorshift &rng) {
        double invP = 1.0 / P;
        for (int i=0; i<P; ++i) {
            ps[i].par.wa  = 0.3 + rng.next_double() * 0.7;
            ps[i].par.wb  = 0.3 + rng.next_double() * 0.7;
            ps[i].par.wc  = 0.3 + rng.next_double() * 0.7;
            ps[i].par.wd  = 0.3 + rng.next_double() * 0.7;
            ps[i].par.eps = 0.1 + rng.next_double() * 0.4;
            ps[i].w = invP;
        }
    }

    // 目的：セル(r,c)を、プレイヤーp視点で分類する（価値計算の分岐用）。
    static inline int cell_type(const State &s, int p, int r, int c) {
        int o = s.owner[r][c];
        if (o == -1) return 0;                 // 空き
        if (o == p) {
            if (s.level[r][c] < U) return 1;   // 自分（強化可能）
            return 2;                          // 自分（満タン、価値0扱い）
        }
        if (s.level[r][c] == 1) return 3;      // 敵（奪いやすい）
        return 4;                              // 敵（削り）
    }

    // 目的：セルの評価値 = V * 係数（分類ごと）を返す。
    static inline double eval_cell(const AIParam &par, const State &s, int p, int r, int c) {
        int t = cell_type(s, p, r, c);
        if (t == 2) return 0.0;
        double mult = 0.0;
        if (t == 0) mult = par.wa;
        else if (t == 1) mult = par.wb;
        else if (t == 3) mult = par.wc;
        else mult = par.wd;
        return (double)V[r][c] * mult;
    }

    // 目的：相手の実際の移動先(obs_r,obs_c)を観測し、粒子重みを更新する。
    // 方針：
    // - 相手は「εで一様ランダム、(1-ε)で最大評価のセルから等確率」というモデル。
    // - 観測がその最大集合に含まれるかで尤度を計算し、重みを乗算。
    // - ESS（有効サンプルサイズ）が小さくなったら系統リサンプリング。
    void observe(const State &turn_start, int p, int obs_r, int obs_c, const vector<Move> &B) {
        if (B.empty()) return;

        bool inB = false;
        for (auto &m : B) if (m.r == obs_r && m.c == obs_c) { inB = true; break; }
        if (!inB) return;

        double wsum = 0.0;
        const double invB = 1.0 / (double)B.size();

        for (int i=0; i<P; ++i) {
            const AIParam &par = ps[i].par;

            double best = -1e100;
            int argcnt = 0;
            bool obs_is_arg = false;

            for (auto &m : B) {
                double e = eval_cell(par, turn_start, p, m.r, m.c);
                if (e > best + 1e-12) {
                    best = e;
                    argcnt = 1;
                    obs_is_arg = (m.r == obs_r && m.c == obs_c);
                } else if (fabs(e - best) <= 1e-12) {
                    argcnt++;
                    if (m.r == obs_r && m.c == obs_c) obs_is_arg = true;
                }
            }

            double p_greedy = 0.0;
            if (obs_is_arg && argcnt > 0) p_greedy = 1.0 / (double)argcnt;

            double prob = par.eps * invB + (1.0 - par.eps) * p_greedy;
            if (prob < 1e-15) prob = 1e-15;
            ps[i].w *= prob;
            wsum += ps[i].w;
        }

        if (wsum <= 0.0) {
            double invP = 1.0 / P;
            for (int i=0; i<P; ++i) ps[i].w = invP;
            return;
        }

        for (int i=0; i<P; ++i) ps[i].w /= wsum;

        double ess_inv = 0.0;
        for (int i=0; i<P; ++i) ess_inv += ps[i].w * ps[i].w;
        double ess = 1.0 / max(1e-18, ess_inv);
        if (ess >= P * 0.5) return;

        // 系統リサンプリング
        array<AIParticle, P> newps;
        double step = 1.0 / P;

        uint64_t seed = 1469598103934665603ULL;
        seed ^= (uint64_t)(obs_r * 1315423911u + obs_c * 2654435761u + p * 97531u);
        seed *= 1099511628211ULL;
        uint64_t sm = seed;
        double u0 = (splitmix64(sm) >> 11) * (1.0 / 9007199254740992.0) * step;

        double cdf = ps[0].w;
        int idx = 0;
        for (int i=0; i<P; ++i) {
            double u = u0 + i * step;
            while (u > cdf && idx + 1 < P) { idx++; cdf += ps[idx].w; }
            newps[i].par = ps[idx].par;
            newps[i].w = step;
        }
        ps = newps;
    }

    // 目的：最も重い粒子（MAP推定に近い）を返す。
    AIParam best_param() const {
        int best = 0;
        for (int i=1; i<P; ++i) if (ps[i].w > ps[best].w) best = i;
        return ps[best].par;
    }
};

// -----------------------------
// Target selection + dynamic balance
// -----------------------------
// 目的：攻撃対象（主にトップの相手、必要なら2位も）を決め、攻撃強度attack_wを決める。
struct TargetInfo {
    bool is_target[MAX_M];
    int leader = 0;
    int second = 0;
    bool include_second = false;
    int leader_cells = 0;
    double myS = 0.0;
    double leadS = 0.0;
    double attack_w = 0.6; // 0..1
};

// 目的：現時点の「自分以外の最高スコアのプレイヤー」を返す。
static inline int compute_leader_ai(const State &s) {
    if (M <= 1) return 0;
    long long scores[MAX_M];
    s.compute_scores(scores);
    int leader = 1;
    for (int p=2; p<M; ++p) if (scores[p] > scores[leader]) leader = p;
    return leader;
}

// 目的：攻撃対象と攻撃強度を決める。
// 方針：
// - leadをスコア最大の相手とする、secは2番手。
// - leadが自分より十分強いほど攻撃重視（attack_w高）。
// - leadのセルがほぼ無い/自分が勝ち寄りなら攻撃を弱める。
// - Mが小さい場合や2位が強い場合は2位も対象に含める。
static inline TargetInfo compute_target_info(const State &s) {
    TargetInfo ti;
    for (int i=0; i<MAX_M; ++i) ti.is_target[i] = false;

    if (M <= 1) {
        ti.leader = 0;
        ti.second = 0;
        ti.attack_w = 0.0;
        return ti;
    }

    long long scores[MAX_M];
    s.compute_scores(scores);

    int lead = 1;
    for (int p=2; p<M; ++p) if (scores[p] > scores[lead]) lead = p;

    int sec = (lead == 1 ? 2 : 1);
    if (sec >= M) sec = lead;
    for (int p=1; p<M; ++p) {
        if (p == lead) continue;
        if (scores[p] > scores[sec]) sec = p;
    }

    ti.leader = lead;
    ti.second = sec;
    ti.myS = (double)scores[0];
    ti.leadS = (double)scores[lead];

    int leader_cells = 0;
    for (int r=0; r<N; ++r) for (int c=0; c<N; ++c) if (s.owner[r][c] == lead) leader_cells++;
    ti.leader_cells = leader_cells;

    double ratio = (ti.leadS + 1.0) / (ti.myS + 1.0);

    double w;
    if (ratio >= 1.15) w = 1.0;
    else if (ratio <= 0.90) w = 0.25;
    else {
        double t = (ratio - 0.90) / (1.15 - 0.90);
        w = 0.25 + t * (1.0 - 0.25);
    }

    if (leader_cells <= 2 && ratio <= 1.0) w *= 0.6;
    if (w < 0.15) w = 0.15;
    if (w > 1.0) w = 1.0;
    ti.attack_w = w;

    bool include_second;
    if (M <= 3) include_second = true;
    else {
        include_second = (M >= 6);
        if (!include_second) {
            double sL = (double)scores[lead] + 1.0;
            double s2 = (double)scores[sec] + 1.0;
            if (s2 >= 0.85 * sL) include_second = true;
            if ((double)scores[sec] >= 0.95 * (double)scores[0]) include_second = true;
        }
    }
    ti.include_second = include_second;

    ti.is_target[lead] = true;
    if (include_second && sec != lead) ti.is_target[sec] = true;

    return ti;
}

// -----------------------------
// High-V focused opponent risk map (targets only)
// -----------------------------
// 目的：次ターンに「ターゲット（強敵）が来そうなセル」を確率で見積もり、riskWに入れる。
// 方針：
// - 相手のモデル（AIParam）に基づき、合法手集合B上での行動確率を計算。
// - さらに「高いVに寄る」よう (V/VMAX)^2 を掛けて、重要セルの衝突リスクを強調する。
static inline void compute_target_risk_map(const State &s,
                                           const array<AIParam, MAX_M> &ai_params,
                                           const bool is_target[MAX_M],
                                           double riskW[MAX_N][MAX_N]) {
    for (int r=0; r<N; ++r) for (int c=0; c<N; ++c) riskW[r][c] = 0.0;

    const double invVMAX = 1.0 / (double)max(1, VMAX);
    vector<Move> B;
    B.reserve(128);

    for (int p=1; p<M; ++p) {
        if (!is_target[p]) continue;

        get_valid_moves(s, p, B);
        if (B.empty()) continue;

        const AIParam &par = ai_params[p];
        const int sz = (int)B.size();

        double best = -1e100;
        int argcnt = 0;
        for (int i=0; i<sz; ++i) {
            const Move &m = B[i];
            double e = AIModel::eval_cell(par, s, p, m.r, m.c);
            if (e > best + 1e-12) { best = e; argcnt = 1; }
            else if (fabs(e - best) <= 1e-12) { argcnt++; }
        }
        if (argcnt <= 0) argcnt = 1;

        const double uniform_prob = par.eps / (double)sz;
        const double greedy_each  = (1.0 - par.eps) / (double)argcnt;

        for (int i=0; i<sz; ++i) {
            const Move &m = B[i];
            double prob = uniform_prob;
            double e = AIModel::eval_cell(par, s, p, m.r, m.c);
            if (fabs(e - best) <= 1e-12) prob += greedy_each;

            double x = (double)V[m.r][m.c] * invVMAX;
            double imp = x * x;

            riskW[m.r][m.c] += prob * imp;
        }
    }
}

// -----------------------------
// Opponent move selection
// -----------------------------
// 目的：相手の行動をモデル通りに1手サンプルする（シミュレーション用）。
// 方針：
// - CRNのため、分岐に関係なく next_double と next_u64 を最初に消費する。
// - εで一様、(1-ε)で最大評価の手から等確率。
static inline Move choose_ai_move(const State &s, int p_id, const AIParam &par, Xorshift &rng, vector<Move> &tmp) {
    get_valid_moves(s, p_id, tmp);

    double r = rng.next_double();
    uint64_t rnd = rng.next_u64();

    if (tmp.empty()) return {s.pos_r[p_id], s.pos_c[p_id]};
    if ((int)tmp.size() == 1) return tmp[0];

    if (r < par.eps) {
        int idx = (int)(rnd % (uint64_t)tmp.size());
        return tmp[idx];
    }

    double best = -1e100;
    int best_cnt = 0;
    Move best_moves[MAX_MOVES];

    for (auto &m : tmp) {
        double e = AIModel::eval_cell(par, s, p_id, m.r, m.c);
        if (e > best + 1e-12) {
            best = e;
            best_cnt = 1;
            best_moves[0] = m;
        } else if (fabs(e - best) <= 1e-12) {
            if (best_cnt < MAX_MOVES) best_moves[best_cnt++] = m;
        }
    }
    if (best_cnt <= 0) return tmp[0];
    int idx = (int)(rnd % (uint64_t)best_cnt);
    return best_moves[idx];
}

// -----------------------------
// Base move heuristics (growth)
// -----------------------------
// 目的：自セルの防衛（隣接に敵がいるか）判定。
static inline bool has_adjacent_enemy(const State &s, int r, int c) {
    static const int dr[4] = {0, 0, 1, -1};
    static const int dc[4] = {1, -1, 0, 0};
    for (int k=0; k<4; ++k) {
        int nr = r + dr[k], nc = c + dc[k];
        if (0 <= nr && nr < N && 0 <= nc && nc < N) {
            int no = s.owner[nr][nc];
            if (no != -1 && no != 0) return true;
        }
    }
    return false;
}

// 目的：セル(r,c)に移動する「基本価値」を返す（候補選別やロールアウト重み）。
// 方針：
// - 空き：取りやすいので倍率大
// - 自分：脅威（隣接敵）なら強化優先、満タンならほぼ無価値
// - 敵：レベル1は奪いやすいので高い、レベル>=2は控えめ
static inline double base_move_score(const State &s, int leader, int r, int c) {
    double w = (double)V[r][c];

    int o = s.owner[r][c];
    if (o == -1) {
        w *= 4.158041517441582;
    } else if (o == 0) {
        bool threatened = has_adjacent_enemy(s, r, c);
        if (s.level[r][c] < U) w *= threatened ? 6.520436877618656 : 0.6;
        else w *= threatened ? 0.8 : 0.0450417279607806;
    } else {
        if (s.level[r][c] == 1) w *= (o == leader ? 6.703571361159733 : 6.0);
        else w *= (o == leader ? 1.2 : 0.8);
    }

    if (w < 0.01) w = 0.01;
    return w;
}

// -----------------------------
// Root candidate evaluation
// -----------------------------
// 目的：候補手mを「探索の根」で評価し、候補選別に使う。
// 方針：
// - base_move_score を土台にする
// - riskW（ターゲットが来そう）を使い、衝突しやすい手を調整
//   * 自分セル強化なら、衝突しやすいほど有利になるので少し加点（BETA）
//   * 空き/敵セルなら、衝突で無駄になりやすいので減点（ALPHA）
// - ターゲットのセルを攻撃する価値を attack_w で調整して加点
// - さらに「相手が来そうな重要セルを先に取る（ブロック）」を加点
static constexpr double ROOT_RISK_ALPHA = 0.08;
static constexpr double ROOT_RISK_BETA  = 0.05;
static constexpr double ROOT_RISK_MIN_MULT = 0.10;

static inline double root_eval_candidate(const State &s, int leader,
                                        const TargetInfo &ti,
                                        const double riskW[MAX_N][MAX_N],
                                        const Move &m) {
    int r = m.r, c = m.c;

    double sc = base_move_score(s, leader, r, c);

    double rr = riskW[r][c];
    if (rr < 0.0) rr = 0.0;
    if (rr > 2.0) rr = 2.0;

    int o = s.owner[r][c];

    if (o == 0) {
        sc *= (1.0 + ROOT_RISK_BETA * rr);
    } else {
        double alpha = ROOT_RISK_ALPHA;
        if (o == -1) alpha *= 1.4;
        double mult = 1.0 - alpha * rr;
        if (mult < ROOT_RISK_MIN_MULT) mult = ROOT_RISK_MIN_MULT;
        sc *= mult;
    }

    if (o != -1 && o != 0 && ti.is_target[o]) {
        double v = (double)V[r][c];
        if (s.level[r][c] == 1) sc += ti.attack_w * (2.2 * v);
        else {
            sc += ti.attack_w * (0.8 * v);
            sc -= ti.attack_w * (0.2 * v);
        }
    }

    if (o != 0 && rr > 0.0) {
        sc += ti.attack_w * rr * (0.15 * (double)VMAX);
    }

    return sc;
}

// -----------------------------
// Rollout weighting (cheap)
// -----------------------------
// 目的：ロールアウト中に「それっぽい手」を確率的に選ぶための重み。
static inline double rollout_move_weight(const State &s, int leader,
                                        const TargetInfo &ti,
                                        int r, int c) {
    double w = base_move_score(s, leader, r, c);

    int o = s.owner[r][c];
    if (o != -1 && o != 0 && ti.is_target[o]) {
        double v = (double)V[r][c];
        if (s.level[r][c] == 1) w += ti.attack_w * (1.8 * v);
        else {
            w += ti.attack_w * (0.5 * v);
            w -= ti.attack_w * (0.25 * v);
        }
    }

    if (w < 0.01) w = 0.01;
    return w;
}

// 目的：ロールアウト中の自分手を「重み付きランダム」で選ぶ。
// 方針：
// - CRNのために先に乱数を消費（tmpサイズが1でも消費）
// - rollout_move_weight に比例して選ぶ（ルーレット選択）。
static inline Move choose_my_rollout_move(const State &s, Xorshift &rng, vector<Move> &tmp) {
    get_valid_moves(s, 0, tmp);

    double r01 = rng.next_double();

    if (tmp.empty()) return {s.pos_r[0], s.pos_c[0]};
    if ((int)tmp.size() == 1) return tmp[0];

    int leader = compute_leader_ai(s);
    TargetInfo ti = compute_target_info(s);

    double weights[MAX_MOVES];
    double sum = 0.0;

    const int sz = (int)tmp.size();
    for (int i=0; i<sz; ++i) {
        double w = rollout_move_weight(s, leader, ti, tmp[i].r, tmp[i].c);
        weights[i] = w;
        sum += w;
    }

    double rv = r01 * sum;
    double cur = 0.0;
    for (int i=0; i<sz; ++i) {
        cur += weights[i];
        if (rv <= cur) return tmp[i];
    }
    return tmp.back();
}

// 目的：2手目（t+1）を貪欲に決めるための手選択。
// 方針：
// - 現在状態でリスクマップを作り、root_eval_candidate が最大の手を返す。
// - 2手目を「軽い木探索」的に見たいが、この版は貪欲に簡略化している。
static inline Move choose_my_greedy_move(const State &s,
                                        const array<AIParam, MAX_M> &ai_params,
                                        vector<Move> &tmp) {
    get_valid_moves(s, 0, tmp);
    if (tmp.empty()) return {s.pos_r[0], s.pos_c[0]};

    int leader = compute_leader_ai(s);
    TargetInfo ti = compute_target_info(s);

    double riskW[MAX_N][MAX_N];
    compute_target_risk_map(s, ai_params, ti.is_target, riskW);

    Move best_m = tmp[0];
    double best_sc = root_eval_candidate(s, leader, ti, riskW, best_m);

    for (int i=1; i<(int)tmp.size(); ++i) {
        const Move &m = tmp[i];
        double sc = root_eval_candidate(s, leader, ti, riskW, m);
        if (sc > best_sc) { best_sc = sc; best_m = m; }
    }
    return best_m;
}

// -----------------------------
// Simulation
// -----------------------------
// 目的：候補手 my_first の価値を、depthターン先までシミュレーションして評価する。
// 方針：
// - 1手目：my_first固定
// - 2手目：choose_my_greedy_move（簡易2手読み）
// - 以降：choose_my_rollout_move（確率ロールアウト）
// - 相手：choose_ai_move（モデルに従う）
// - 最後に get_score_objective を返す。
static inline double run_simulation_2ply(State s, const Move my_first, int depth,
                                        const array<AIParam, MAX_M> &ai_params,
                                        uint64_t seed) {
    Xorshift rng(seed);

    array<Move, MAX_M> moves;
    vector<Move> tmp; tmp.reserve(128);

    moves[0] = my_first;
    for (int p=1; p<M; ++p) moves[p] = choose_ai_move(s, p, ai_params[p], rng, tmp);
    advance_exact(s, moves);

    if (depth == 0) return s.get_score_objective();

    moves[0] = choose_my_greedy_move(s, ai_params, tmp);
    for (int p=1; p<M; ++p) moves[p] = choose_ai_move(s, p, ai_params[p], rng, tmp);
    advance_exact(s, moves);

    int rem = depth - 1;
    for (int d=0; d<rem; ++d) {
        moves[0] = choose_my_rollout_move(s, rng, tmp);
        for (int p=1; p<M; ++p) moves[p] = choose_ai_move(s, p, ai_params[p], rng, tmp);
        advance_exact(s, moves);
    }

    return s.get_score_objective();
}

// -----------------------------
// CRN seed (candidate-independent)
// -----------------------------
// 目的：候補手に依存しない形で、サンプル番号sample_idからseedを生成する。
// 方針：UCBで同じ候補に追加試行するときでも、seedが「試行回数」だけで決まるようにする。
static inline uint64_t make_rollout_seed(uint64_t turn_base, uint64_t sample_id) {
    uint64_t x = turn_base ^ (sample_id * 0x9e3779b97f4a7c15ULL);
    return splitmix64(x);
}

// -----------------------------
// Depth schedule (Proposal B fixed + safety)
// -----------------------------
// 目的：ターン番号で基本探索深さを決める（簡易スケジュール）。
static inline int base_depth_by_turn(int t) {
    if (t < 30) return 7;
    if (t < 70) return 10;
    return 11;
}

// 目的：ターン当たり時間 turn_time が短いときに深さを下げて時間切れを避ける。
static inline int apply_depth_safety(int depth, double turn_time, int &dec_out) {
    int dec = 0;
    if (turn_time < 0.006) dec = 6;
    else if (turn_time < 0.008) dec = 5;
    else if (turn_time < 0.010) dec = 4;
    else if (turn_time < 0.012) dec = 3;
    else if (turn_time < 0.015) dec = 2;
    else if (turn_time < 0.018) dec = 1;

    dec_out = dec;
    depth -= dec;
    if (depth < 0) depth = 0;
    return depth;
}

// 目的：時間が短いとき候補数を減らして評価の回転数を確保する。
static inline int cap_candidates_by_time(int cap, double turn_time) {
    if (turn_time < 0.008) cap = min(cap, 12);
    else if (turn_time < 0.011) cap = min(cap, 14);
    else if (turn_time < 0.014) cap = min(cap, 16);
    else cap = min(cap, MAX_CAND);
    return cap;
}

// -----------------------------
// Candidate selection refresh: must + topK + random
// -----------------------------
// 目的：合法手全列挙から、探索対象の候補手（最大cand_cap）を作る。
// 方針：
// - 必ず「その場」(stay) を入れる（不測の状況で安全）。
// - いくつかの“重要枠”（高V空き、ターゲット攻撃、脅威防衛、ブロック）を先に入れる。
// - 残りを root_eval_candidate の上位から入れ、少数をランダムで混ぜて探索を広げる。
struct CandFeat {
    Move m;
    double root_sc;
    double v;
    double risk;
    int owner;
    int lvl;
    bool threatened_own;
    bool is_target_cell;
    bool is_empty;
};

// 目的：重複や不正（盤外/非合法）を除いてselに追加する。
static inline bool push_unique(vector<Move> &sel, const Move &m, bool used[MAX_N][MAX_N], const bool legal[MAX_N][MAX_N]) {
    if (m.r < 0 || m.r >= N || m.c < 0 || m.c >= N) return false;
    if (!legal[m.r][m.c]) return false;
    if (used[m.r][m.c]) return false;
    used[m.r][m.c] = true;
    sel.push_back(m);
    return true;
}

// 目的：候補選択の本体。
// 返り値：選んだ候補手列。
// out_*：デバッグ用に、全候補数・選択数・内訳を返す。
static inline vector<Move> select_candidates_refreshed(
    const State &s,
    const vector<Move> &all_moves,
    int cand_cap,
    int leader,
    const TargetInfo &ti,
    const double riskW[MAX_N][MAX_N],
    uint64_t sel_seed,
    int &out_all_sz,
    int &out_sel_sz,
    int &out_must_sz,
    int &out_topk_sz,
    int &out_rand_sz
) {
    out_all_sz = (int)all_moves.size();

    bool legal[MAX_N][MAX_N];
    memset(legal, 0, sizeof(legal));
    for (const auto &m : all_moves) {
        if (0 <= m.r && m.r < N && 0 <= m.c && m.c < N) legal[m.r][m.c] = true;
    }

    vector<CandFeat> feats;
    feats.reserve(all_moves.size());

    // 目的：全合法手の特徴量（root_scなど）を計算。
    for (const auto &mv : all_moves) {
        int r = mv.r, c = mv.c;
        int o = s.owner[r][c];
        int lv = s.level[r][c];
        bool empty = (o == -1);
        bool targ = (o != -1 && o != 0 && ti.is_target[o]);
        bool thr_own = (o == 0 && lv < U && has_adjacent_enemy(s, r, c));

        CandFeat f;
        f.m = mv;
        f.root_sc = root_eval_candidate(s, leader, ti, riskW, mv);
        f.v = (double)V[r][c];
        f.risk = riskW[r][c];
        f.owner = o;
        f.lvl = lv;
        f.threatened_own = thr_own;
        f.is_target_cell = targ;
        f.is_empty = empty;
        feats.push_back(f);
    }

    // 目的：root_sc降順（同点はVなどで整列）に並べるためのインデックス列。
    vector<int> idx(feats.size());
    for (int i=0; i<(int)idx.size(); ++i) idx[i] = i;
    sort(idx.begin(), idx.end(), [&](int a, int b){
        if (feats[a].root_sc != feats[b].root_sc) return feats[a].root_sc > feats[b].root_sc;
        if (feats[a].v != feats[b].v) return feats[a].v > feats[b].v;
        if (feats[a].m.r != feats[b].m.r) return feats[a].m.r < feats[b].m.r;
        return feats[a].m.c < feats[b].m.c;
    });

    bool used[MAX_N][MAX_N];
    memset(used, 0, sizeof(used));
    vector<Move> sel;
    sel.reserve(cand_cap);

    out_must_sz = 0;
    out_topk_sz = 0;
    out_rand_sz = 0;

    // must：その場（合法なら）を必ず入れる。
    Move stay{ s.pos_r[0], s.pos_c[0] };
    if (push_unique(sel, stay, used, legal)) {
        out_must_sz++;
    } else if (!all_moves.empty()) {
        push_unique(sel, all_moves[0], used, legal);
        out_must_sz++;
    }

    // 目的：重要枠（バケット）を先に入れるためのヘルパ。
    auto add_bucket = [&](const vector<int> &cand_ids, int take) {
        int added = 0;
        for (int id : cand_ids) {
            if ((int)sel.size() >= cand_cap) break;
            if (added >= take) break;
            if (push_unique(sel, feats[id].m, used, legal)) {
                out_must_sz++;
                added++;
            }
        }
    };

    // バケット1：高Vの空きセル
    {
        vector<int> vempty;
        for (int id : idx) if (feats[id].is_empty) vempty.push_back(id);
        sort(vempty.begin(), vempty.end(), [&](int a, int b){
            if (feats[a].v != feats[b].v) return feats[a].v > feats[b].v;
            return feats[a].root_sc > feats[b].root_sc;
        });
        add_bucket(vempty, 2);
    }

    // バケット2：ターゲットのレベル1（奪いやすい）
    {
        vector<int> at;
        for (int id : idx) if (feats[id].is_target_cell && feats[id].lvl == 1) at.push_back(id);
        sort(at.begin(), at.end(), [&](int a, int b){
            if (feats[a].v != feats[b].v) return feats[a].v > feats[b].v;
            return feats[a].root_sc > feats[b].root_sc;
        });
        add_bucket(at, 2);
    }

    // バケット3：自セルの防衛（隣接敵がいてレベル<U）
    {
        vector<int> df;
        for (int id : idx) if (feats[id].threatened_own) df.push_back(id);
        sort(df.begin(), df.end(), [&](int a, int b){
            double sa = feats[a].risk * 10.0 + (feats[a].lvl == 1 ? 5.0 : 0.0) + feats[a].v * 0.001;
            double sb = feats[b].risk * 10.0 + (feats[b].lvl == 1 ? 5.0 : 0.0) + feats[b].v * 0.001;
            if (sa != sb) return sa > sb;
            return feats[a].root_sc > feats[b].root_sc;
        });
        add_bucket(df, 2);
    }

    // バケット4：ブロック（相手が来そうな高リスクセルで、自分の所有でない）
    {
        vector<int> bk;
        for (int id : idx) {
            if (feats[id].owner == 0) continue;
            if (feats[id].risk <= 1e-12) continue;
            bk.push_back(id);
        }
        sort(bk.begin(), bk.end(), [&](int a, int b){
            double sa = feats[a].risk + 0.002 * feats[a].v;
            double sb = feats[b].risk + 0.002 * feats[b].v;
            if (sa != sb) return sa > sb;
            return feats[a].root_sc > feats[b].root_sc;
        });
        add_bucket(bk, 2);
    }

    // 残り：上位（topK）+ ランダム（探索の多様性）
    int remaining_slots = cand_cap - (int)sel.size();
    if (remaining_slots < 0) remaining_slots = 0;

    int rand_target = 0;
    if (cand_cap >= 16) rand_target = 4;
    else if (cand_cap >= 14) rand_target = 3;
    else rand_target = 2;
    rand_target = min(rand_target, remaining_slots);

    int top_target = remaining_slots - rand_target;

    int top_added = 0;
    for (int id : idx) {
        if ((int)sel.size() >= cand_cap) break;
        if (top_added >= top_target) break;
        if (push_unique(sel, feats[id].m, used, legal)) {
            out_topk_sz++;
            top_added++;
        }
    }

    // ランダム枠：まだ入っていない候補から一様に拾う
    vector<int> rem;
    rem.reserve(idx.size());
    for (int id : idx) {
        const Move &m = feats[id].m;
        if (!used[m.r][m.c]) rem.push_back(id);
    }

    Xorshift rr(sel_seed ^ 0x9e3779b97f4a7c15ULL);
    for (int i=0; i<rand_target && !rem.empty() && (int)sel.size() < cand_cap; ++i) {
        int j = (int)(rr.next_u64() % (uint64_t)rem.size());
        int id = rem[j];
        rem[j] = rem.back();
        rem.pop_back();
        if (push_unique(sel, feats[id].m, used, legal)) {
            out_rand_sz++;
        } else {
            i--;
        }
    }

    // もし重複等で埋まらなければ、上位から埋める。
    for (int id : idx) {
        if ((int)sel.size() >= cand_cap) break;
        if (push_unique(sel, feats[id].m, used, legal)) out_topk_sz++;
    }

    out_sel_sz = (int)sel.size();
    return sel;
}

// -----------------------------
// main
// -----------------------------
// 目的：入出力ループ（Tターン）。
// 方針：
// - 各ターン：
//   1) 現在状態turn_startを保持
//   2) 相手モデルから ai_params（best_param）を作る
//   3) ターゲット/リスクマップ/合法手列挙
//   4) 候補選別
//   5) 候補ごとに1回ずつ評価（CRN）
//   6) 残り時間までUCBで追加評価を回し、平均最大を採用して出力
//   7) 実際の相手手と次状態を読み取り、モデル更新（observe）
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!read_int(N) || !read_int(M) || !read_int(T) || !read_int(U)) return 0;

    VMAX = 1;
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) {
        if (!read_int(V[i][j])) return 0;
        VMAX = max(VMAX, V[i][j]);
    }

    State state;
    state.init_empty();
    for (int p=0; p<M; ++p) {
        if (!read_int(state.pos_r[p]) || !read_int(state.pos_c[p])) return 0;
        if (0 <= state.pos_r[p] && state.pos_r[p] < N && 0 <= state.pos_c[p] && state.pos_c[p] < N) {
            state.owner[state.pos_r[p]][state.pos_c[p]] = p;
            state.level[state.pos_r[p]][state.pos_c[p]] = 1;
        }
    }

    reachable_territory.reserve(MAX_N * MAX_N);

    // 目的：相手ごとのAIModelを初期化（Vに依存したseed）。
    array<AIModel, MAX_M> ai_models;
    {
        uint64_t seed = 123456789ULL;
        for (int r=0; r<N; ++r) for (int c=0; c<N; ++c) seed = seed * 1315423911ULL + (uint64_t)V[r][c];
        Xorshift init_rng(seed);
        for (int p=1; p<M; ++p) ai_models[p].init(init_rng);
    }

    const double TIME_LIMIT = 1.94;

    // 目的：デバッグを全ターンではなく、最大9点だけ出す（負荷とログ量の削減）。
    vector<int> dbg_turns;
    {
        int K = min(9, max(0, T));
        dbg_turns.reserve(K);
        if (T > 0 && K > 0) {
            int prev = -1;
            for (int i=0; i<K; ++i) {
                int t = (int)((long long)i * (T - 1) / max(1, (K - 1)));
                if (t <= prev) t = prev + 1;
                if (t >= T) break;
                dbg_turns.push_back(t);
                prev = t;
            }
        }
    }
    int dbg_idx = 0;

    cerr << "DBG hdr N=" << N << " M=" << M << " T=" << T << " U=" << U
         << " VMAX=" << VMAX << " TIME_LIMIT=" << TIME_LIMIT
         << " MAX_CAND=" << MAX_CAND << " UCB_C=" << 0.21 << "\n";

    vector<Move> all_moves;
    all_moves.reserve(128);

    int depth_safety_fired = 0;

    for (int t=0; t<T; ++t) {
        const double t_begin = global_timer.get_sec();
        double remain = TIME_LIMIT - t_begin;
        if (remain < 0.0) remain = 0.0;

        // 目的：残りターンで単純割りして、このターンの時間目安を作る。
        double turn_time = remain / max(1, (T - t));
        double end_time = t_begin + max(0.004, turn_time);
        if (end_time > TIME_LIMIT - 0.001) end_time = TIME_LIMIT - 0.001;

        State turn_start = state;

        // 目的：このターンの相手パラメータは「最尤（best_param）」で固定して使う。
        array<AIParam, MAX_M> ai_params;
        for (int p=0; p<M; ++p) ai_params[p] = {0.6,0.6,0.6,0.6,0.25};
        for (int p=1; p<M; ++p) ai_params[p] = ai_models[p].best_param();

        int leader = compute_leader_ai(turn_start);
        TargetInfo ti = compute_target_info(turn_start);

        double riskW[MAX_N][MAX_N];
        compute_target_risk_map(turn_start, ai_params, ti.is_target, riskW);

        get_valid_moves(turn_start, 0, all_moves);
        if (all_moves.empty()) all_moves.push_back({turn_start.pos_r[0], turn_start.pos_c[0]});

        // 目的：探索深さを決める（ターンによる基本 + 時間による安全減衰）。
        int depth0 = base_depth_by_turn(t);
        depth0 = min(depth0, T - t - 1);
        if (depth0 < 0) depth0 = 0;
        int dec = 0;
        int depth = apply_depth_safety(depth0, turn_time, dec);
        if (dec > 0) depth_safety_fired++;

        // 目的：候補数の上限を時間で調整。
        int cand_cap = cap_candidates_by_time(MAX_CAND, turn_time);

        // 目的：候補選別の乱数（ランダム枠）をターンと位置で決める。
        uint64_t sel_seed = 0xC0FFEE12345678ULL;
        sel_seed ^= (uint64_t)t * 0x9e3779b97f4a7c15ULL;
        sel_seed ^= (uint64_t)(turn_start.pos_r[0] * 1315423911u + turn_start.pos_c[0] * 2654435761u);

        int all_sz=0, sel_sz=0, must_sz=0, topk_sz=0, rand_sz=0;
        vector<Move> top = select_candidates_refreshed(
            turn_start, all_moves, cand_cap, leader, ti, riskW, sel_seed,
            all_sz, sel_sz, must_sz, topk_sz, rand_sz
        );

        const bool is_dbg_turn = (dbg_idx < (int)dbg_turns.size() && t == dbg_turns[dbg_idx]);

        const int K = (int)top.size();
        Move best_move{turn_start.pos_r[0], turn_start.pos_c[0]};
        double best_avg = -1e100;
        int total_sims = 0;
        int loop_sims = 0;

        // 目的：候補集合topに対して、UCBで追加シミュレーション配分しながら平均最大を選ぶ。
        if (K > 0) {
            array<double, MAX_CAND> sum;
            array<int, MAX_CAND> cnt;
            for (int i=0; i<K; ++i) { sum[i] = 0.0; cnt[i] = 0; }

            // 目的：ターン固有の基準seed（候補に依存させない）。
            uint64_t turn_base = 88172645463393265ULL;
            turn_base ^= (uint64_t)t * 0x9e3779b97f4a7c15ULL;
            turn_base ^= (uint64_t)(turn_start.pos_r[0] * 31 + turn_start.pos_c[0] * 131);

            int total = 0;

            // 初期：全候補を1回ずつ（公平に平均を持つ）
            for (int i=0; i<K; ++i) {
                uint64_t sseed = make_rollout_seed(turn_base, 1);
                double sc = run_simulation_2ply(turn_start, top[i], depth, ai_params, sseed);
                sum[i] += sc;
                cnt[i]++;
                total++;
            }

            const double UCB_C = 0.21;

            int loop = 0;
            int check_mask = 15;

            // 追加：時間までUCB最大の候補を回す
            while (true) {
                if ((loop & check_mask) == 0) {
                    double nowt = global_timer.get_sec();
                    if (nowt > end_time) break;
                    if (end_time - nowt < 0.02) check_mask = 7;
                    if (end_time - nowt < 0.01) check_mask = 3;
                }

                int best_i = 0;
                double best_ucb = -1e100;
                double log_total = log((double)max(1, total));

                for (int i=0; i<K; ++i) {
                    double avg = sum[i] / (double)cnt[i];
                    double ucb = avg + UCB_C * sqrt(log_total / (double)cnt[i]);
                    if (ucb > best_ucb) { best_ucb = ucb; best_i = i; }
                }

                uint64_t sample_id = (uint64_t)cnt[best_i] + 1;
                uint64_t sseed = make_rollout_seed(turn_base, sample_id);
                double sc = run_simulation_2ply(turn_start, top[best_i], depth, ai_params, sseed);
                sum[best_i] += sc;
                cnt[best_i]++;
                total++;
                loop++;
            }

            // 最終：平均が最大の候補を出力
            int best_move_idx = 0;
            best_avg = -1e100;
            for (int i=0; i<K; ++i) {
                double avg = sum[i] / (double)cnt[i];
                if (avg > best_avg) { best_avg = avg; best_move_idx = i; }
            }
            best_move = top[best_move_idx];
            total_sims = total;
            loop_sims = total - K;
        }

        cout << best_move.r << ' ' << best_move.c << '\n' << flush;

        // 目的：デバッグ出力（最大9ターン）
        if (is_dbg_turn) {
            const double t_end = global_timer.get_sec();
            cerr << "DBG t=" << t
                 << " used=" << (t_end - t_begin)
                 << " remain=" << remain
                 << " turn_time=" << turn_time
                 << " end_in=" << (end_time - t_begin)
                 << " depth=" << depth
                 << " dec=" << dec
                 << " cand_cap=" << cand_cap
                 << " cand=" << all_sz << "->" << sel_sz
                 << " pick(must/top/rand)=" << must_sz << "/" << topk_sz << "/" << rand_sz
                 << " sims=" << total_sims
                 << " loop=" << loop_sims
                 << " best_avg=" << best_avg
                 << " leader=" << ti.leader
                 << " inc2=" << (ti.include_second ? 1 : 0)
                 << " attack_w=" << ti.attack_w
                 << "\n";
            dbg_idx++;
        }

        // 目的：このターンの相手の実際の移動先を読み取る（観測）。
        int tx_r[MAX_M], tx_c[MAX_M];
        for (int p=0; p<M; ++p) {
            if (!read_int(tx_r[p]) || !read_int(tx_c[p])) return 0;
        }

        // 目的：相手モデルを観測で更新（合法手集合Bを作って尤度更新）。
        for (int p=1; p<M; ++p) {
            vector<Move> B;
            B.reserve(128);
            get_valid_moves(turn_start, p, B);
            ai_models[p].observe(turn_start, p, tx_r[p], tx_c[p], B);
        }

        // 目的：次状態（位置、owner、level）を読み取り、stateを更新。
        for (int p=0; p<M; ++p) {
            if (!read_int(state.pos_r[p]) || !read_int(state.pos_c[p])) return 0;
        }
        for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) {
            if (!read_int(state.owner[i][j])) return 0;
        }
        for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) {
            if (!read_int(state.level[i][j])) return 0;
        }
    }

    cerr << "DBG summary depth_safety_fired=" << depth_safety_fired << "\n";
    return 0;
}
