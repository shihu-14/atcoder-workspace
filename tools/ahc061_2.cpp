#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstring>
#include <array>
#include <iomanip>

using namespace std;

// ------------------------------------------------------------------
// 高速乱数生成 (Xorshift)
// ------------------------------------------------------------------
struct Xorshift {
    uint64_t x = 123456789;
    inline uint64_t next() {
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        return x;
    }
    inline int next_int(int n) {
        return next() % n;
    }
    inline double next_double() {
        return (double)next() / 0xffffffffffffffff;
    }
} rng;

// ------------------------------------------------------------------
// 設定・グローバル変数
// ------------------------------------------------------------------
constexpr int MAX_N = 10;
constexpr int MAX_M = 10;

int N, M, T, U;
int V[MAX_N][MAX_N]; // マスの価値

// 時間計測用クラス
struct Timer {
    chrono::high_resolution_clock::time_point start;
    
    Timer() {
        reset();
    }
    void reset() {
        start = chrono::high_resolution_clock::now();
    }
    double get_sec() {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now - start).count() / 1000.0;
    }
} global_timer;

// ------------------------------------------------------------------
// 盤面状態 (固定長配列で高速化)
// ------------------------------------------------------------------
struct State {
    int pos_r[MAX_M];
    int pos_c[MAX_M];
    int owner[MAX_N][MAX_N];
    int level[MAX_N][MAX_N];

    // 初期化
    void init_empty() {
        for(int i=0; i<N; ++i) {
            for(int j=0; j<N; ++j) {
                owner[i][j] = -1;
                level[i][j] = 0;
            }
        }
    }

    // スコア計算 (log比率)
    double get_score_objective() const {
        long long scores[MAX_M] = {0};
        
        for(int r=0; r<N; ++r) {
            for(int c=0; c<N; ++c) {
                int o = owner[r][c];
                if(o != -1) {
                    scores[o] += (long long)V[r][c] * level[r][c];
                }
            }
        }
        
        long long s0 = scores[0];
        long long max_other = 1;
        for(int i=1; i<M; ++i) {
            if(scores[i] > max_other) max_other = scores[i];
        }
        
        // log(S0) - log(S_rival)
        return log(s0 + 1.0) - log(max_other + 1.0);
    }
};

// ------------------------------------------------------------------
// 移動ロジック
// ------------------------------------------------------------------

// 移動先候補を表す構造体
struct Move {
    int r, c;
};

// BFS用の訪問管理 (staticで使い回して高速化)
int visited_token[MAX_N][MAX_N];
int current_token = 0;

// 有効な移動先を列挙
// ルール: 
// 1. 現在地から、自分の領土だけを通って到達できるマス
// 2. その到達可能マスに隣接するマス
// 3. ただし、現在、敵がいるマスへは移動不可
void get_valid_moves(const State& s, int p_id, vector<Move>& out_moves) {
    out_moves.clear();
    
    current_token++; // 訪問済みフラグのリセット代わり
    
    int start_r = s.pos_r[p_id];
    int start_c = s.pos_c[p_id];
    
    // フェイルセーフ
    if(start_r < 0 || start_r >= N || start_c < 0 || start_c >= N) {
        out_moves.push_back({0, 0});
        return;
    }

    // BFSキュー (固定長配列による簡易リングバッファで高速化も可能だが、ここではqueueを使う)
    // Nが小さいのでqueueでも十分速い
    queue<pair<int, int>> q;
    q.push({start_r, start_c});
    visited_token[start_r][start_c] = current_token;
    
    // 現在地待機は常に可能（敵衝突チェック不要：自分がいるので）
    out_moves.push_back({start_r, start_c});

    // 敵の位置マップ作成
    bool is_occupied[MAX_N][MAX_N] = {false};
    for(int i=0; i<M; ++i) {
        if(i == p_id) continue;
        int er = s.pos_r[i];
        int ec = s.pos_c[i];
        if(er >= 0 && er < N && ec >= 0 && ec < N) {
            is_occupied[er][ec] = true;
        }
    }

    int dr[] = {0, 0, 1, -1};
    int dc[] = {1, -1, 0, 0};

    while(!q.empty()) {
        auto curr = q.front();
        q.pop();
        
        for(int i=0; i<4; ++i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if(nr >= 0 && nr < N && nc >= 0 && nc < N) {
                // まだチェックしていないマスなら
                // (Moveとしての追加はここで行わないと、BFSで到達するたびに追加されてしまう)
                // -> visited_tokenとは別に「追加済みフラグ」が必要だが、
                // visited_tokenは「領土として到達済み」を管理するもの。
                // 「隣接マス」として追加済みかは別途管理が必要。
                // 簡略化のため、vectorに追加後、呼び出し元で重複削除するか、setを使う。
                // 高速化のため、vectorに追加しつつ、フラグ管理する。
                
                // ここでは「到達可能領土」と「隣接マス」を区別する。
                // 隣接マスは移動候補になる。
                // 領土ならさらに探索を広げる。
                
                // 移動候補として有効か？（敵がいないか）
                if(!is_occupied[nr][nc]) {
                    // まだリストに追加していなければ追加したいが、
                    // 重複チェックコストがかかる。
                    // 許容して最後にsort/uniqueするか、setを使うか。
                    // N=10なのでsetでも許容範囲。
                    // 速度優先のため、一旦重複ありで入れて、後でuniqueする戦略は取らず、
                    // 「移動先として追加したかフラグ」を持つのが最速。
                }
                
                // BFSの継続判定: 自分の領土ならキューに入れる
                if(visited_token[nr][nc] != current_token && s.owner[nr][nc] == p_id) {
                    visited_token[nr][nc] = current_token;
                    q.push({nr, nc});
                }
            }
        }
    }

    // 上記BFSだと「隣接マス」の列挙が漏れる・重複するので、
    // 正確かつ高速な実装:
    // 1. BFSで「到達可能な自分の領土」を全て列挙し、マークする。
    // 2. マークされたマスおよびその隣接マスを全て候補とする（敵がいる場所を除く）。
    
    // 再実装
    current_token++;
    int territory_token = current_token; // 領土として到達可能
    
    // 到達可能領土リスト
    static vector<pair<int,int>> reachable_territory;
    reachable_territory.clear();
    
    q.push({start_r, start_c});
    visited_token[start_r][start_c] = territory_token;
    reachable_territory.push_back({start_r, start_c});
    
    while(!q.empty()){
        auto curr = q.front();
        q.pop();
        for(int i=0; i<4; ++i){
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];
            if(nr >= 0 && nr < N && nc >= 0 && nc < N) {
                if(visited_token[nr][nc] != territory_token && s.owner[nr][nc] == p_id) {
                    visited_token[nr][nc] = territory_token;
                    reachable_territory.push_back({nr, nc});
                    q.push({nr, nc});
                }
            }
        }
    }
    
    // 候補列挙 (重複排除用フラグ)
    static int added_token[MAX_N][MAX_N];
    static int current_added_token = 0;
    current_added_token++;
    
    out_moves.clear();
    
    for(auto& t : reachable_territory) {
        // 領土そのものも移動先候補 (待機)
        if(added_token[t.first][t.second] != current_added_token && !is_occupied[t.first][t.second]) {
            added_token[t.first][t.second] = current_added_token;
            out_moves.push_back({t.first, t.second});
        }
        
        // 隣接マス
        for(int i=0; i<4; ++i) {
            int nr = t.first + dr[i];
            int nc = t.second + dc[i];
            if(nr >= 0 && nr < N && nc >= 0 && nc < N) {
                if(added_token[nr][nc] != current_added_token && !is_occupied[nr][nc]) {
                    added_token[nr][nc] = current_added_token;
                    out_moves.push_back({nr, nc});
                }
            }
        }
    }
    
    // もし候補が空なら待機 (本来ありえないが安全策)
    if(out_moves.empty()) {
        out_moves.push_back({start_r, start_c});
    }
}

// ------------------------------------------------------------------
// 盤面更新 (シミュレーション用)
// ------------------------------------------------------------------
void advance(State& s, const vector<Move>& moves) {
    // 衝突判定用: count[r][c] -> 何人来るか
    static int target_map[MAX_N][MAX_N][MAX_M + 1]; // [r][c] -> list of p_ids
    static int target_count[MAX_N][MAX_N];

    // クリア
    for(int r=0; r<N; ++r) {
        memset(target_count[r], 0, sizeof(int)*N);
    }

    // 移動先登録
    for(int p=0; p<M; ++p) {
        int r = moves[p].r;
        int c = moves[p].c;
        target_map[r][c][target_count[r][c]++] = p;
    }

    // 衝突解決
    for(int r=0; r<N; ++r) {
        for(int c=0; c<N; ++c) {
            int cnt = target_count[r][c];
            if(cnt == 0) continue;

            int winner = -1;
            int owner = s.owner[r][c];
            bool owner_present = false;

            // 所有者が含まれているか確認
            for(int k=0; k<cnt; ++k) {
                int p = target_map[r][c][k];
                if(p == owner) {
                    owner_present = true;
                    winner = p;
                    break;
                }
            }

            // 所有者がいない場合
            if(!owner_present) {
                if(cnt == 1) {
                    winner = target_map[r][c][0];
                } else {
                    winner = -1; // 競合消滅
                }
            }

            // 勝者がいれば更新
            if(winner != -1) {
                s.pos_r[winner] = r;
                s.pos_c[winner] = c;

                if(s.owner[r][c] == -1) {
                    s.owner[r][c] = winner;
                    s.level[r][c] = 1;
                } else if(s.owner[r][c] == winner) {
                    if(s.level[r][c] < U) s.level[r][c]++;
                } else {
                    s.level[r][c]--;
                    if(s.level[r][c] == 0) {
                        s.owner[r][c] = winner;
                        s.level[r][c] = 1;
                    }
                }
            }
        }
    }
}

// ------------------------------------------------------------------
// プレイアウト (Greedy Random)
// ------------------------------------------------------------------
// 価値が高いマスに移動しやすいように重み付け抽選する
Move select_move_weighted(const State& s, int p_id) {
    static vector<Move> cands;
    get_valid_moves(s, p_id, cands);
    
    if(cands.empty()) return {s.pos_r[p_id], s.pos_c[p_id]};
    if(cands.size() == 1) return cands[0];

    // 重み計算
    double weights[100]; // 最大マス数分あれば十分
    double sum_w = 0;
    
    for(size_t i=0; i<cands.size(); ++i) {
        int r = cands[i].r;
        int c = cands[i].c;
        
        // 基本価値
        double w = (double)V[r][c];
        
        // 状況によるボーナス (ヒューリスティック)
        if(s.owner[r][c] == -1) {
            w *= 2.0; // 新規獲得はおいしい
        } else if(s.owner[r][c] == p_id) {
            if(s.level[r][c] < U) w *= 0.5; // 強化はそこそこ
            else w = 0.1; // カンスト済みは行きたくない
        } else {
            // 敵地
            if(s.level[r][c] == 1) w *= 3.0; // 奪えるなら最高
            else w *= 1.2; // 削り
        }
        
        if(w < 1.0) w = 1.0;
        weights[i] = w;
        sum_w += w;
    }

    double r = rng.next_double() * sum_w;
    double curr_sum = 0;
    for(size_t i=0; i<cands.size(); ++i) {
        curr_sum += weights[i];
        if(r <= curr_sum) return cands[i];
    }
    return cands.back();
}

double run_simulation(State s, Move my_move, int depth) {
    static vector<Move> moves(MAX_M);
    moves[0] = my_move;
    
    // 初手: 敵も賢く動くと仮定
    for(int p=1; p<M; ++p) {
        moves[p] = select_move_weighted(s, p);
    }
    advance(s, moves);
    
    // 続き
    for(int d=0; d<depth; ++d) {
        for(int p=0; p<M; ++p) {
            moves[p] = select_move_weighted(s, p);
        }
        advance(s, moves);
    }
    
    return s.get_score_objective();
}

// ------------------------------------------------------------------
// メイン
// ------------------------------------------------------------------
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if(!(cin >> N >> M >> T >> U)) return 0;
    
    for(int i=0; i<N; ++i) for(int j=0; j<N; ++j) cin >> V[i][j];

    State state;
    state.init_empty();

    for(int i=0; i<M; ++i) {
        cin >> state.pos_r[i] >> state.pos_c[i];
        state.owner[state.pos_r[i]][state.pos_c[i]] = i;
        state.level[state.pos_r[i]][state.pos_c[i]] = 1;
    }

    // 全体の制限時間を考慮 (1.95秒をターゲット)
    double TIME_LIMIT_TOTAL = 1.95;

    for(int t=0; t<T; ++t) {
        // --- 時間管理 ---
        double current_time = global_timer.get_sec();
        double remaining_time = TIME_LIMIT_TOTAL - current_time;
        double time_for_turn = remaining_time / (T - t);
        
        // どんなに時間がなくても最低限確保する時間 (ms)
        if(time_for_turn < 0.005) time_for_turn = 0.005; 
        
        double turn_end_time = current_time + time_for_turn;
        
        // --- 思考フェーズ ---
        static vector<Move> candidates;
        get_valid_moves(state, 0, candidates);
        
        Move best_move = {state.pos_r[0], state.pos_c[0]};
        if(!candidates.empty()) best_move = candidates[0];

        // 候補が1つなら即決
        if(candidates.size() > 1) {
            double best_avg_score = -1e18;
            
            // 各候補のスコア記録
            // scores[i] = total_score, counts[i] = num_playouts
            vector<double> sum_scores(candidates.size(), 0);
            vector<int> counts(candidates.size(), 0);
            
            int loop_cnt = 0;
            
            // 時間いっぱい回す
            while(true) {
                // 100回ごとに時間チェック
                if((loop_cnt & 127) == 0) {
                    if(global_timer.get_sec() > turn_end_time) break;
                }
                
                // 候補選択 (UCB1とか使いたいが、計算コストが重いのでRound Robinで十分)
                int idx = loop_cnt % candidates.size();
                
                // 深さは終盤につれて浅くしても良いが、固定で十分強力
                // 序盤は展開重要なので深く、終盤は詰み読みで浅く？
                // ここでは固定
                double score = run_simulation(state, candidates[idx], 12);
                
                sum_scores[idx] += score;
                counts[idx]++;
                loop_cnt++;
            }
            
            // 最良手選択
            for(size_t i=0; i<candidates.size(); ++i) {
                if(counts[i] == 0) continue;
                double avg = sum_scores[i] / counts[i];
                if(avg > best_avg_score) {
                    best_avg_score = avg;
                    best_move = candidates[i];
                }
            }
        }

        // --- 出力 ---
        cout << best_move.r << " " << best_move.c << endl;

        // --- 入力受取 ---
        int dr, dc;
        for(int i=0; i<M; ++i) cin >> dr >> dc; // move output (dummy)

        for(int i=0; i<M; ++i) cin >> state.pos_r[i] >> state.pos_c[i];
        for(int i=0; i<N; ++i) for(int j=0; j<N; ++j) cin >> state.owner[i][j];
        for(int i=0; i<N; ++i) for(int j=0; j<N; ++j) cin >> state.level[i][j];
    }

    return 0;
}