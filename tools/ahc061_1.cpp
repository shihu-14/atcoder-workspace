#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <climits>
#include <cfloat>
#include <cassert>
#include <ctime>
#include <cctype>
#include <cwctype>
#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <utility>
#include <bitset>
#include <vector>
#include <deque>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <tuple>
#include <array>
#include <new>
#include <memory>
#include <limits>
#include <random>
#include <exception>
#include <stdexcept>
#include <regex>
#include <complex>
#include <chrono>
#include <future>
#include <thread>
#include <mutex>
#include <atomic>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using mint = modint;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = int(a)-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;

// ============================================================================
// Logic Implementation
// ============================================================================

const int MAX_N = 20; 
const int MAX_M = 20; 
const double TIME_LIMIT_PER_TURN = 0.025; 

// Global Inputs
int N, M, T, U;
int V_grid[MAX_N][MAX_N];
int start_x[MAX_M], start_y[MAX_M];

struct Point {
    int x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point& other) const { return !(*this == other); }
};

struct Player {
    int id;
    Point pos;
    // Weights: 0:Neutral, 1:Own<U, 2:EnemyL=1, 3:EnemyL>=2
    std::array<double, 4> weights; 

    Player() {
        weights = {1.0, 1.0, 1.0, 1.0};
    }
};

struct State {
    int turn;
    int board_owner[MAX_N][MAX_N];
    int board_level[MAX_N][MAX_N];
    Point player_pos[MAX_M];
    
    int first_move_idx = -1; 
    double eval_score = -1e18;

    long long get_player_score(int pid) const {
        long long score = 0;
        rep(i, N) {
            rep(j, N) {
                if(board_owner[i][j] == pid) {
                    score += (long long)V_grid[i][j] * board_level[i][j];
                }
            }
        }
        return score;
    }

    double calculate_evaluation() const {
        long long s0 = get_player_score(0);
        long long max_ai = 0;
        rep2(i, 1, M) {
            max_ai = max(max_ai, get_player_score(i));
        }
        return (double)s0 / (double)(max_ai + 100); 
    }
    
    bool operator<(const State& other) const {
        return eval_score < other.eval_score;
    }
};

bool is_valid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}

int get_move_type(int pid, int x, int y, const State& s) {
    int owner = s.board_owner[x][y];
    int level = s.board_level[x][y];
    
    if (owner == -1) return 0; 
    if (owner == pid) {
        if (level < U) return 1; 
        else return 2; 
    }
    if (level == 1) return 3;
    return 4;
}

double get_move_value(int pid, int x, int y, const State& s, const std::array<double, 4>& w) {
    int type = get_move_type(pid, x, y, s);
    double val = (double)V_grid[x][y];
    
    if (type == 0) return val * w[0];
    if (type == 1) return val * w[1];
    if (type == 2) return 0.0; 
    if (type == 3) return val * w[2];
    if (type == 4) return val * w[3];
    return 0.0;
}

class Solver {
public:
    Player players[MAX_M];
    State current_state;
    std::mt19937 rng;

    Solver() {
        rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }

    void init() {
        current_state.turn = 0;
        rep(i, N) {
            rep(j, N) {
                current_state.board_owner[i][j] = -1;
                current_state.board_level[i][j] = 0;
            }
        }
        rep(i, M) {
            current_state.player_pos[i] = {start_x[i], start_y[i]};
            players[i].id = i;
            players[i].pos = {start_x[i], start_y[i]};
            
            current_state.board_owner[start_x[i]][start_y[i]] = i;
            current_state.board_level[start_x[i]][start_y[i]] = 1;
        }
    }

    // 学習：盤面更新前に呼ぶこと
    void update_estimates(int turn, const vector<Point>& actual_moves) {
        if (turn <= 0) return;
        
        rep2(p, 1, M) {
            Point move = actual_moves[p];
            // 現在の状態(更新前)を使って、敵が何を選んだか評価する
            int type = get_move_type(p, move.x, move.y, current_state);
            int idx = -1;
            if (type == 0) idx = 0; 
            else if (type == 1) idx = 1; 
            else if (type == 3) idx = 2; 
            else if (type == 4) idx = 3; 
            
            if (idx != -1) {
                players[p].weights[idx] += 0.5;
            }
        }
    }

    State simulate(State s, const vector<Point>& moves) {
        State next_s = s;
        next_s.turn++;

        vector<int> candidates[MAX_N][MAX_N];
        rep(p, M) {
            candidates[moves[p].x][moves[p].y].push_back(p);
        }

        bool eliminated[MAX_M] = {false};

        rep(i, N) {
            rep(j, N) {
                if (candidates[i][j].empty()) continue;

                int owner = s.board_owner[i][j];
                bool owner_present = false;
                for(int pid : candidates[i][j]) {
                    if (pid == owner) {
                        owner_present = true;
                        break;
                    }
                }

                if (owner_present) {
                    for(int pid : candidates[i][j]) {
                        if (pid != owner) eliminated[pid] = true;
                    }
                } else {
                    if (candidates[i][j].size() > 1) {
                        for(int pid : candidates[i][j]) eliminated[pid] = true;
                    }
                }
            }
        }

        rep(p, M) {
            if (eliminated[p]) {
                next_s.player_pos[p] = s.player_pos[p];
            } else {
                Point target = moves[p];
                next_s.player_pos[p] = target;
                
                int owner = next_s.board_owner[target.x][target.y];
                
                if (owner == -1) {
                    next_s.board_owner[target.x][target.y] = p;
                    next_s.board_level[target.x][target.y] = 1;
                } else if (owner == p) {
                    if (next_s.board_level[target.x][target.y] < U) {
                        next_s.board_level[target.x][target.y]++;
                    }
                } else {
                    next_s.board_level[target.x][target.y]--;
                    if (next_s.board_level[target.x][target.y] == 0) {
                        next_s.board_owner[target.x][target.y] = p;
                        next_s.board_level[target.x][target.y] = 1;
                    } else {
                        next_s.player_pos[p] = s.player_pos[p]; 
                    }
                }
            }
        }
        return next_s;
    }

    Point predict_enemy_move(int pid, const State& s) {
        Point best_move = s.player_pos[pid];
        double best_val = -1.0;
        Point curr = s.player_pos[pid];
        
        int dirs[5][2] = {{0,0}, {1,0}, {-1,0}, {0,1}, {0,-1}};
        
        for(auto& d : dirs) {
            int nx = curr.x + d[0];
            int ny = curr.y + d[1];
            if(is_valid(nx, ny)) {
                bool occupied = false;
                rep(other, M) {
                    if (other != pid && s.player_pos[other].x == nx && s.player_pos[other].y == ny) {
                        occupied = true; break;
                    }
                }
                
                if (!occupied) {
                    double val = get_move_value(pid, nx, ny, s, players[pid].weights);
                    if (val > best_val) {
                        best_val = val;
                        best_move = {nx, ny};
                    }
                }
            }
        }
        return best_move;
    }

    Point get_best_move() {
        int beam_width = 30; 
        int depth = 4;
        
        vector<State> beam;
        current_state.first_move_idx = -1; 
        beam.push_back(current_state);

        auto start_time = std::chrono::high_resolution_clock::now();

        for (int d = 0; d < depth; ++d) {
            vector<State> next_beam;
            
            for (const auto& s : beam) {
                Point my_pos = s.player_pos[0];
                int dirs[5][2] = {{0,0}, {1,0}, {-1,0}, {0,1}, {0,-1}};
                
                rep(i, 5) { 
                    int nx = my_pos.x + dirs[i][0];
                    int ny = my_pos.y + dirs[i][1];
                    
                    if(!is_valid(nx, ny)) continue;
                    
                    bool occupied = false;
                    rep2(p, 1, M) {
                        if(s.player_pos[p].x == nx && s.player_pos[p].y == ny) occupied = true;
                    }
                    if(occupied) continue;

                    vector<Point> moves(M);
                    moves[0] = {nx, ny};
                    
                    rep2(p, 1, M) {
                        moves[p] = predict_enemy_move(p, s);
                    }

                    State ns = simulate(s, moves);
                    
                    if (d == 0) ns.first_move_idx = i;
                    else ns.first_move_idx = s.first_move_idx;

                    ns.eval_score = ns.calculate_evaluation();
                    next_beam.push_back(ns);
                }
            }

            sort(next_beam.rbegin(), next_beam.rend()); 
            if (next_beam.size() > beam_width) {
                next_beam.resize(beam_width);
            }
            beam = next_beam;
            
            auto now = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double>(now - start_time).count();
            if (elapsed > TIME_LIMIT_PER_TURN * 0.8) break; 
        }

        if (beam.empty()) return current_state.player_pos[0]; 

        int best_move_idx = beam[0].first_move_idx;
        int dirs[5][2] = {{0,0}, {1,0}, {-1,0}, {0,1}, {0,-1}};
        
        return {current_state.player_pos[0].x + dirs[best_move_idx][0], 
                current_state.player_pos[0].y + dirs[best_move_idx][1]};
    }

    void update_state(int t, const vector<Point>& moves, const vector<Point>& end_pos, 
                      const vector<vector<int>>& owners, const vector<vector<int>>& levels) {
        
        // 1. まずパラメータ推定を更新 (現在の盤面情報を使うため、更新前に行う)
        update_estimates(t, moves);

        // 2. 盤面情報を新しいものに更新
        current_state.turn = t;
        rep(p, M) current_state.player_pos[p] = end_pos[p];
        rep(i, N) {
            rep(j, N) {
                current_state.board_owner[i][j] = owners[i][j];
                current_state.board_level[i][j] = levels[i][j];
            }
        }
    }
};

void solve()
{
    // 入力読み込み
    if (!(cin >> N >> M >> T >> U)) return;
    
    rep(i, N) {
        rep(j, N) {
            cin >> V_grid[i][j];
        }
    }
    rep(i, M) {
        cin >> start_x[i] >> start_y[i];
    }

    Solver solver;
    solver.init();

    rep2(t, 1, T + 1) {
        Point my_move = solver.get_best_move();
        cout << my_move.x << " " << my_move.y << endl;

        vector<Point> intended_moves(M);
        vector<Point> end_positions(M);
        vector<vector<int>> owners(N, vector<int>(N));
        vector<vector<int>> levels(N, vector<int>(N));

        // 入力エラーチェック: 読み込み失敗時はループを抜けて終了
        bool ok = true;
        rep(i, M) if (!(cin >> intended_moves[i].x >> intended_moves[i].y)) ok = false;
        rep(i, M) if (!(cin >> end_positions[i].x >> end_positions[i].y)) ok = false;
        rep(i, N) rep(j, N) if (!(cin >> owners[i][j])) ok = false;
        rep(i, N) rep(j, N) if (!(cin >> levels[i][j])) ok = false;

        if (!ok) break;

        solver.update_state(t, intended_moves, end_positions, owners, levels);
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    while(t--)
    {
        solve();
    }
    return 0;
}