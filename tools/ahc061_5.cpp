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
// AND target must NOT contain another player's piece currently.
vector<Point> get_valid_moves(const State& s, int p_id) {
    vector<Point> moves;
    bool visited[10][10] = {};
    queue<Point> q;
    
    Point start = s.positions[p_id];
    q.push(start);
    visited[start.x][start.y] = true;

    // BFS to find connected component of own territory
    // Note: The rule says "set of squares reachable... via adjacent own territory".
    // The move destination is "in reachable territory OR adjacent to it".
    
    // First, collect all reachable "Own Territory" cells
    vector<Point> reachable_territory;
    
    // Re-initialize queue for connected component search
    // Using a separate queue to distinguish "my territory path" vs "final adjacent step"
    queue<Point> q_comp;
    bool visited_comp[10][10] = {};
    
    // If current position is on own territory, we can traverse.
    // If current position is NOT own territory (e.g. captured or neutral), 
    // reachable territory is just the current cell (if we interpret rule strictly, 
    // usually you are always on your territory, but enemies can attack under you.
    // Problem says: "Player's piece is always on own territory... territory never becomes 0".
    // So we can always traverse.
    
    q_comp.push(start);
    visited_comp[start.x][start.y] = true;
    reachable_territory.push_back(start);

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
                    reachable_territory.push_back({(int8_t)nx, (int8_t)ny});
                }
            }
        }
    }

    // Now, candidates are all cells in reachable_territory AND their neighbors
    bool added_candidate[10][10] = {};
    
    auto add_if_valid = [&](int x, int y) {
        if (!inside(x, y)) return;
        if (added_candidate[x][y]) return;
        
        // Check if another player is there
        bool occupied = false;
        rep(i, M) {
            if (i != p_id && s.positions[i].x == x && s.positions[i].y == y) {
                occupied = true;
                break;
            }
        }
        if (occupied) return;

        added_candidate[x][y] = true;
        moves.push_back({(int8_t)x, (int8_t)y});
    };

    for(const auto& p : reachable_territory) {
        add_if_valid(p.x, p.y); // Can move to own territory
        rep(d, 4) {
            add_if_valid(p.x + dx[d], p.y + dy[d]); // Can move to neighbor
        }
    }
    
    return moves;
}

// Evaluation function for AI prediction (based on problem description)
double get_ai_eval(const State& s, int p_id, int x, int y, double wa, double wb, double wc, double wd) {
    if (!inside(x, y)) return -1e18;
    
    // Check if occupied by others (Hard constraint)
    rep(i, M) {
        if (i != p_id && s.positions[i].x == x && s.positions[i].y == y) return -1e18;
    }

    int owner = s.owner[x][y];
    int level = s.level[x][y];
    int val = V[x][y];
    
    if (owner == -1) {
        return val * wa;
    } else if (owner == p_id) {
        if (level < U) return val * wb;
        else return 0.0;
    } else {
        if (level == 1) return val * wc;
        else return val * wd;
    }
}

// Simulate one turn
// Returns true if simulation successful
void simulate(State& s, Point my_move) {
    Point next_pos[8];
    
    // 1. Determine moves
    // My move
    next_pos[0] = my_move;

    // AI moves (Greedy prediction)
    // Assuming weights are all 1.0 for simplicity/robustness
    double wa=1.0, wb=1.0, wc=1.0, wd=1.0;
    
    rep2(p, 1, M) {
        // Optimization: Instead of full BFS for AI, just check 4 neighbors + stay
        // This is an approximation to save time. 
        // Full BFS for 7 enemies * 100 cells is too slow inside search.
        // AIs mostly expand or defend locally.
        
        vector<Point> candidates;
        Point curr = s.positions[p];
        candidates.push_back(curr);
        rep(d, 4) {
            int nx = curr.x + dx[d];
            int ny = curr.y + dy[d];
            if(inside(nx, ny)) candidates.push_back({(int8_t)nx, (int8_t)ny});
        }
        
        // Also add logic: if current pos is my territory, can move to any adjacent own territory
        // (Simplified "Reachable" logic for AI)

        Point best_p = curr;
        double best_val = -1e18;

        // Try to stay or move adjacent (simplified)
        // If the AI is stuck or warping, this approximation fails, but it's MVP.
        for(auto& dest : candidates) {
            double val = get_ai_eval(s, p, dest.x, dest.y, wa, wb, wc, wd);
            if (val > best_val) {
                best_val = val;
                best_p = dest;
            }
        }
        next_pos[p] = best_p;
    }

    // 2. Conflict Resolution
    vector<int> at_cell[10][10];
    rep(p, M) {
        at_cell[next_pos[p].x][next_pos[p].y].push_back(p);
    }

    // Determine who stays
    bool alive[8];
    fill(alive, alive + M, false);

    rep(x, N) rep(y, N) {
        if (at_cell[x][y].empty()) continue;
        
        if (at_cell[x][y].size() == 1) {
            alive[at_cell[x][y][0]] = true;
        } else {
            // Collision
            int owner = s.owner[x][y];
            bool owner_present = false;
            for(int p : at_cell[x][y]) {
                if (p == owner) {
                    owner_present = true;
                    break;
                }
            }
            
            if (owner_present && owner != -1) {
                alive[owner] = true; // Only owner survives
            } else {
                // All crash
            }
        }
    }

    // 3. Update Territory
    rep(p, M) {
        if (!alive[p]) {
            // Returned to original position (start of turn)
            // Does not affect map, just position reset
            // Position in 's' is already the start position
            next_pos[p] = s.positions[p]; 
        }
    }

    // Apply effects for those who successfully moved/stayed (and weren't repelled)
    // Note: The rule says "For pieces not collected (alive)... apply capture/strengthen/attack"
    rep(p, M) {
        if (alive[p]) {
            int x = next_pos[p].x;
            int y = next_pos[p].y;
            int owner = s.owner[x][y];
            
            if (owner == -1) {
                // Occupy
                s.owner[x][y] = p;
                s.level[x][y] = 1;
            } else if (owner == p) {
                // Strengthen
                if (s.level[x][y] < U) s.level[x][y]++;
            } else {
                // Attack
                s.level[x][y]--;
                if (s.level[x][y] == 0) {
                    s.owner[x][y] = p;
                    s.level[x][y] = 1;
                } else {
                    // Attack failed to capture, piece removed?
                    // Rule: "If level doesn't become 0, collect attacking piece."
                    // So attacker is effectively removed, but the damage remains.
                    // "駒の復帰: collected pieces return to start"
                    alive[p] = false;
                    next_pos[p] = s.positions[p]; // Reset position
                }
            }
        }
    }

    // Update positions in state
    rep(p, M) s.positions[p] = next_pos[p];

    // 4. Update Scores (Lazy calculation or incremental)
    // Since N is small, full recalc is OK.
    fill(s.scores, s.scores + M, 0);
    rep(i, N) rep(j, N) {
        if (s.owner[i][j] != -1) {
            s.scores[s.owner[i][j]] += V[i][j] * s.level[i][j];
        }
    }
}

// Score Evaluation for Chokudai Search
void evaluate_state(State& s) {
    int my_score = s.scores[0];
    int max_ai_score = 0;
    rep2(i, 1, M) chmax(max_ai_score, s.scores[i]);
    
    // Objective: Maximize S0 / SA.
    // In search, Maximize S0 - w * SA is more stable.
    double w = 1.2; // Penalize AI slightly more to be safe
    s.eval_score = (double)my_score - w * max_ai_score;
    
    // Tie-breaker: Total owned value (even if level low)
    // int potential = 0;
    // rep(i, N) rep(j, N) if(s.owner[i][j] == 0) potential += V[i][j];
    // s.eval_score += potential * 0.001;
}

// Main Search Function
Point chokudai_search(const State& root_state, int turn) {
    // Time Limit
    // "2 sec" is total. 100 turns. ~20ms per turn. Safe: 15ms.
    Timer timer(0.015);
    
    // Beam Storage
    // max_depth 10 is enough lookahead for this dynamic game
    int max_depth = min(10, T_MAX - turn);
    if (max_depth <= 0) return root_state.positions[0]; // Should not happen

    vector<priority_queue<State>> beams(max_depth + 1);
    beams[0].push(root_state);

    // Keep track of best immediate move found so far
    Point best_move = root_state.positions[0]; // Fallback
    double best_eval = -1e18;

    int loop_cnt = 0;

    while (!timer.done()) {
        loop_cnt++;
        
        // Pick depth to expand
        // Simple strategy: Iterate depths, or pick random
        // Focusing on shallow depths first is good for breadth
        rep(d, max_depth) {
            if (beams[d].empty()) continue;
            
            // Pop best state
            State curr = beams[d].top();
            beams[d].pop();
            
            // Limit beam width implicitly by time or explicitly?
            // Chokudai search usually keeps expanding.
            // If the queue is huge, we process top ones.
            
            // Generate Moves
            vector<Point> moves;
            if (d == 0) {
                // Root: Use rigorous move generation
                moves = get_valid_moves(curr, 0);
                // Heuristic: Shuffle to avoid bias if scores equal
                // shuffle(rng(moves), ...); 
            } else {
                // Depth > 0: Simplify move generation to save time
                // Only consider 4 neighbors + stay
                Point p = curr.positions[0];
                moves.push_back(p);
                rep(dir, 4) {
                    int nx = p.x + dx[dir];
                    int ny = p.y + dy[dir];
                    if (inside(nx, ny)) {
                        // Very simplified check: Assume connectivity holds roughly
                        // or just allow it for simulation lookahead
                        moves.push_back({(int8_t)nx, (int8_t)ny});
                    }
                }
            }

            if (moves.empty()) moves.push_back(curr.positions[0]); // Stay if stuck

            // Expand
            // Limit number of children to avoid explosion
            int branch_limit = (d == 0) ? moves.size() : 2; 
            
            // Heuristic sorting of moves could help, but taking top N is simple
            rep(i, moves.size()) {
                if (timer.done()) break;
                
                // For depth > 0, random sample if too many moves
                if (d > 0 && i >= branch_limit) break; 

                Point mv = moves[i];
                State next_s = curr;
                
                if (d == 0) {
                    next_s.first_move_idx = mv.x * 10 + mv.y;
                }

                simulate(next_s, mv);
                evaluate_state(next_s);

                // Update global best (using evaluation from any depth)
                if (next_s.eval_score > best_eval) {
                    best_eval = next_s.eval_score;
                    if (d == 0) best_move = mv;
                    else {
                        // Recover first move
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
    // Read Initial Config
    if (!(cin >> N >> M >> T_MAX >> U)) return;
    
    rep(i, N) rep(j, N) cin >> V[i][j];
    
    State current_state;
    // Init owners and levels
    rep(i, N) rep(j, N) {
        current_state.owner[i][j] = -1;
        current_state.level[i][j] = 0;
    }
    
    rep(p, M) {
        int sx, sy;
        cin >> sx >> sy;
        current_state.positions[p] = {(int8_t)sx, (int8_t)sy};
        current_state.owner[sx][sy] = p;
        current_state.level[sx][sy] = 1;
    }
    
    // Initial score calc
    fill(current_state.scores, current_state.scores + M, 0);
    rep(i, N) rep(j, N) {
        if(current_state.owner[i][j] != -1)
            current_state.scores[current_state.owner[i][j]] += V[i][j];
    }

    // Game Loop
    for (int t = 1; t <= T_MAX; ++t) {
        // 1. Decide Move
        Point my_move = chokudai_search(current_state, t);
        
        // Output Move
        cout << (int)my_move.x << " " << (int)my_move.y << endl; // Flush included

        // 2. Read Feedback
        // Moves chosen by everyone
        rep(p, M) {
            int tx, ty;
            cin >> tx >> ty; 
            // We can store this if we want to learn AI behavior, 
            // but for MVP we ignore learning.
        }

        // Actual positions after resolution
        rep(p, M) {
            int ex, ey;
            cin >> ex >> ey;
            current_state.positions[p] = {(int8_t)ex, (int8_t)ey};
        }

        // Board state
        rep(i, N) rep(j, N) {
            int o; cin >> o;
            current_state.owner[i][j] = o;
        }
        rep(i, N) rep(j, N) {
            int l; cin >> l;
            current_state.level[i][j] = l;
        }

        // Update scores for next turn base
        fill(current_state.scores, current_state.scores + M, 0);
        rep(i, N) rep(j, N) {
            if(current_state.owner[i][j] != -1)
                current_state.scores[current_state.owner[i][j]] += V[i][j] * current_state.level[i][j];
        }
    }
}

int main()
{
    // Interactive problem: syncing with stdio is usually needed, 
    // but cin.tie(nullptr) is fine as long as we flush (endl does flush).
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    solve();
    
    return 0;
}