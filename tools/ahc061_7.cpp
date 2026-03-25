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
        // +1.0 はゼロ除算防止
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
void get_valid_moves(const State& s, int p_id, vector<Move>& out_moves) {
    out_moves.clear();
    current_token++; 
    
    int start_r = s.pos_r[p_id];
    int start_c = s.pos_c[p_id];
    
    // フェイルセーフ
    if(start_r < 0 || start_r >= N || start_c < 0 || start_c >= N) {
        out_moves.push_back({0, 0});
        return;
    }

    // BFSキュー
    queue<pair<int, int>> q;
    q.push({start_r, start_c});
    visited_token[start_r][start_c] = current_token;
    
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

    int territory_token = current_token; 
    
    // 到達可能領土リスト
    static vector<pair<int,int>> reachable_territory;
    reachable_territory.clear();
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
    
    // 到達可能領土自体も移動先候補
    for(auto& t : reachable_territory) {
        if(added_token[t.first][t.second] != current_added_token && !is_occupied[t.first][t.second]) {
            added_token[t.first][t.second] = current_added_token;
            out_moves.push_back({t.first, t.second});
        }
        
        // その隣接マス
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
    
    if(out_moves.empty()) {
        out_moves.push_back({start_r, start_c});
    }
}

// ------------------------------------------------------------------
// 盤面更新 (シミュレーション用)
// ------------------------------------------------------------------
void advance(State& s, const vector<Move>& moves) {
    static int target_map[MAX_N][MAX_N][MAX_M + 1]; 
    static int target_count[MAX_N][MAX_N];

    for(int r=0; r<N; ++r) {
        memset(target_count[r], 0, sizeof(int)*N);
    }

    for(int p=0; p<M; ++p) {
        int r = moves[p].r;
        int c = moves[p].c;
        target_map[r][c][target_count[r][c]++] = p;
    }

    for(int r=0; r<N; ++r) {
        for(int c=0; c<N; ++c) {
            int cnt = target_count[r][c];
            if(cnt == 0) continue;

            int winner = -1;
            int owner = s.owner[r][c];
            bool owner_present = false;

            for(int k=0; k<cnt; ++k) {
                int p = target_map[r][c][k];
                if(p == owner) {
                    owner_present = true;
                    winner = p;
                    break;
                }
            }

            if(!owner_present) {
                if(cnt == 1) winner = target_map[r][c][0];
                else winner = -1; 
            }

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
Move select_move_weighted(const State& s, int p_id) {
    static vector<Move> cands;
    get_valid_moves(s, p_id, cands);
    
    if(cands.empty()) return {s.pos_r[p_id], s.pos_c[p_id]};
    if(cands.size() == 1) return cands[0];

    // 重み計算
    double weights[100]; 
    double sum_w = 0;
    
    int dr[] = {0, 0, 1, -1};
    int dc[] = {1, -1, 0, 0};

    for(size_t i=0; i<cands.size(); ++i) {
        int r = cands[i].r;
        int c = cands[i].c;
        double w = (double)V[r][c];
        
        if(s.owner[r][c] == -1) {
            w *= 2.0; 
        } else if(s.owner[r][c] == p_id) {
            bool is_threatened = false;
            for(int k=0; k<4; ++k) {
                int nr = r + dr[k];
                int nc = c + dc[k];
                if(nr>=0 && nr<N && nc>=0 && nc<N) {
                    if(s.owner[nr][nc] != -1 && s.owner[nr][nc] != p_id) {
                        is_threatened = true;
                        break;
                    }
                }
            }
            if(is_threatened) {
                if(s.level[r][c] < U) w *= 5.0; 
                else w *= 0.5;
            } else {
                if(s.level[r][c] < U) w *= 0.5;
                else w *= 0.05;
            }
        } else {
            if(s.level[r][c] == 1) w *= 5.0; 
            else w *= 1.2; 
        }
        
        if(w < 0.1) w = 0.1; 
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
    
    for(int p=1; p<M; ++p) moves[p] = select_move_weighted(s, p);
    advance(s, moves);
    
    for(int d=0; d<depth; ++d) {
        for(int p=0; p<M; ++p) moves[p] = select_move_weighted(s, p);
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

    double TIME_LIMIT_TOTAL = 1.95;

    for(int t=0; t<T; ++t) {
        double current_time = global_timer.get_sec();
        double remaining_time = TIME_LIMIT_TOTAL - current_time;
        double time_for_turn = remaining_time / (T - t);
        if(time_for_turn < 0.005) time_for_turn = 0.005; 
        double turn_end_time = current_time + time_for_turn;
        
        static vector<Move> candidates;
        get_valid_moves(state, 0, candidates);
        
        Move best_move = {state.pos_r[0], state.pos_c[0]};
        if(!candidates.empty()) best_move = candidates[0];

        if(candidates.size() > 1) {
            double best_avg_score = -1e18;
            vector<double> sum_scores(candidates.size(), 0);
            vector<int> counts(candidates.size(), 0);
            
            // UCB1定数 C (探索と活用のバランス。理論値はsqrt(2)だが調整可能)
            // スコアがlog比率で小さい値なので、Cも小さめに
            const double C = 0.5;
            
            int total_playouts = 0;
            
            // 全候補を最低1回は回す
            for(size_t i=0; i<candidates.size(); ++i) {
                double score = run_simulation(state, candidates[i], 12);
                sum_scores[i] += score;
                counts[i]++;
                total_playouts++;
            }
            
            int loop_cnt = 0;
            while(true) {
                if((loop_cnt & 127) == 0) {
                    if(global_timer.get_sec() > turn_end_time) break;
                }
                
                // UCB1で次に試す手を選ぶ
                int best_idx = -1;
                double best_ucb = -1e18;
                double log_total = log(total_playouts);
                
                for(size_t i=0; i<candidates.size(); ++i) {
                    if(counts[i] == 0) { // 未試行（通常ここには来ない）
                        best_idx = i;
                        break;
                    }
                    // 平均スコア (exploitation)
                    double avg = sum_scores[i] / counts[i];
                    // 探索項 (exploration)
                    double explore = C * sqrt(log_total / counts[i]);
                    double ucb = avg + explore;
                    
                    if(ucb > best_ucb) {
                        best_ucb = ucb;
                        best_idx = i;
                    }
                }
                
                if(best_idx != -1) {
                    double score = run_simulation(state, candidates[best_idx], 12);
                    sum_scores[best_idx] += score;
                    counts[best_idx]++;
                    total_playouts++;
                }
                loop_cnt++;
            }
            
            // 最終決定は「最も多く試行された手」または「平均スコアが最高の手」
            // 堅実なのは平均スコア
            for(size_t i=0; i<candidates.size(); ++i) {
                if(counts[i] == 0) continue;
                double avg = sum_scores[i] / counts[i];
                if(avg > best_avg_score) {
                    best_avg_score = avg;
                    best_move = candidates[i];
                }
            }
        }

        cout << best_move.r << " " << best_move.c << endl;

        int dr, dc;
        for(int i=0; i<M; ++i) cin >> dr >> dc; 

        for(int i=0; i<M; ++i) cin >> state.pos_r[i] >> state.pos_c[i];
        for(int i=0; i<N; ++i) for(int j=0; j<N; ++j) cin >> state.owner[i][j];
        for(int i=0; i<N; ++i) for(int j=0; j<N; ++j) cin >> state.level[i][j];
    }
    return 0;
}