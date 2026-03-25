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

// --- Global Constants & Input Data ---
int N, M, T_MAX, U;
int V[10][10];

// --- Hyperparameters (To be Tuned) ---
const double PARAM_K = 1.8;           // Penalty weight for Top AI score (1.0 ~ 2.0)
const double PARAM_CONN_BONUS = 100.0; // Bonus per cell in the connected component

// --- Structures ---
struct Point {
    int8_t x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point& other) const { return x != other.x || y != other.y; }
};

struct State {
    int8_t owner[10][10]; // -1: Neutral, 0-(M-1): Player ID
    int8_t level[10][10];
    Point positions[8];
    int scores[8]; // Current score cache (sum of V*L)
    
    // For search reconstruction
    int first_move_idx; // Encoded move (x*10 + y) taken at root
    double eval_score;  // Heuristic value

    bool operator<(const State& other) const {
        return eval_score < other.eval_score; // Priority Queue pops largest
    }
};

// --- Time Management ---
struct Timer {
    chrono::high_resolution_clock::time_point start;
    double limit_sec;
    Timer(double l) : start(chrono::high_resolution_clock::now()), limit_sec(l) {}
    bool done() {
        auto now = chrono::high_resolution_clock::now();
        double d = chrono::duration<double>(now - start).count();
        return d >= limit_sec;
    }
};

// --- Random ---
uint64_t xorshift64() {
    static uint64_t x = 88172645463325252ull;
    x ^= x << 13; x ^= x >> 7; x ^= x << 17;
    return x;
}

// --- Logic ---

// Check if coordinate is valid
inline bool inside(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}

// Get valid moves for a specific player
// Rule: Reachable from current position via own territory + adjacent cells.
vector<Point> get_valid_moves(const State& s, int p_id) {
    vector<Point> moves;
    
    // Check if another player is there helper
    auto is_occupied = [&](int x, int y) {
        rep(i, M) {
            if (i != p_id && s.positions[i].x == x && s.positions[i].y == y) return true;
        }
        return false;
    };

    // BFS to find connected component of own territory
    // The rule allows moving to any cell in the "Reachable Territory" OR adjacent to it.
    
    bool visited_comp[10][10] = {};
    queue<Point> q_comp;
    
    Point start = s.positions[p_id];
    q_comp.push(start);
    visited_comp[start.x][start.y] = true;
    
    // Collect all reachable territory cells
    // Even if current pos is not owned (e.g. just invaded), logic implies we can move from where we are.
    // Standard interpretation: You are always ON your territory effectively for movement base,
    // or if you are on enemy territory (not possible by rules? "always on own territory"),
    // let's assume standard connectivity BFS on owner==p_id.
    
    vector<Point> reachable;
    reachable.push_back(start); // Can always stay or move from here

    while(!q_comp.empty()){
        Point curr = q_comp.front();
        q_comp.pop();

        rep(d, 4) {
            int nx = curr.x + dx[d];
            int ny = curr.y + dy[d];
            if(inside(nx, ny) && !visited_comp[nx][ny]) {
                if(s.owner[nx][ny] == p_id) {
                    visited_comp[nx][ny] = true;
                    q_comp.push({(int8_t)nx, (int8_t)ny});
                    reachable.push_back({(int8_t)nx, (int8_t)ny});
                }
            }
        }
    }

    // Generate candidates
    bool added_candidate[10][10] = {};
    
    auto add = [&](int x, int y) {
        if (!inside(x, y)) return;
        if (added_candidate[x][y]) return;
        if (is_occupied(x, y)) return; // Cannot move to occupied cell

        added_candidate[x][y] = true;
        moves.push_back({(int8_t)x, (int8_t)y});
    };

    for(const auto& p : reachable) {
        add(p.x, p.y); // Can move to reachable territory itself
        rep(d, 4) {
            add(p.x + dx[d], p.y + dy[d]); // Can move to neighbor
        }
    }
    
    return moves;
}

// Evaluation function for AI prediction
double get_ai_eval(const State& s, int p_id, int x, int y) {
    if (!inside(x, y)) return -1e18;
    
    // Check occupied
    rep(i, M) {
        if (i != p_id && s.positions[i].x == x && s.positions[i].y == y) return -1e18;
    }

    int owner = s.owner[x][y];
    int level = s.level[x][y];
    int val = V[x][y];
    
    // Simplified greedy weights (all 1.0)
    if (owner == -1) return val;
    else if (owner == p_id) return (level < U) ? val : 0.0;
    else return val; // Attack (level 1 or >1 both valued)
}

// Simulate one turn
void simulate(State& s, Point my_move) {
    Point next_pos[8];
    next_pos[0] = my_move;

    // AI moves (Greedy prediction)
    rep2(p, 1, M) {
        Point curr = s.positions[p];
        Point best_p = curr;
        double best_val = -1e18;

        // Optimization: Check 4 neighbors + stay only
        auto check = [&](int x, int y) {
            double v = get_ai_eval(s, p, x, y);
            if (v > best_val) {
                best_val = v;
                best_p = {(int8_t)x, (int8_t)y};
            }
        };

        check(curr.x, curr.y);
        rep(d, 4) check(curr.x + dx[d], curr.y + dy[d]);
        
        next_pos[p] = best_p;
    }

    // Conflict Resolution
    vector<int> at_cell[10][10];
    rep(p, M) at_cell[next_pos[p].x][next_pos[p].y].push_back(p);

    bool alive[8] = {};
    rep(x, N) rep(y, N) {
        if (at_cell[x][y].empty()) continue;
        if (at_cell[x][y].size() == 1) {
            alive[at_cell[x][y][0]] = true;
        } else {
            // Collision
            int owner = s.owner[x][y];
            bool owner_survives = false;
            for(int p : at_cell[x][y]) {
                if(p == owner) {
                    owner_survives = true;
                    break;
                }
            }
            if(owner_survives && owner != -1) alive[owner] = true;
        }
    }

    // Reset dead
    rep(p, M) if(!alive[p]) next_pos[p] = s.positions[p];

    // Territory Update
    rep(p, M) {
        if (alive[p]) {
            int x = next_pos[p].x;
            int y = next_pos[p].y;
            int owner = s.owner[x][y];
            
            if (owner == -1) {
                s.owner[x][y] = p;
                s.level[x][y] = 1;
            } else if (owner == p) {
                if (s.level[x][y] < U) s.level[x][y]++;
            } else {
                s.level[x][y]--;
                if (s.level[x][y] == 0) {
                    s.owner[x][y] = p;
                    s.level[x][y] = 1;
                } else {
                    // Attack failed to capture -> Attacker repelled
                    alive[p] = false;
                    next_pos[p] = s.positions[p];
                }
            }
        }
    }

    rep(p, M) s.positions[p] = next_pos[p];

    // Score Update
    fill(s.scores, s.scores + M, 0);
    rep(i, N) rep(j, N) {
        if (s.owner[i][j] != -1) {
            s.scores[s.owner[i][j]] += V[i][j] * s.level[i][j];
        }
    }
}

// Calculate Connected Component Size for Player 0
int get_connected_size(const State& s) {
    bool visited[10][10] = {};
    queue<pii> q;
    
    Point start = s.positions[0];
    // If somehow not on own territory (should not happen by rules, but for safety)
    if (s.owner[start.x][start.y] != 0) return 0;

    q.push({start.x, start.y});
    visited[start.x][start.y] = true;
    
    int cnt = 0;
    while(!q.empty()){
        auto [cx, cy] = q.front();
        q.pop();
        cnt++;

        rep(d, 4) {
            int nx = cx + dx[d];
            int ny = cy + dy[d];
            if(inside(nx, ny) && !visited[nx][ny] && s.owner[nx][ny] == 0) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }
    return cnt;
}

// Score Evaluation for Chokudai Search
void evaluate_state(State& s) {
    int my_score = s.scores[0];
    int max_ai_score = 0;
    rep2(i, 1, M) chmax(max_ai_score, s.scores[i]);
    
    int conn_size = get_connected_size(s);

    // Modified Evaluation Function
    // Eval = (MyScore - K * TopAI) + Bonus * ConnectedSize
    s.eval_score = (double)my_score - PARAM_K * max_ai_score + PARAM_CONN_BONUS * conn_size;
}

// Main Search Function
Point chokudai_search(const State& root_state, int turn) {
    Timer timer(0.018); // Slightly increased margin
    int max_depth = min(10, T_MAX - turn);
    if (max_depth <= 0) return root_state.positions[0];

    vector<priority_queue<State>> beams(max_depth + 1);
    beams[0].push(root_state);

    Point best_move = root_state.positions[0];
    double best_eval = -1e18;

    while (!timer.done()) {
        rep(d, max_depth) {
            if (beams[d].empty()) continue;
            
            State curr = beams[d].top();
            beams[d].pop();
            
            // Move Generation
            vector<Point> moves;
            if (d == 0) {
                moves = get_valid_moves(curr, 0);
            } else {
                // Simplified for depth > 0
                Point p = curr.positions[0];
                moves.push_back(p);
                rep(dir, 4) {
                    int nx = p.x + dx[dir];
                    int ny = p.y + dy[dir];
                    if (inside(nx, ny)) moves.push_back({(int8_t)nx, (int8_t)ny});
                }
            }

            if (moves.empty()) moves.push_back(curr.positions[0]);

            int branch_limit = (d == 0) ? moves.size() : 2;
            
            rep(i, moves.size()) {
                if (timer.done()) break;
                if (d > 0 && i >= branch_limit) break;

                Point mv = moves[i];
                State next_s = curr;
                if (d == 0) next_s.first_move_idx = mv.x * 10 + mv.y;

                simulate(next_s, mv);
                evaluate_state(next_s); // Uses new eval

                if (next_s.eval_score > best_eval) {
                    best_eval = next_s.eval_score;
                    if (d == 0) best_move = mv;
                    else {
                        int fx = next_s.first_move_idx / 10;
                        int fy = next_s.first_move_idx % 10;
                        best_move = {(int8_t)fx, (int8_t)fy};
                    }
                }

                beams[d+1].push(next_s);
            }
            if (timer.done()) break;
        }
    }
    return best_move;
}

void solve()
{
    if (!(cin >> N >> M >> T_MAX >> U)) return;
    rep(i, N) rep(j, N) cin >> V[i][j];
    
    State current_state;
    rep(i, N) rep(j, N) {
        current_state.owner[i][j] = -1;
        current_state.level[i][j] = 0;
    }
    
    rep(p, M) {
        int sx, sy; cin >> sx >> sy;
        current_state.positions[p] = {(int8_t)sx, (int8_t)sy};
        current_state.owner[sx][sy] = p;
        current_state.level[sx][sy] = 1;
    }
    
    fill(current_state.scores, current_state.scores + M, 0);
    rep(i, N) rep(j, N) {
        if(current_state.owner[i][j] != -1)
            current_state.scores[current_state.owner[i][j]] += V[i][j];
    }

    for (int t = 1; t <= T_MAX; ++t) {
        Point my_move = chokudai_search(current_state, t);
        cout << (int)my_move.x << " " << (int)my_move.y << endl;

        rep(p, M) { int tx, ty; cin >> tx >> ty; }
        rep(p, M) {
            int ex, ey; cin >> ex >> ey;
            current_state.positions[p] = {(int8_t)ex, (int8_t)ey};
        }
        rep(i, N) rep(j, N) { int o; cin >> o; current_state.owner[i][j] = o; }
        rep(i, N) rep(j, N) { int l; cin >> l; current_state.level[i][j] = l; }

        fill(current_state.scores, current_state.scores + M, 0);
        rep(i, N) rep(j, N) {
            if(current_state.owner[i][j] != -1)
                current_state.scores[current_state.owner[i][j]] += V[i][j] * current_state.level[i][j];
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}