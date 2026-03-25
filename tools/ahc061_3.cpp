#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <array>
#include <cstdint>

using namespace std;

// =============================================================
// 乱数生成器: Xorshift64
// - next_u64(): 64bit乱数
// - next_int(n): [0, n) の整数乱数
// - next_double(): [0,1) の実数乱数
// =============================================================
struct Xorshift {
    uint64_t x;
    Xorshift(uint64_t seed=123456789ULL) : x(seed ? seed : 123456789ULL) {}
    inline uint64_t next_u64() {
        // XORSHIFTの典型的な更新
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        return x;
    }
    inline int next_int(int n) {
        if (n <= 0) return 0;
        return (int)(next_u64() % (uint64_t)n);
    }
    inline double next_double() {
        // 53bit相当の乱数からdoubleに変換して [0,1) を作る
        return (next_u64() >> 11) * (1.0 / 9007199254740992.0);
    }
};

// =============================================================
// splitmix64
// - seedの生成・分岐用。xを更新しつつ、分布の良い64bit値を返す。
// - UCBの試行ごとに異なるseedを作る用途など。
// =============================================================
static inline uint64_t splitmix64(uint64_t &x) {
    uint64_t z = (x += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

// 盤面最大サイズ（問題制約に合わせた固定上限）
constexpr int MAX_N = 10;
constexpr int MAX_M = 10;

// 1ターンに検討する候補手の最大数
constexpr int MAX_CAND = 24;

int N, M, T, U;            // N:盤面サイズ, M:プレイヤー数, T:ターン数, U:レベル上限
int V[MAX_N][MAX_N];       // マスの価値（点数係数）


// =============================================================
// Timer
// - 実行時間制限（例: 1.94秒）内でターンごとに使える時間を配分するために使う
// =============================================================
struct Timer {
    chrono::high_resolution_clock::time_point start;
    Timer() { reset(); }
    void reset() { start = chrono::high_resolution_clock::now(); }
    double get_sec() const {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now - start).count() / 1000.0;
    }
} global_timer;


// =============================================================
// FastQueue
// - BFS用の固定長キュー
// - vector/queueより軽い（小さい盤面で高速化狙い）
// =============================================================
template<typename T, int CAP>
struct FastQueue {
    T data[CAP];
    int head = 0, tail = 0;
    inline void clear() { head = 0; tail = 0; }
    inline void push(const T &v) { data[tail++] = v; }
    inline T pop() { return data[head++]; }
    inline bool empty() const { return head == tail; }
};


// =============================================================
// State: ゲーム状態
// - pos_r/pos_c: 各プレイヤーの現在位置
// - owner[r][c]: マス(r,c)を支配しているプレイヤーID（-1は無所属）
// - level[r][c]: そのマスのレベル（0は未占領）
// =============================================================
struct State {
    int pos_r[MAX_M];
    int pos_c[MAX_M];
    int owner[MAX_N][MAX_N];
    int level[MAX_N][MAX_N];

    // 盤面を空に初期化（全マス無所属、レベル0）
    void init_empty() {
        for (int r=0; r<N; ++r) {
            for (int c=0; c<N; ++c) {
                owner[r][c] = -1;
                level[r][c] = 0;
            }
        }
    }

    // 各プレイヤーのスコアを計算
    // スコア = Σ(V[r][c] * level[r][c]) （自分の領土のみ）
    inline void compute_scores(long long scores_out[MAX_M]) const {
        for (int i=0; i<MAX_M; ++i) scores_out[i] = 0;
        for (int r=0; r<N; ++r) {
            for (int c=0; c<N; ++c) {
                int o = owner[r][c];
                if (o != -1) scores_out[o] += 1LL * V[r][c] * level[r][c];
            }
        }
    }

    // 目的関数（探索の評価値）
    // - 自分(0)のスコアが高く、かつ他者の最大スコアより優位になるほど良い
    // - log(自分+1) - log(他者最大+1) で差を評価（大差ほど大きいが飽和しやすい）
    // - わずかに「領土マス数」「自領土レベル合計」を加点（タイブレーク的）
    double get_score_objective() const {
        long long scores[MAX_M];
        compute_scores(scores);

        int territory_count0 = 0; // 自分領土のマス数
        int level_sum0 = 0;       // 自分領土のレベル合計
        for (int r=0; r<N; ++r) {
            for (int c=0; c<N; ++c) {
                if (owner[r][c] == 0) {
                    territory_count0++;
                    level_sum0 += level[r][c];
                }
            }
        }

        long long s0 = scores[0];
        long long max_other = 1;
        for (int i=1; i<M; ++i) max_other = max(max_other, scores[i]);

        double score = log((double)s0 + 1.0) - log((double)max_other + 1.0);
        score += territory_count0 * 0.0001;
        score += level_sum0 * 0.00001;
        return score;
    }
};

// 1手（移動先マス）
struct Move { int r, c; };


// =============================================================
// 合法手生成 get_valid_moves
//
// ルール概要（このコードが仮定している移動可能範囲）:
// - 自分の領土（owner==自分）の連結成分をBFSで集める（自分の現在位置から到達可能な領土）
// - その領土内のマス、および領土に隣接する上下左右のマスを候補に入れる
// - 他プレイヤーの現在位置は「占有」として移動先にできない
//
// 高速化ポイント:
// - visited_token/added_token を使い、毎回配列を全クリアせず「世代番号」で訪問管理
// =============================================================
static uint32_t visited_token[MAX_N][MAX_N];
static uint32_t current_token = 1;
static uint32_t added_token[MAX_N][MAX_N];
static uint32_t current_added_token = 1;

static FastQueue<pair<int,int>, MAX_N*MAX_N> bfs_q;
static vector<pair<int,int>> reachable_territory;

// tokenを1増やし、オーバーフローしたら配列を0クリアして再開
static inline void token_bump(uint32_t &tok, uint32_t grid[MAX_N][MAX_N]) {
    tok++;
    if (tok == 0) { // overflow wrap
        memset(grid, 0, sizeof(uint32_t)*MAX_N*MAX_N);
        tok = 1;
    }
}

static inline void get_valid_moves(const State& s, int p_id, vector<Move>& out_moves) {
    out_moves.clear();

    // BFS訪問管理用トークン更新
    token_bump(current_token, visited_token);
    const uint32_t tok = current_token;

    int start_r = s.pos_r[p_id];
    int start_c = s.pos_c[p_id];

    // 位置が壊れていた場合のフォールバック（通常は起きない想定）
    if (start_r < 0 || start_r >= N || start_c < 0 || start_c >= N) {
        out_moves.push_back({0,0});
        return;
    }

    // 他プレイヤーが居るマスは移動先にできない（占有扱い）
    bool is_occupied[MAX_N][MAX_N];
    memset(is_occupied, 0, sizeof(is_occupied));
    for (int i=0; i<M; ++i) {
        if (i == p_id) continue;
        int r = s.pos_r[i], c = s.pos_c[i];
        if (0 <= r && r < N && 0 <= c && c < N) is_occupied[r][c] = true;
    }

    static const int dr[4] = {0, 0, 1, -1};
    static const int dc[4] = {1, -1, 0, 0};

    // BFS: 「自分領土 owner==p_id」かつ「startから連結」なマスを列挙
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
                // 次マスが未訪問で、かつ自分領土なら連結成分に追加
                if (visited_token[nr][nc] != tok && s.owner[nr][nc] == p_id) {
                    visited_token[nr][nc] = tok;
                    reachable_territory.push_back({nr, nc});
                    bfs_q.push({nr, nc});
                }
            }
        }
    }

    // out_movesの重複排除用トークン更新
    token_bump(current_added_token, added_token);
    const uint32_t atok = current_added_token;

    // reachable_territory の各マス自身 + その上下左右を候補に入れる
    for (auto &t : reachable_territory) {
        int r = t.first, c = t.second;

        // 自マス（領土内）
        if (added_token[r][c] != atok && !is_occupied[r][c]) {
            added_token[r][c] = atok;
            out_moves.push_back({r,c});
        }

        // 隣接マス
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

    // 念のため空なら「その場に留まる」を入れる
    if (out_moves.empty()) out_moves.push_back({start_r, start_c});
}


// =============================================================
// advance_exact: ルールに従って1ターン進める
//
// 大きく2段階:
// (1) 全員が移動先を宣言した後、同じマスに複数来た場合の衝突解決
//     - 1人だけならその人が移動成功
//     - 複数なら「そのマスのowner」が含まれていればownerが勝者（その人だけ残る）
//       含まれていなければ全員 collected（回収）扱いで移動失敗
// (2) 移動が成功した（collectedされていない）プレイヤーが、到達マスに対して
//     - 空マスなら占領（owner=自分, level=1）
//     - 自分マスなら強化（level++ ただしU上限）
//     - 敵マスなら攻撃（level--、0になったら奪取、0にならなければ攻撃者がcollected）
//
// 最後に位置更新:
// - collectedのプレイヤーは元の位置へ戻る
// - それ以外は到達位置へ
// =============================================================
static inline void advance_exact(State& s, const array<Move, MAX_M>& moves) {
    // ターン開始時位置（collected時に戻るため保存）
    int start_r[MAX_M], start_c[MAX_M];
    for (int p=0; p<M; ++p) {
        start_r[p] = s.pos_r[p];
        start_c[p] = s.pos_c[p];
    }

    // 各マスに「何人が来たか」「誰が来たか」を記録する
    static int target_count[MAX_N][MAX_N];
    static int target_list[MAX_N][MAX_N][MAX_M];
    for (int r=0; r<N; ++r) {
        memset(target_count[r], 0, sizeof(int)*N);
    }

    for (int p=0; p<M; ++p) {
        int r = moves[p].r;
        int c = moves[p].c;
        target_list[r][c][target_count[r][c]++] = p;
    }

    // collected[p]=true: このターンで回収され、移動が無効になり開始位置へ戻る
    bool collected[MAX_M];
    memset(collected, 0, sizeof(collected));

    // 衝突解決後の「暫定位置」
    int new_r[MAX_M], new_c[MAX_M];
    for (int p=0; p<M; ++p) {
        new_r[p] = start_r[p];
        new_c[p] = start_c[p];
    }

    // (1) 衝突解決
    for (int r=0; r<N; ++r) {
        for (int c=0; c<N; ++c) {
            int cnt = target_count[r][c];
            if (cnt == 0) continue;

            int cell_owner = s.owner[r][c];
            int winner = -1;

            if (cnt == 1) {
                // 1人だけならその人が移動成功
                winner = target_list[r][c][0];
            } else {
                // 複数なら「現ownerが参加しているか」を見る
                if (cell_owner != -1) {
                    for (int k=0; k<cnt; ++k) {
                        int p = target_list[r][c][k];
                        if (p == cell_owner) { winner = p; break; }
                    }
                }
                // ownerがいない（またはowner不参加）なら winner=-1 のまま
            }

            if (winner == -1) {
                // 勝者なし: 全員回収
                for (int k=0; k<cnt; ++k) collected[target_list[r][c][k]] = true;
            } else {
                // 勝者以外は回収、勝者だけそのマスへ
                for (int k=0; k<cnt; ++k) {
                    int p = target_list[r][c][k];
                    if (p != winner) collected[p] = true;
                }
                new_r[winner] = r;
                new_c[winner] = c;
            }
        }
    }

    // (2) 領土更新（collectedされていないプレイヤーのみ）
    for (int p=0; p<M; ++p) {
        if (collected[p]) continue;

        int r = new_r[p];
        int c = new_c[p];

        int o = s.owner[r][c];
        if (o == -1) {
            // 空マス: 占領
            s.owner[r][c] = p;
            s.level[r][c] = 1;
        } else if (o == p) {
            // 自分マス: 強化（Uが上限）
            if (s.level[r][c] < U) s.level[r][c]++;
        } else {
            // 敵マス: 攻撃（レベルを1減らす）
            s.level[r][c]--;
            if (s.level[r][c] == 0) {
                // 0になったら奪取（ownerを自分に、レベル1で再スタート）
                s.owner[r][c] = p;
                s.level[r][c] = 1;
            } else {
                // 0にならない場合は攻撃者が回収（開始位置へ戻る）
                collected[p] = true;
            }
        }
    }

    // 位置確定
    for (int p=0; p<M; ++p) {
        if (collected[p]) {
            s.pos_r[p] = start_r[p];
            s.pos_c[p] = start_c[p];
        } else {
            s.pos_r[p] = new_r[p];
            s.pos_c[p] = new_c[p];
        }
    }
}


// =============================================================
// 相手モデル: AIParam / AIModel（粒子フィルタ）
//
// 考え方:
// - 相手プレイヤーは「各マスをどう評価するか」と「ランダムに動く割合 eps」で行動すると仮定
// - 評価パラメータ:
//     wa: 空マスの価値倍率
//     wb: 自分のマス（レベル<U）の価値倍率（=強化したい度合い）
//     wc: 敵マス（レベル1）の価値倍率（=奪いやすいマスを狙う度合い）
//     wd: 敵マス（レベル2以上）の価値倍率（=攻撃しても回収されやすい）
//     eps: 乱択率（epsでランダム、1-epsで貪欲最大）
//
// - observe() は、相手の実際の選択(観測)が「そのパラメータならどれくらい起きやすいか」を
//   尤度として粒子重みを更新し、必要ならリサンプリングする。
// =============================================================
struct AIParam {
    double wa, wb, wc, wd, eps;
};

struct AIParticle {
    AIParam par;
    double w;   // 粒子の重み（確率）
};

struct AIModel {
    static constexpr int P = 24;     // 粒子数（固定）
    array<AIParticle, P> ps;

    // 初期化: パラメータをランダムに撒いて等重みにする
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

    // あるマス(r,c)を「相手p視点で見た種類」に分類する
    // 0: 空
    // 1: 自分のマスで level<U
    // 2: 自分のマスで level==U（これ以上強化できないので価値0扱い）
    // 3: 他人のマスで level==1（奪いやすい）
    // 4: 他人のマスで level>=2
    static inline int cell_type(const State &s, int p, int r, int c) {
        int o = s.owner[r][c];
        if (o == -1) return 0; // empty
        if (o == p) {
            if (s.level[r][c] < U) return 1; // own < U
            return 2; // own == U
        }
        // other
        if (s.level[r][c] == 1) return 3;
        return 4;
    }

    // 相手pが「そのマスへ行く価値」を数値化
    // 基本は V[r][c] に倍率を掛ける。自分のマス(level==U)は0扱い。
    static inline double eval_cell(const AIParam &par, const State &s, int p, int r, int c) {
        int t = cell_type(s, p, r, c);
        if (t == 2) return 0.0; // own==U
        double mult = 0.0;
        if (t == 0) mult = par.wa;
        else if (t == 1) mult = par.wb;
        else if (t == 3) mult = par.wc;
        else mult = par.wd;
        return (double)V[r][c] * mult;
    }

    // 観測更新:
    // - turn_start: ターン開始状態（相手が判断した盤面）
    // - p: 相手ID
    // - obs_r, obs_c: 実際に相手が宣言した移動先（観測）
    // - B: 相手の合法手集合
    //
    // 行動モデル:
    // - epsの確率で B から一様ランダム
    // - 1-epsの確率で eval_cell 最大の手（同値は等確率）
    void observe(const State &turn_start, int p, int obs_r, int obs_c, const vector<Move> &B) {
        if (B.empty()) return;

        // 観測手がBに含まれないなら更新しない（入力不整合などへの保険）
        bool inB = false;
        for (auto &m : B) {
            if (m.r == obs_r && m.c == obs_c) { inB = true; break; }
        }
        if (!inB) return;

        double wsum = 0.0;
        const double invB = 1.0 / (double)B.size();

        // 各粒子について、観測が起こる確率(prob)を計算し、重み *= prob
        for (int i=0; i<P; ++i) {
            const AIParam &par = ps[i].par;

            double best = -1e100;
            int argcnt = 0;
            bool obs_is_arg = false;

            // argmax集合のサイズと、観測手がargmaxに含まれるかを求める
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

            // 貪欲選択で観測手が選ばれる確率（同率argmaxは等確率）
            double p_greedy = 0.0;
            if (obs_is_arg && argcnt > 0) p_greedy = 1.0 / (double)argcnt;

            // 全体確率 = eps*(一様) + (1-eps)*(貪欲)
            double prob = par.eps * invB + (1.0 - par.eps) * p_greedy;

            // 0に落ちると復帰しづらいので下限を置く
            if (prob < 1e-15) prob = 1e-15;

            ps[i].w *= prob;
            wsum += ps[i].w;
        }

        // 正規化できないほど小さい場合はリセット
        if (wsum <= 0.0) {
            double invP = 1.0 / P;
            for (int i=0; i<P; ++i) ps[i].w = invP;
            return;
        }

        // 正規化（重みの合計を1に）
        for (int i=0; i<P; ++i) ps[i].w /= wsum;

        // ESS（effective sample size）で粒子の偏り具合を測る
        // ess = 1 / Σ(w^2)
        double ess_inv = 0.0;
        for (int i=0; i<P; ++i) ess_inv += ps[i].w * ps[i].w;
        double ess = 1.0 / max(1e-18, ess_inv);

        // 偏りが小さいならリサンプリングしない
        if (ess >= P * 0.5) return;

        // systematic resampling:
        // - [0,1) を等間隔step=1/Pでサンプルし、CDFから粒子を複製
        array<AIParticle, P> newps;
        double step = 1.0 / P;

        // u0は観測手から決める（毎回同じ観測なら同じu0）
        // これにより再現性が上がり、不要なランダム揺れを減らす狙い
        uint64_t seed = 1469598103934665603ULL;
        seed ^= (uint64_t)(obs_r * 1315423911u + obs_c * 2654435761u + p * 97531u);
        seed *= 1099511628211ULL;
        uint64_t sm = seed;
        double u0 = (splitmix64(sm) >> 11) * (1.0 / 9007199254740992.0) * step;

        double cdf = ps[0].w;
        int idx = 0;
        for (int i=0; i<P; ++i) {
            double u = u0 + i * step;
            while (u > cdf && idx + 1 < P) {
                idx++;
                cdf += ps[idx].w;
            }
            newps[i].par = ps[idx].par;
            newps[i].w = step; // 等重み（合計1）
        }
        ps = newps;
    }

    // 現在もっとも重い粒子（最有力推定）を返す
    AIParam best_param() const {
        int best = 0;
        for (int i=1; i<P; ++i) if (ps[i].w > ps[best].w) best = i;
        return ps[best].par;
    }
};


// =============================================================
// プレイアウト時の手選択
// - choose_ai_move: 相手の行動モデル（epsでランダム、それ以外で貪欲）
// - choose_my_rollout_move: 自分のプレイアウト用（目的関数を意識した重み付きランダム）
//
// 注意:
// - 本番の手選択はUCB1で評価するが、プレイアウト内の自分手は軽量な方策で動かしている。
// =============================================================
static inline Move choose_ai_move(const State &s, int p_id, const AIParam &par, Xorshift &rng, vector<Move> &tmp) {
    get_valid_moves(s, p_id, tmp);
    if (tmp.empty()) return {s.pos_r[p_id], s.pos_c[p_id]};
    if ((int)tmp.size() == 1) return tmp[0];

    // epsで一様ランダム
    double r = rng.next_double();
    if (r < par.eps) {
        return tmp[rng.next_int((int)tmp.size())];
    }

    // それ以外は eval_cell 最大の手（同値は等確率）
    double best = -1e100;
    int best_cnt = 0;
    Move best_moves[100];

    for (auto &m : tmp) {
        double e = AIModel::eval_cell(par, s, p_id, m.r, m.c);
        if (e > best + 1e-12) {
            best = e;
            best_cnt = 1;
            best_moves[0] = m;
        } else if (fabs(e - best) <= 1e-12) {
            best_moves[best_cnt++] = m;
        }
    }
    return best_moves[rng.next_int(best_cnt)];
}

static inline Move choose_my_rollout_move(const State &s, int leader, Xorshift &rng, vector<Move> &tmp) {
    get_valid_moves(s, 0, tmp);
    if (tmp.empty()) return {s.pos_r[0], s.pos_c[0]};
    if ((int)tmp.size() == 1) return tmp[0];

    // 重み付きランダム:
    // - 空マスは取りたい
    // - 自分マスは状況（脅威の有無、U未満か）で強化したい/したくないが変わる
    // - 敵マスはレベル1を優先（奪取しやすい）
    double weights[100];
    double sum = 0.0;
    static const int dr[4] = {0, 0, 1, -1};
    static const int dc[4] = {1, -1, 0, 0};

    for (int i=0; i<(int)tmp.size(); ++i) {
        int r = tmp[i].r, c = tmp[i].c;
        double w = (double)V[r][c];

        int o = s.owner[r][c];
        if (o == -1) {
            // 空マス: 占領できるので高め
            w *= 3.0;
        } else if (o == 0) {
            // 自分マス
            bool threatened = false;
            // 隣に敵領土があるなら「脅威あり」とみなす
            for (int k=0; k<4; ++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                    int no = s.owner[nr][nc];
                    if (no != -1 && no != 0) { threatened = true; break; }
                }
            }
            if (s.level[r][c] < U) {
                // U未満なら、脅威があるとき強化価値が大きい
                w *= threatened ? 6.0 : 0.6;
            } else {
                // すでにUなら、基本的に行く価値は低いが、脅威があると少しだけ上げる
                w *= threatened ? 0.8 : 0.05;
            }
        } else {
            // 敵マス
            if (s.level[r][c] == 1) {
                // レベル1は奪取しやすい。特にleader（最大得点者）のマスは重点的に狙う
                w *= (o == leader ? 10.0 : 6.0);
            } else {
                // レベル2以上は攻撃しても回収されやすい（無駄になりやすい）ので控えめ
                w *= (o == leader ? 1.2 : 0.8);
            }
        }

        // 0にならないよう最低値を置く（確率0を避ける）
        if (w < 0.01) w = 0.01;
        weights[i] = w;
        sum += w;
    }

    // 重み付き乱択
    double rv = rng.next_double() * sum;
    double cur = 0.0;
    for (int i=0; i<(int)tmp.size(); ++i) {
        cur += weights[i];
        if (rv <= cur) return tmp[i];
    }
    return tmp.back();
}


// =============================================================
// シミュレーション（プレイアウト）
// - run_simulation: 「今の候補手 my_first を採用した場合」の未来をdepth手分進めて評価値を返す
//
// 注意点:
// - ai_params は各相手のパラメータ（best_param）を使う
// - 毎手 leader を計算し直す（自分のロールアウト方策で leader を使うため）
// - ここは最も回数が回るため、重い処理があると総プレイアウト数が減って弱くなる
// =============================================================
static inline int compute_leader_ai(const State &s) {
    long long scores[MAX_M];
    s.compute_scores(scores);
    int leader = 1;
    for (int p=2; p<M; ++p) if (scores[p] > scores[leader]) leader = p;
    return leader;
}

static inline double run_simulation(State s, const Move my_first, int depth,
                                    const array<AIParam, MAX_M> &ai_params,
                                    uint64_t seed) {
    Xorshift rng(seed);

    array<Move, MAX_M> moves;
    // 各手の合法手生成に使う一時vector（この実装ではプレイアウトごとに生成される）
    // ※ここが速度ボトルネックになりうる（改善余地）
    vector<Move> tmp; tmp.reserve(128);

    int leader = compute_leader_ai(s);

    // まず my_first を適用した1手目
    moves[0] = my_first;
    for (int p=1; p<M; ++p) moves[p] = choose_ai_move(s, p, ai_params[p], rng, tmp);
    advance_exact(s, moves);

    // depth回、全員の手を進める
    for (int d=0; d<depth; ++d) {
        leader = compute_leader_ai(s);
        moves[0] = choose_my_rollout_move(s, leader, rng, tmp);
        for (int p=1; p<M; ++p) moves[p] = choose_ai_move(s, p, ai_params[p], rng, tmp);
        advance_exact(s, moves);
    }

    // 最終状態の目的関数値を返す
    return s.get_score_objective();
}


// =============================================================
// 候補剪定用の軽量評価 fast_eval_candidate
// - 本番のプレイアウト前に、候補手を大雑把に並べ替えるための指標
// - 空マスは高め、敵レベル1は高め（特にleaderのマスはさらに高め）
// - 自分マス(level==U)は価値低め
// =============================================================
static inline double fast_eval_candidate(const State &s, const Move &m, int leader) {
    int r = m.r, c = m.c;
    double base = (double)V[r][c];
    int o = s.owner[r][c];
    if (o == -1) return base * 3.0;
    if (o == 0) {
        if (s.level[r][c] < U) return base * 1.0;
        return base * 0.05;
    }
    if (s.level[r][c] == 1) return base * (o == leader ? 10.0 : 6.0);
    return base * (o == leader ? 0.9 : 0.6);
}


// =============================================================
// main
// - 入力読み取り
// - 各ターン:
//   1) 相手モデルから ai_params を構築（best_param）
//   2) 自分の合法手生成
//   3) fast_evalで上位候補を抽出（最大24）
//   4) UCB1でプレイアウト回数を配分し、最良手を選ぶ
//   5) 出力後、ジャッジから「各プレイヤーの宣言手」「更新後の状態」を読み、相手モデルを更新
// =============================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 入力: N M T U, 次にV
    if (!(cin >> N >> M >> T >> U)) return 0;
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) cin >> V[i][j];

    // 初期状態構築
    State state;
    state.init_empty();
    for (int p=0; p<M; ++p) {
        cin >> state.pos_r[p] >> state.pos_c[p];
        state.owner[state.pos_r[p]][state.pos_c[p]] = p;
        state.level[state.pos_r[p]][state.pos_c[p]] = 1;
    }

    // BFSで使う領土配列の予約（10x10なので小さいが、再確保回避）
    reachable_territory.reserve(MAX_N * MAX_N);

    // 相手モデル初期化（盤面Vからseedを作り、再現性を持たせる）
    array<AIModel, MAX_M> ai_models;
    {
        uint64_t seed = 123456789ULL;
        for (int r=0; r<N; ++r) for (int c=0; c<N; ++c) seed = seed * 1315423911ULL + (uint64_t)V[r][c];
        Xorshift init_rng(seed);
        for (int p=1; p<M; ++p) ai_models[p].init(init_rng);
    }

    const double TIME_LIMIT = 1.94; // 全体制限時間（秒）

    vector<Move> candidates;
    candidates.reserve(128);

    for (int t=0; t<T; ++t) {
        // ターンごとの使える時間を「残り時間 / 残りターン数」で配分する
        const double cur_time = global_timer.get_sec();
        double remain = TIME_LIMIT - cur_time;
        if (remain < 0.0) remain = 0.0;
        double turn_time = remain / max(1, (T - t));
        double end_time = cur_time + max(0.005, turn_time);

        // turn_start: このターン開始時の状態（相手の判断もこの状態に基づく）
        State turn_start = state;

        // 相手パラメータの推定値（最有力粒子）
        array<AIParam, MAX_M> ai_params;
        // p=0はダミー（相手用しか使わないが初期化しておく）
        for (int p=0; p<M; ++p) ai_params[p] = {0.6,0.6,0.6,0.6,0.25};
        for (int p=1; p<M; ++p) ai_params[p] = ai_models[p].best_param();

        int leader = compute_leader_ai(turn_start);

        // 自分の合法手を列挙
        get_valid_moves(turn_start, 0, candidates);
        if (candidates.empty()) candidates.push_back({turn_start.pos_r[0], turn_start.pos_c[0]});

        // 候補を軽量評価して降順に並べる（上位MAX_CANDだけ使う）
        struct ScoredMove { double s; Move m; };
        static vector<ScoredMove> scored;
        scored.clear();
        scored.reserve(candidates.size());
        for (auto &m : candidates) {
            scored.push_back({fast_eval_candidate(turn_start, m, leader), m});
        }
        sort(scored.begin(), scored.end(), [](const ScoredMove &a, const ScoredMove &b){ return a.s > b.s; });

        // 「その場に留まる」を候補に必ず含めたい
        Move stay{turn_start.pos_r[0], turn_start.pos_c[0]};

        array<Move, MAX_CAND> top;
        int K = 0;
        bool has_stay = false;
        for (auto &sm : scored) {
            if (K >= MAX_CAND) break;
            top[K++] = sm.m;
            if (sm.m.r == stay.r && sm.m.c == stay.c) has_stay = true;
        }
        if (!has_stay) {
            if (K < MAX_CAND) top[K++] = stay;
            else top[K-1] = stay;
        }

        // プレイアウトの深さ（ターンが進むほど深くする）
        int depth;
        if (t < 30) depth = 6;
        else if (t < 70) depth = 8;
        else depth = 10;
        depth = min(depth, T - t - 1);
        if (depth < 0) depth = 0;

        // UCB1の統計値:
        // sum[i]: 候補iの評価値合計
        // cnt[i]: 候補iの試行回数
        array<double, MAX_CAND> sum;
        array<int, MAX_CAND> cnt;
        for (int i=0; i<K; ++i) { sum[i] = 0.0; cnt[i] = 0; }

        // seedの基準値（ターン番号と自分位置で決める）
        // → 同じ状況ならプレイアウトの乱数系列が安定しやすい
        uint64_t base = 88172645463393265ULL;
        base ^= (uint64_t)t * 0x9e3779b97f4a7c15ULL;
        base ^= (uint64_t)(turn_start.pos_r[0] * 31 + turn_start.pos_c[0] * 131);

        // まずは各候補を1回ずつ試す（初期化）
        int total = 0;
        for (int i=0; i<K; ++i) {
            uint64_t sseed = base;
            sseed ^= (uint64_t)i * 0xBF58476D1CE4E5B9ULL;
            sseed ^= (uint64_t)(total + 1) * 0x94D049BB133111EBULL;
            double sc = run_simulation(turn_start, top[i], depth, ai_params, sseed);
            sum[i] += sc;
            cnt[i]++;
            total++;
        }

        // UCB1:
        // ucb = avg + C * sqrt(log(total)/cnt)
        // Cが大きいほど「未探索」を重視
        const double C = 0.35;
        int loop = 0;
        while (true) {
            // 時間チェックは毎回やると重いので、ある程度まとめて行う
            if ((loop & 63) == 0) {
                if (global_timer.get_sec() > end_time) break;
            }

            int best_i = 0;
            double best_ucb = -1e100;
            double log_total = log((double)total);

            // 次に試す候補（UCB最大）を選ぶ
            for (int i=0; i<K; ++i) {
                double avg = sum[i] / (double)cnt[i];
                double ucb = avg + C * sqrt(log_total / (double)cnt[i]);
                if (ucb > best_ucb) {
                    best_ucb = ucb;
                    best_i = i;
                }
            }

            // 選んだ候補をもう1回プレイアウト
            uint64_t sseed = base;
            sseed ^= (uint64_t)best_i * 0xBF58476D1CE4E5B9ULL;
            sseed ^= (uint64_t)(total + 1) * 0x94D049BB133111EBULL;
            double sc = run_simulation(turn_start, top[best_i], depth, ai_params, sseed);
            sum[best_i] += sc;
            cnt[best_i]++;
            total++;
            loop++;
        }

        // 最終的には「平均評価値が最大」の候補を選んで出力
        int best_move_idx = 0;
        double best_avg = -1e100;
        for (int i=0; i<K; ++i) {
            double avg = sum[i] / (double)cnt[i];
            if (avg > best_avg) {
                best_avg = avg;
                best_move_idx = i;
            }
        }
        Move best_move = top[best_move_idx];

        // 自分の手を出力
        cout << best_move.r << ' ' << best_move.c << '\n' << flush;

        // ここから先は「ジャッジからのフィードバック読み取り」
        // tx_r[p], tx_c[p]: 各プレイヤーが実際に宣言した移動先（観測）
        int tx_r[MAX_M], tx_c[MAX_M];
        for (int p=0; p<M; ++p) cin >> tx_r[p] >> tx_c[p];

        // 相手モデル更新:
        // - turn_start と観測 tx を使う（相手が意思決定したのは turn_start の盤面だから）
        for (int p=1; p<M; ++p) {
            vector<Move> B;
            B.reserve(128);
            get_valid_moves(turn_start, p, B);
            ai_models[p].observe(turn_start, p, tx_r[p], tx_c[p], B);
        }

        // ジャッジが返す「ターン終了後の状態」を読み取って次ターンへ
        for (int p=0; p<M; ++p) cin >> state.pos_r[p] >> state.pos_c[p];
        for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) cin >> state.owner[i][j];
        for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) cin >> state.level[i][j];
    }

    return 0;
}
