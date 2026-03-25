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
        if (n <= 0) return 0;
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
int V[MAX_N][MAX_N]; 

struct Timer {
    chrono::high_resolution_clock::time_point start;
    Timer() { reset(); }
    void reset() { start = chrono::high_resolution_clock::now(); }
    double get_sec() {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now - start).count() / 1000.0;
    }
} global_timer;

// ------------------------------------------------------------------
// 盤面状態
// ------------------------------------------------------------------
struct State {
    int pos_r[MAX_M];
    int pos_c[MAX_M];
    int owner[MAX_N][MAX_N];
    int level[MAX_N][MAX_N];

    void init_empty() {
        for(int i=0; i<N; ++i) {
            for(int j=0; j<N; ++j) {
                owner[i][j] = -1;
                level[i][j] = 0;
            }
        }
    }

    double get_score_objective() const {
        long long scores[MAX_M] = {0};
        int territory_count[MAX_M] = {0};
        
        for(int r=0; r<N; ++r) {
            for(int c=0; c<N; ++c) {
                int o = owner[r][c];
                if(o != -1) {
                    scores[o] += (long long)V[r][c] * level[r][c];
                    territory_count[o]++;
                }
            }
        }
        
        long long s0 = scores[0];
        long long max_other = 1;
        for(int i=1; i<M; ++i) {
            if(scores[i] > max_other) max_other = scores[i];
        }
        
        // 基本の対数スコア比率
        double score = log(s0 + 1.0) - log(max_other + 1.0);
        // 微小な領土支配率ボーナス（同スコア時のタイブレーク）
        score += territory_count[0] * 0.0001;
        
        return score;
    }
};

struct Move {
    int r, c;
};

int visited_token[MAX_N][MAX_N];
int current_token = 0;

void get_valid_moves(const State& s, int p_id, vector<Move>& out_moves) {
    out_moves.clear();
    current_token++;
    int start_r = s.pos_r[p_id];
    int start_c = s.pos_c[p_id];
    
    if(start_r < 0 || start_r >= N || start_c < 0 || start_c >= N) {
        out_moves.push_back({0, 0});
        return;
    }

    queue<pair<int, int>> q;
    q.push({start_r, start_c});
    visited_token[start_r][start_c] = current_token;
    
    bool is_occupied[MAX_N][MAX_N] = {false};
    for(int i=0; i<M; ++i) {
        if(i == p_id) continue;
        int er = s.pos_r[i];
        int ec = s.pos_c[i];
        if(er >= 0 && er < N && ec >= 0 && ec < N) is_occupied[er][ec] = true;
    }

    int dr[] = {0, 0, 1, -1};
    int dc[] = {1, -1, 0, 0};
    int territory_token = current_token;
    static vector<pair<int,int>> reachable_territory;
    reachable_territory.clear();
    reachable_territory.push_back({start_r, start_c});
    
    while(!q.empty()){
        auto curr = q.front(); q.pop();
        for(int i=0; i<4; ++i){
            int nr = curr.first + dr[i]; int nc = curr.second + dc[i];
            if(nr >= 0 && nr < N && nc >= 0 && nc < N) {
                if(visited_token[nr][nc] != territory_token && s.owner[nr][nc] == p_id) {
                    visited_token[nr][nc] = territory_token;
                    reachable_territory.push_back({nr, nc});
                    q.push({nr, nc});
                }
            }
        }
    }
    
    static int added_token[MAX_N][MAX_N];
    static int current_added_token = 0;
    current_added_token++;
    for(auto& t : reachable_territory) {
        if(added_token[t.first][t.second] != current_added_token && !is_occupied[t.first][t.second]) {
            added_token[t.first][t.second] = current_added_token;
            out_moves.push_back({t.first, t.second});
        }
        for(int i=0; i<4; ++i) {
            int nr = t.first + dr[i]; int nc = t.second + dc[i];
            if(nr >= 0 && nr < N && nc >= 0 && nc < N) {
                if(added_token[nr][nc] != current_added_token && !is_occupied[nr][nc]) {
                    added_token[nr][nc] = current_added_token;
                    out_moves.push_back({nr, nc});
                }
            }
        }
    }
    if(out_moves.empty()) out_moves.push_back({start_r, start_c});
}

void advance(State& s, const vector<Move>& moves) {
    static int target_map[MAX_N][MAX_N][MAX_M + 1]; 
    static int target_count[MAX_N][MAX_N];
    for(int r=0; r<N; ++r) memset(target_count[r], 0, sizeof(int)*N);
    for(int p=0; p<M; ++p) {
        int r = moves[p].r; int c = moves[p].c;
        target_map[r][c][target_count[r][c]++] = p;
    }
    for(int r=0; r<N; ++r) {
        for(int c=0; c<N; ++c) {
            int cnt = target_count[r][c];
            if(cnt == 0) continue;
            int winner = -1; int owner = s.owner[r][c]; bool owner_present = false;
            for(int k=0; k<cnt; ++k) {
                int p = target_map[r][c][k];
                if(p == owner) { owner_present = true; winner = p; break; }
            }
            if(!owner_present) {
                if(cnt == 1) winner = target_map[r][c][0];
                else winner = -1;
            }
            if(winner != -1) {
                s.pos_r[winner] = r; s.pos_c[winner] = c;
                if(s.owner[r][c] == -1) { s.owner[r][c] = winner; s.level[r][c] = 1; }
                else if(s.owner[r][c] == winner) { if(s.level[r][c] < U) s.level[r][c]++; }
                else {
                    s.level[r][c]--;
                    if(s.level[r][c] == 0) { s.owner[r][c] = winner; s.level[r][c] = 1; }
                }
            }
        }
    }
}

// 改良：1手読み（即死回避・即時占領）を含む重み付け選択
Move select_move_best(const State& s, int p_id) {
    static vector<Move> cands;
    get_valid_moves(s, p_id, cands);
    if(cands.size() <= 1) return cands.empty() ? Move{s.pos_r[p_id], s.pos_c[p_id]} : cands[0];

    double weights[100]; double sum_w = 0;
    int dr[] = {0, 0, 1, -1}; int dc[] = {1, -1, 0, 0};

    for(size_t i=0; i<cands.size(); ++i) {
        int r = cands[i].r; int c = cands[i].c;
        double w = (double)V[r][c];
        
        if(s.owner[r][c] == -1) w *= 3.0; // 拡張優先
        else if(s.owner[r][c] == p_id) {
            bool threatened = false;
            for(int k=0; k<4; ++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if(nr>=0 && nr<N && nc>=0 && nc<N && s.owner[nr][nc] != -1 && s.owner[nr][nc] != p_id) {
                    threatened = true; break;
                }
            }
            if(threatened) w *= (s.level[r][c] < U ? 8.0 : 0.5); // 防衛最優先
            else w *= (s.level[r][c] < U ? 0.4 : 0.01);
        } else {
            if(s.level[r][c] == 1) w *= 10.0; // 即時占領を極めて高く評価
            else w *= 1.5;
        }
        weights[i] = max(0.01, w);
        sum_w += weights[i];
    }
    double rv = rng.next_double() * sum_w;
    double cur = 0;
    for(size_t i=0; i<cands.size(); ++i) { cur += weights[i]; if(rv <= cur) return cands[i]; }
    return cands.back();
}

double run_simulation(State s, Move my_move, int max_depth) {
    static vector<Move> moves(MAX_M);
    moves[0] = my_move;
    for(int p=1; p<M; ++p) moves[p] = select_move_best(s, p);
    advance(s, moves);
    
    for(int d=0; d<max_depth; ++d) {
        for(int p=0; p<M; ++p) moves[p] = select_move_best(s, p);
        advance(s, moves);
    }
    return s.get_score_objective();
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    if(!(cin >> N >> M >> T >> U)) return 0;
    for(int i=0; i<N; ++i) for(int j=0; j<N; ++j) cin >> V[i][j];
    State state; state.init_empty();
    for(int i=0; i<M; ++i) {
        cin >> state.pos_r[i] >> state.pos_c[i];
        state.owner[state.pos_r[i]][state.pos_c[i]] = i; state.level[state.pos_r[i]][state.pos_c[i]] = 1;
    }

    const double TIME_LIMIT = 1.94;
    for(int t=0; t<T; ++t) {
        double cur_time = global_timer.get_sec();
        double turn_time = (TIME_LIMIT - cur_time) / (T - t);
        double end_time = cur_time + max(0.005, turn_time);
        
        static vector<Move> candidates;
        get_valid_moves(state, 0, candidates);
        Move best_move = {state.pos_r[0], state.pos_c[0]};
        if(!candidates.empty()) best_move = candidates[0];

        if(candidates.size() > 1) {
            vector<double> sum_scores(candidates.size(), 0);
            vector<int> counts(candidates.size(), 0);
            const double C = 0.4;
            int total_p = 0;
            // 深さの動的調整
            int depth = min(12, T - t - 1);

            for(size_t i=0; i<candidates.size(); ++i) {
                sum_scores[i] += run_simulation(state, candidates[i], depth);
                counts[i]++; total_p++;
            }
            
            int loop = 0;
            while(true) {
                if((loop & 63) == 0 && global_timer.get_sec() > end_time) break;
                int best_idx = 0; double best_ucb = -1e18;
                double log_total = log(total_p);
                for(size_t i=0; i<candidates.size(); ++i) {
                    double ucb = (sum_scores[i]/counts[i]) + C * sqrt(log_total/counts[i]);
                    if(ucb > best_ucb) { best_ucb = ucb; best_idx = i; }
                }
                sum_scores[best_idx] += run_simulation(state, candidates[best_idx], depth);
                counts[best_idx]++; total_p++; loop++;
            }
            
            double best_avg = -1e18;
            for(size_t i=0; i<candidates.size(); ++i) {
                if(counts[i] > 0 && sum_scores[i]/counts[i] > best_avg) {
                    best_avg = sum_scores[i]/counts[i]; best_move = candidates[i];
                }
            }
        }
        cout << best_move.r << " " << best_move.c << endl;
        int dr, dc; for(int i=0; i<M; ++i) cin >> dr >> dc;
        for(int i=0; i<M; ++i) cin >> state.pos_r[i] >> state.pos_c[i];
        for(int i=0; i<N; ++i) for(int j=0; j<N; ++j) cin >> state.owner[i][j];
        for(int i=0; i<N; ++i) for(int j=0; j<N; ++j) cin >> state.level[i][j];
    }
    return 0;
}