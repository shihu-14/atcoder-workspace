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

// ACL (AtCoder Library)
// #include <atcoder/all>
// using namespace atcoder;

using namespace std;

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
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())

const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const int INF = 1001001001;

const int N = 20;
int M; 
struct Point { int r, c; };
vector<array<Point, 2>> card_pos;

struct Node {
    int id;
    int parent;
    bool is_flipped;
    vector<int> children;
};

vector<Node> tree;
int root_id;

// Output variables
string output_moves;
Point current_pos;

mt19937 engine(12345);

// Stats
struct OpStats {
    long long try_count = 0;
    long long update_count = 0;
};
struct PhaseStats {
    OpStats regraft;
    OpStats flip;
    OpStats swap;
};
PhaseStats phase_stats[3];
int current_phase = 0;

int dist(Point p1, Point p2) {
    return abs(p1.r - p2.r) + abs(p1.c - p2.c);
}

Point get_start_pos(int id) {
    if (id == root_id) return {0, 0};
    int idx = tree[id].is_flipped ? 1 : 0;
    return card_pos[id][idx];
}

Point get_end_pos(int id) {
    if (id == root_id) return {0, 0};
    int idx = tree[id].is_flipped ? 0 : 1;
    return card_pos[id][idx];
}

// 差分計算用 (Start -> Child -> ... -> End)
int calc_local_wiring_cost(int u) {
    Point curr = get_start_pos(u);
    int cost = 0;
    
    for (int child : tree[u].children) {
        Point next = get_start_pos(child);
        cost += dist(curr, next);
        curr = get_end_pos(child);
    }
    
    if (u != root_id) {
        cost += dist(curr, get_end_pos(u));
    }
    return cost;
}

int calc_subtree_cost(int u) {
    int cost = 0;
    Point curr = get_start_pos(u);
    for (int child : tree[u].children) {
        cost += dist(curr, get_start_pos(child));
        cost += calc_subtree_cost(child);
        curr = get_end_pos(child);
    }
    if (u != root_id) cost += dist(curr, get_end_pos(u));
    return cost;
}

int calc_total_score() {
    return calc_subtree_cost(root_id);
}

bool is_ancestor(int u, int v) {
    int curr = v;
    while (curr != -1) {
        if (curr == u) return true;
        curr = tree[curr].parent;
    }
    return false;
}

// --- 初期解構築 (確率的貪欲法) ---
// random_pick: 上位候補からランダムに選ぶかどうか
void build_initial_solution(int mode, bool random_pick) {
    vector<pair<int, int>> pairs_by_val;
    rep(i, M) {
        int val = 0;
        if (mode == 0 || mode == 2) val = dist(card_pos[i][0], card_pos[i][1]);
        else if (mode == 3 || mode == 4) val = (card_pos[i][0].r + card_pos[i][0].c) + (card_pos[i][1].r + card_pos[i][1].c);
        else if (mode == 5) val = card_pos[i][0].r + card_pos[i][1].r;
        pairs_by_val.push_back({val, (int)i});
    }

    if (mode == 0 || mode == 3) sort(rng(pairs_by_val), greater<pair<int, int>>());
    else if (mode == 2 || mode == 4 || mode == 5) sort(rng(pairs_by_val));
    else shuffle(rng(pairs_by_val), engine);

    root_id = M;
    tree.assign(M + 1, Node());
    tree[root_id] = {root_id, -1, false, {}};
    rep(i, M) {
        tree[i].id = i;
        tree[i].is_flipped = false; 
    }

    // 候補保持用構造体
    struct Candidate {
        int increase;
        int parent;
        int idx;
        bool flip;
        bool operator<(const Candidate& o) const { return increase < o.increase; }
    };

    for (auto p : pairs_by_val) {
        int target_id = p.second;
        int d_pair = dist(card_pos[target_id][0], card_pos[target_id][1]);

        // 候補ノード列挙
        queue<int> q;
        q.push(root_id);
        vector<int> node_candidates;
        while(!q.empty()){
            int u = q.front(); q.pop();
            node_candidates.push_back(u);
            for(int c : tree[u].children) q.push(c);
        }

        vector<Candidate> best_candidates;
        // 上位K個を保持する (random_pick=trueの時)
        int K = random_pick ? 3 : 1; 

        for (int u : node_candidates) {
            int sz = tree[u].children.size();
            for (int i = 0; i <= sz; ++i) {
                Point prev_p = (i == 0) ? get_start_pos(u) : get_end_pos(tree[u].children[i-1]);
                Point next_p = (i == sz) ? ((u == root_id) ? get_start_pos(u) : get_end_pos(u)) : get_start_pos(tree[u].children[i]);
                
                int current_link_cost = 0;
                if (u != root_id || i < sz) current_link_cost = dist(prev_p, next_p);

                for (int f = 0; f < 2; ++f) {
                    bool flip = (f == 1);
                    Point t_start = card_pos[target_id][flip ? 1 : 0];
                    Point t_end = card_pos[target_id][flip ? 0 : 1];
                    
                    int increase = dist(prev_p, t_start) + d_pair;
                    if (u != root_id || i < sz) {
                        increase += dist(t_end, next_p);
                        increase -= current_link_cost;
                    }
                    
                    best_candidates.push_back({increase, u, i, flip});
                }
            }
        }

        // ソートして上位を選ぶ
        // 全候補をソートするのは重いが、N=20程度なら許容範囲
        if (best_candidates.size() > K) {
            partial_sort(best_candidates.begin(), best_candidates.begin() + K, best_candidates.end());
            best_candidates.resize(K);
        } else {
            sort(rng(best_candidates));
        }

        // 決定
        int pick_idx = 0;
        if (random_pick && !best_candidates.empty()) {
            pick_idx = engine() % best_candidates.size();
        }
        
        const auto& best = best_candidates[pick_idx];

        tree[target_id].parent = best.parent;
        tree[target_id].is_flipped = best.flip;
        tree[target_id].children.clear();
        tree[best.parent].children.insert(tree[best.parent].children.begin() + best.idx, target_id);
    }
}

// --- Simulated Annealing ---
void hill_climbing(chrono::steady_clock::time_point start_time) {
    int current_score = calc_total_score();
    int best_score = current_score;
    vector<Node> best_tree = tree;

    // パラメータ
    double start_temp = 400.0;
    double end_temp = 0.1;
    double time_limit = 1980.0;

    int iter = 0;
    
    const int prob_thresholds[3][2] = {
        {80, 85}, 
        {40, 60}, 
        {5, 95}   
    };

    int prob_regraft = prob_thresholds[0][0];
    int prob_flip    = prob_thresholds[0][1];

    while (true) {
        iter++;
        if ((iter & 0xFF) == 0) {
            auto now = chrono::steady_clock::now();
            double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
            if (elapsed > time_limit) break;

            if (elapsed < time_limit / 3.0) current_phase = 0;
            else if (elapsed < time_limit * 2.0 / 3.0) current_phase = 1;
            else current_phase = 2;

            prob_regraft = prob_thresholds[current_phase][0];
            prob_flip    = prob_thresholds[current_phase][1];
        }

        int type_rnd = engine() % 100;
        int delta_score = 0;
        bool accepted = false;

        // Regraft
        if (type_rnd < prob_regraft) {
            phase_stats[current_phase].regraft.try_count++;
            int target = engine() % M;
            int old_parent = tree[target].parent;
            
            int cost_before_removal = calc_local_wiring_cost(old_parent);
            auto& siblings = tree[old_parent].children;
            auto it = find(rng(siblings), target);
            int old_idx = distance(siblings.begin(), it);
            siblings.erase(it);
            int cost_after_removal = calc_local_wiring_cost(old_parent);
            int removal_delta = cost_after_removal - cost_before_removal;

            int best_p = -1;
            int best_i = -1;
            int min_incr = INF;
            
            vector<int> parent_candidates;
            parent_candidates.push_back(old_parent);
            rep(k, 30) {
                int p = (engine() % (M + 1));
                if (p == M) p = root_id;
                if (p == target) continue;
                if (is_ancestor(target, p)) continue;
                parent_candidates.push_back(p);
            }
            UNIQUE(parent_candidates);
            
            Point t_start = get_start_pos(target);
            Point t_end = get_end_pos(target);

            for (int p : parent_candidates) {
                int sz = tree[p].children.size();
                for (int i = 0; i <= sz; ++i) {
                    Point prev_p = (i == 0) ? get_start_pos(p) : get_end_pos(tree[p].children[i-1]);
                    Point next_p = (i == sz) ? ((p == root_id) ? get_start_pos(p) : get_end_pos(p)) : get_start_pos(tree[p].children[i]);
                    
                    int current_link_cost = 0;
                    if (p != root_id || i < sz) current_link_cost = dist(prev_p, next_p);
                    
                    int increase = dist(prev_p, t_start);
                    if (p != root_id || i < sz) {
                        increase += dist(t_end, next_p);
                        increase -= current_link_cost;
                    }
                    if (increase < min_incr) {
                        min_incr = increase;
                        best_p = p;
                        best_i = i;
                    }
                }
            }
            
            delta_score = removal_delta + min_incr;
            
            auto now = chrono::steady_clock::now();
            double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
            double temp = start_temp * pow(end_temp / start_temp, elapsed / time_limit);
            
            if (uniform_real_distribution<>(0.0, 1.0)(engine) < exp(-delta_score / temp)) {
                tree[target].parent = best_p;
                tree[best_p].children.insert(tree[best_p].children.begin() + best_i, target);
                current_score += delta_score;
                if (best_p != old_parent) {
                    accepted = true;
                    phase_stats[current_phase].regraft.update_count++;
                }
            } else {
                tree[target].parent = old_parent;
                tree[old_parent].children.insert(tree[old_parent].children.begin() + old_idx, target);
            }
        }
        // Flip
        else if (type_rnd < prob_flip) {
            phase_stats[current_phase].flip.try_count++;
            int target = engine() % M;
            int parent = tree[target].parent;
            
            int old_wiring = calc_local_wiring_cost(target) + calc_local_wiring_cost(parent);
            bool old_flip = tree[target].is_flipped;
            tree[target].is_flipped = !old_flip;
            int new_wiring = calc_local_wiring_cost(target) + calc_local_wiring_cost(parent);
            
            delta_score = new_wiring - old_wiring;

            auto now = chrono::steady_clock::now();
            double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
            double temp = start_temp * pow(end_temp / start_temp, elapsed / time_limit);
            
            if (uniform_real_distribution<>(0.0, 1.0)(engine) < exp(-delta_score / temp)) {
                current_score += delta_score;
                accepted = true;
                phase_stats[current_phase].flip.update_count++;
            } else {
                tree[target].is_flipped = old_flip;
            }
        }
        // Siblings Optimization (Swap / 2-opt / Insert)
        else {
            phase_stats[current_phase].swap.try_count++;
            int p = -1;
            rep(k, 10) {
                int temp = (engine() % (M + 1));
                if (temp == M) temp = root_id;
                if (tree[temp].children.size() >= 2) {
                    p = temp;
                    break;
                }
            }
            if (p != -1) {
                int old_p_cost = calc_local_wiring_cost(p);
                
                // 3種類の操作を確率で選択
                // 0: Swap, 1: 2-opt(Reverse), 2: Insert
                int sub_type = engine() % 3;
                bool reverted = false;

                if (sub_type == 0) { // Swap
                    int sz = tree[p].children.size();
                    int i1 = engine() % sz;
                    int i2 = engine() % sz;
                    if (i1 != i2) {
                        swap(tree[p].children[i1], tree[p].children[i2]);
                        
                        int new_p_cost = calc_local_wiring_cost(p);
                        delta_score = new_p_cost - old_p_cost;
                        
                        auto now = chrono::steady_clock::now();
                        double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
                        double temp = start_temp * pow(end_temp / start_temp, elapsed / time_limit);

                        if (uniform_real_distribution<>(0.0, 1.0)(engine) < exp(-delta_score / temp)) {
                            current_score += delta_score;
                            accepted = true;
                            phase_stats[current_phase].swap.update_count++;
                        } else {
                            swap(tree[p].children[i1], tree[p].children[i2]); // Revert
                        }
                    }
                } else if (sub_type == 1) { // 2-opt
                    int sz = tree[p].children.size();
                    int i1 = engine() % sz;
                    int i2 = engine() % sz;
                    if (i1 > i2) swap(i1, i2);
                    if (i1 != i2) {
                        reverse(tree[p].children.begin() + i1, tree[p].children.begin() + i2 + 1);
                        
                        int new_p_cost = calc_local_wiring_cost(p);
                        delta_score = new_p_cost - old_p_cost;
                        
                        auto now = chrono::steady_clock::now();
                        double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
                        double temp = start_temp * pow(end_temp / start_temp, elapsed / time_limit);

                        if (uniform_real_distribution<>(0.0, 1.0)(engine) < exp(-delta_score / temp)) {
                            current_score += delta_score;
                            accepted = true;
                            phase_stats[current_phase].swap.update_count++;
                        } else {
                            reverse(tree[p].children.begin() + i1, tree[p].children.begin() + i2 + 1); // Revert
                        }
                    }
                } else { // Insert (Block Move)
                    int sz = tree[p].children.size();
                    int i_from = engine() % sz;
                    int i_to = engine() % sz;
                    if (i_from != i_to) {
                        int val = tree[p].children[i_from];
                        tree[p].children.erase(tree[p].children.begin() + i_from);
                        tree[p].children.insert(tree[p].children.begin() + i_to, val);
                        
                        int new_p_cost = calc_local_wiring_cost(p);
                        delta_score = new_p_cost - old_p_cost;
                        
                        auto now = chrono::steady_clock::now();
                        double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
                        double temp = start_temp * pow(end_temp / start_temp, elapsed / time_limit);

                        if (uniform_real_distribution<>(0.0, 1.0)(engine) < exp(-delta_score / temp)) {
                            current_score += delta_score;
                            accepted = true;
                            phase_stats[current_phase].swap.update_count++;
                        } else {
                            // Revert
                            tree[p].children.erase(tree[p].children.begin() + i_to);
                            tree[p].children.insert(tree[p].children.begin() + i_from, val);
                        }
                    }
                }
            }
        }

        if (accepted) {
            if (current_score < best_score) {
                best_score = current_score;
                best_tree = tree;
            }
        }
    }
    tree = best_tree;

    auto end_time = chrono::steady_clock::now();
    double total_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cerr << "=== Final Result ===" << endl;
    cerr << "Best Score: " << best_score << endl;
    cerr << "Total Time: " << total_time << " ms" << endl;
    cerr << "Total Iter: " << iter << endl;
    
    const string phase_names[] = {"Phase 1 (Early)", "Phase 2 (Mid)", "Phase 3 (Late)"};
    const string op_names[] = {"Regraft", "Flip   ", "Swap   "};
    
    cerr << "\n=== Detailed Stats (Try / Update / Rate) ===" << endl;
    rep(p, 3) {
        cerr << "--- " << phase_names[p] << " ---" << endl;
        long long phase_try = 0;
        long long phase_upd = 0;
        long long t, u;
        double r;

        t = phase_stats[p].regraft.try_count; u = phase_stats[p].regraft.update_count;
        r = (t > 0) ? (double)u / t * 100.0 : 0.0;
        cerr << op_names[0] << ": " << setw(8) << t << " / " << setw(8) << u << " (" << fixed << setprecision(2) << setw(6) << r << "%)" << endl;
        phase_try += t; phase_upd += u;

        t = phase_stats[p].flip.try_count; u = phase_stats[p].flip.update_count;
        r = (t > 0) ? (double)u / t * 100.0 : 0.0;
        cerr << op_names[1] << ": " << setw(8) << t << " / " << setw(8) << u << " (" << fixed << setprecision(2) << setw(6) << r << "%)" << endl;
        phase_try += t; phase_upd += u;

        t = phase_stats[p].swap.try_count; u = phase_stats[p].swap.update_count;
        r = (t > 0) ? (double)u / t * 100.0 : 0.0;
        cerr << op_names[2] << ": " << setw(8) << t << " / " << setw(8) << u << " (" << fixed << setprecision(2) << setw(6) << r << "%)" << endl;
        phase_try += t; phase_upd += u;

        double total_rate = (phase_try > 0) ? (double)phase_upd / phase_try * 100.0 : 0.0;
        cerr << ">> TOTAL : " << setw(8) << phase_try << " / " << setw(8) << phase_upd << " (" << fixed << setprecision(2) << setw(6) << total_rate << "%)" << endl;
    }
}

void move_to(Point target, bool pick) {
    while (current_pos.r < target.r) { output_moves += 'D'; current_pos.r++; }
    while (current_pos.r > target.r) { output_moves += 'U'; current_pos.r--; }
    while (current_pos.c < target.c) { output_moves += 'R'; current_pos.c++; }
    while (current_pos.c > target.c) { output_moves += 'L'; current_pos.c--; }
    if (pick) output_moves += 'Z';
}

// 1. 事後処理によるパス短縮
string optimize_moves(string s) {
    bool changed = true;
    while (changed) {
        changed = false;
        string next_s = "";
        for (int i = 0; i < s.size(); ++i) {
            if (i + 1 < s.size()) {
                // 移動のみで往復している場合(Pick/Dropを含まない)
                if ((s[i] == 'U' && s[i+1] == 'D') || 
                    (s[i] == 'D' && s[i+1] == 'U') || 
                    (s[i] == 'L' && s[i+1] == 'R') || 
                    (s[i] == 'R' && s[i+1] == 'L')) {
                    i++; 
                    changed = true;
                    continue;
                }
            }
            next_s += s[i];
        }
        s = next_s;
    }
    return s;
}

void dfs_solve(int u) {
    if (u != root_id) {
        move_to(get_start_pos(u), true);
    }
    for (int child : tree[u].children) {
        dfs_solve(child);
    }
    if (u != root_id) {
        move_to(get_end_pos(u), true);
    }
}

void solve() {
    int n_in;
    if (!(cin >> n_in)) return;
    M = N * N / 2;
    vector<vector<int>> grid(N, vector<int>(N));
    card_pos.resize(M);
    vector<int> card_cnt(M, 0);
    rep(i, N) rep(j, N) {
        cin >> grid[i][j];
        int id = grid[i][j];
        if (card_cnt[id] < 2) {
            card_pos[id][card_cnt[id]] = {(int)i, (int)j};
            card_cnt[id]++;
        }
    }

    auto start_time = chrono::steady_clock::now();
    int multi_start_limit_ms = 300;
    
    int best_initial_score = INF;
    vector<Node> best_initial_tree;
    int try_count = 0;

    // 0:距離降順, 1:ランダム, 2:距離昇順, 3:原点降順, 4:原点昇順, 5:行スキャン
    // ここで確率的貪欲法(random_pick=true)を使って、最強のMode0も複数回試す
    
    // 定義済みモードを一通り試す(ランダム性あり)
    for(int mode=0; mode<=5; ++mode) {
        // 何回かずつ試すと良い
        int loops = (mode == 0) ? 5 : 1; // 最強のMode0は多めに
        rep(_, loops) {
            build_initial_solution(mode, true);
            int score = calc_total_score();
            if(score < best_initial_score){
                best_initial_score = score;
                best_initial_tree = tree;
            }
            try_count++;
        }
    }

    // 残り時間はランダム順(Mode 1) と 最強(Mode 0) を交互に
    while (true) {
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
        if (elapsed > multi_start_limit_ms) break;

        // 50%でランダム順、50%で距離順(の確率的選択)
        int mode = (try_count % 2 == 0) ? 0 : 1;
        build_initial_solution(mode, true);
        
        int score = calc_total_score();
        if (score < best_initial_score) {
            best_initial_score = score;
            best_initial_tree = tree;
        }
        try_count++;
    }
    tree = best_initial_tree;
    cerr << "Multi-Start Best Score: " << best_initial_score << " (Trials: " << try_count << ")" << endl;

    hill_climbing(start_time);

    current_pos = {0, 0};
    output_moves = "";
    dfs_solve(root_id);
    
    // 最後にパス圧縮
    string optimized = optimize_moves(output_moves);
    
    rep(i, optimized.size()) cout << optimized[i] << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}