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


// グローバル定数・変数
const int N = 20;
int M; // ペア数 (N*N/2)
struct Point { int r, c; };
// card_pos[pair_id][0] と card_pos[pair_id][1]
vector<array<Point, 2>> card_pos;

// 木構造のノード
struct Node {
    int id;               // ペアID (0 ~ M-1). -1はルート
    int parent;           // 親ノードID
    bool is_flipped;      // trueなら 1->0 の順, falseなら 0->1 の順
    vector<int> children; // 子ノードのリスト（順序付き）
    Point drop_pos;       // 仮想的な拠点座標 (Drop)
};

// 状態管理
vector<Node> tree; // tree[id] がペアidの情報。tree[M]をルートとする。
int root_id;

// 出力用変数をグローバル上部に移動 (コンパイルエラー回避)
string output_moves;
Point current_pos;

// 乱数エンジン
mt19937 engine(12345);

// --- 詳細デバッグ用構造体 ---
struct OpStats {
    long long try_count = 0;
    long long update_count = 0;
};

struct PhaseStats {
    OpStats regraft;
    OpStats flip;
    OpStats swap;
    OpStats move_drop;
};

// 3つのフェーズ (0:序盤, 1:中盤, 2:終盤)
PhaseStats phase_stats[3];
int current_phase = 0;

// 距離計算
int dist(Point p1, Point p2) {
    return abs(p1.r - p2.r) + abs(p1.c - p2.c);
}

// ノードの開始座標取得
Point get_start_pos(int id) {
    if (id == root_id) return {0, 0}; // ルートは(0,0)スタート
    int idx = tree[id].is_flipped ? 1 : 0;
    return card_pos[id][idx];
}

// ノードの終了座標取得
Point get_end_pos(int id) {
    if (id == root_id) return {0, 0}; // ルートの終わりは便宜上定義しないが、計算上使用時は注意
    int idx = tree[id].is_flipped ? 0 : 1;
    return card_pos[id][idx];
}

// あるノード（部分木）の内部移動コストのみを計算する (初期スコア計算用)
int calc_subtree_cost(int u) {
    int cost = 0;
    Point center = tree[u].drop_pos;
    
    // Start -> Center
    cost += dist(get_start_pos(u), center);

    Point curr = center;
    for (int child : tree[u].children) {
        cost += dist(curr, get_start_pos(child));
        cost += calc_subtree_cost(child); // 再帰
        curr = get_end_pos(child);
    }
    // 最後の子から Center へ戻る
    if (!tree[u].children.empty()) {
        cost += dist(curr, center);
    }

    // Center -> End
    if (u != root_id) cost += dist(center, get_end_pos(u));
    
    return cost;
}

// 全体のスコア計算 (初期計算用)
int calc_total_score() {
    return calc_subtree_cost(root_id);
}

// --- 差分計算のためのヘルパー関数 ---
// Start -> Drop -> Child1 ... ChildK -> Drop -> End
int calc_local_wiring_cost(int u) {
    Point start = get_start_pos(u);
    Point end = get_end_pos(u);
    Point center = tree[u].drop_pos;

    // 1. 親自身の移動 (Start -> Drop ... Drop -> End)
    int cost = dist(start, center);
    if (u != root_id) cost += dist(center, end);

    // 2. 子供の巡回 (Drop -> Child ... Child -> Drop)
    Point curr = center;
    for (int child : tree[u].children) {
        Point next = get_start_pos(child);
        cost += dist(curr, next);
        curr = get_end_pos(child);
    }
    
    // 子供がいる場合のみ、最後に戻るコストが発生
    if (!tree[u].children.empty()) {
        cost += dist(curr, center);
    }

    return cost;
}

// uがvの祖先かどうか判定
bool is_ancestor(int u, int v) {
    int curr = v;
    while (curr != -1) {
        if (curr == u) return true;
        curr = tree[curr].parent;
    }
    return false;
}

// --- 初期解構築 ---
void build_initial_solution(int mode) {
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
    tree[root_id] = {root_id, -1, false, {}, {0, 0}};
    rep(i, M) {
        tree[i].id = i;
        tree[i].is_flipped = false; 
        tree[i].drop_pos = card_pos[i][0]; // 初期値はStart位置
    }

    for (auto p : pairs_by_val) {
        int target_id = p.second;
        int best_parent = -1;
        int best_idx = -1;
        int min_increase = INF;
        bool best_flip = false;
        Point best_drop = {0,0};

        queue<int> q;
        q.push(root_id);
        vector<int> candidates;
        while(!q.empty()){
            int u = q.front(); q.pop();
            candidates.push_back(u);
            for(int c : tree[u].children) q.push(c);
        }

        int d_pair = dist(card_pos[target_id][0], card_pos[target_id][1]);

        for (int u : candidates) {
            int sz = tree[u].children.size();
            for (int i = 0; i <= sz; ++i) {
                Point u_center = tree[u].drop_pos;
                Point prev_p = (i == 0) ? u_center : get_end_pos(tree[u].children[i-1]);
                Point next_p = (i == sz) ? u_center : get_start_pos(tree[u].children[i]);
                
                int current_link_cost = dist(prev_p, next_p);

                for (int f = 0; f < 2; ++f) {
                    bool flip = (f == 1);
                    Point t_start = card_pos[target_id][flip ? 1 : 0];
                    Point t_end = card_pos[target_id][flip ? 0 : 1];
                    
                    int increase = dist(prev_p, t_start) + d_pair + dist(t_end, next_p) - current_link_cost;
                    
                    if (increase < min_increase) {
                        min_increase = increase;
                        best_parent = u;
                        best_idx = i;
                        best_flip = flip;
                        best_drop = t_start; 
                    }
                }
            }
        }

        tree[target_id].parent = best_parent;
        tree[target_id].is_flipped = best_flip;
        tree[target_id].drop_pos = best_drop;
        tree[target_id].children.clear();
        tree[best_parent].children.insert(tree[best_parent].children.begin() + best_idx, target_id);
    }
}

// --- Simulated Annealing (焼きなまし法) ---
void hill_climbing(chrono::steady_clock::time_point start_time) {
    int current_score = calc_total_score();
    int best_score = current_score;
    vector<Node> best_tree = tree;

    // パラメータ (指定値)
    double start_temp = 4000.0; 
    double end_temp = 0.1;     
    double time_limit = 1980.0; 

    int iter = 0;
    
    // Phase thresholds (指定値)
    // {Regraft, Flip} の閾値。残りが Swap/MoveDrop
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

        // --- Smart Re-grafting ---
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
            
            for (int p : parent_candidates) {
                int sz = tree[p].children.size();
                Point p_center = tree[p].drop_pos;
                for (int i = 0; i <= sz; ++i) {
                    Point prev_p = (i == 0) ? p_center : get_end_pos(tree[p].children[i-1]);
                    Point next_p = (i == sz) ? p_center : get_start_pos(tree[p].children[i]);
                    
                    int current_link_cost = dist(prev_p, next_p);
                    int increase = dist(prev_p, get_start_pos(target)) + dist(get_end_pos(target), next_p) - current_link_cost;
                    
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
        // --- Flip ---
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
        // --- Swap / 2-opt / Move Drop ---
        else {
            // 指定閾値外 (else) なので、ここで Swap と Move Drop を分岐させる
            // 例: 90% Swap, 10% Move Drop
            
            int sub_type = engine() % 10;
            
            if (sub_type == 0) { // 10% Move Drop
                phase_stats[current_phase].move_drop.try_count++;
                
                int target = -1;
                rep(k, 10) {
                    int t = engine() % M;
                    if (!tree[t].children.empty()) {
                        target = t;
                        break;
                    }
                }

                if (target != -1) {
                    Point old_drop = tree[target].drop_pos;
                    int old_cost = calc_local_wiring_cost(target);
                    
                    int dr = (engine() % 5) - 2; 
                    int dc = (engine() % 5) - 2;
                    Point new_drop = {old_drop.r + dr, old_drop.c + dc};
                    new_drop.r = clamp(new_drop.r, 0, N - 1);
                    new_drop.c = clamp(new_drop.c, 0, N - 1);
                    
                    tree[target].drop_pos = new_drop;
                    int new_cost = calc_local_wiring_cost(target);
                    
                    delta_score = new_cost - old_cost;
                    
                    auto now = chrono::steady_clock::now();
                    double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
                    double temp = start_temp * pow(end_temp / start_temp, elapsed / time_limit);

                    if (uniform_real_distribution<>(0.0, 1.0)(engine) < exp(-delta_score / temp)) {
                        current_score += delta_score;
                        accepted = true;
                        phase_stats[current_phase].move_drop.update_count++;
                    } else {
                        tree[target].drop_pos = old_drop;
                    }
                }
            } else { // 90% Swap or 2-opt
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
                    int sz = tree[p].children.size();
                    int i1 = engine() % sz;
                    int i2 = engine() % sz;
                    if (i1 > i2) swap(i1, i2);

                    if (i1 != i2) {
                        int old_p_cost = calc_local_wiring_cost(p);
                        bool use_reverse = (engine() % 2 == 0); // 50% 2-opt

                        if (use_reverse) reverse(tree[p].children.begin() + i1, tree[p].children.begin() + i2 + 1);
                        else swap(tree[p].children[i1], tree[p].children[i2]);

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
                            if (use_reverse) reverse(tree[p].children.begin() + i1, tree[p].children.begin() + i2 + 1);
                            else swap(tree[p].children[i1], tree[p].children[i2]);
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

    // Stats
    auto end_time = chrono::steady_clock::now();
    double total_time = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cerr << "=== Final Result ===" << endl;
    cerr << "Best Score: " << best_score << endl;
    cerr << "Total Time: " << total_time << " ms" << endl;
    cerr << "Total Iter: " << iter << endl;
    
    const string phase_names[] = {"Phase 1 (Early)", "Phase 2 (Mid)", "Phase 3 (Late)"};
    const string op_names[] = {"Regraft", "Flip   ", "Swap   ", "Drop   "};
    
    cerr << "\n=== Detailed Stats (Try / Update / Rate) ===" << endl;
    rep(p, 3) {
        cerr << "--- " << phase_names[p] << " ---" << endl;
        long long phase_try = 0;
        long long phase_upd = 0;
        
        long long t, u;
        double r;

        // Regraft
        t = phase_stats[p].regraft.try_count; u = phase_stats[p].regraft.update_count;
        r = (t > 0) ? (double)u / t * 100.0 : 0.0;
        cerr << op_names[0] << ": " << setw(8) << t << " / " << setw(8) << u << " (" << fixed << setprecision(2) << setw(6) << r << "%)" << endl;
        phase_try += t; phase_upd += u;

        // Flip
        t = phase_stats[p].flip.try_count; u = phase_stats[p].flip.update_count;
        r = (t > 0) ? (double)u / t * 100.0 : 0.0;
        cerr << op_names[1] << ": " << setw(8) << t << " / " << setw(8) << u << " (" << fixed << setprecision(2) << setw(6) << r << "%)" << endl;
        phase_try += t; phase_upd += u;

        // Swap
        t = phase_stats[p].swap.try_count; u = phase_stats[p].swap.update_count;
        r = (t > 0) ? (double)u / t * 100.0 : 0.0;
        cerr << op_names[2] << ": " << setw(8) << t << " / " << setw(8) << u << " (" << fixed << setprecision(2) << setw(6) << r << "%)" << endl;
        phase_try += t; phase_upd += u;

        // Move Drop
        t = phase_stats[p].move_drop.try_count; u = phase_stats[p].move_drop.update_count;
        r = (t > 0) ? (double)u / t * 100.0 : 0.0;
        cerr << op_names[3] << ": " << setw(8) << t << " / " << setw(8) << u << " (" << fixed << setprecision(2) << setw(6) << r << "%)" << endl;
        phase_try += t; phase_upd += u;

        double total_rate = (phase_try > 0) ? (double)phase_upd / phase_try * 100.0 : 0.0;
        cerr << ">> TOTAL : " << setw(8) << phase_try << " / " << setw(8) << phase_upd << " (" << fixed << setprecision(2) << setw(6) << total_rate << "%)" << endl;
    }
}

// --- Output Generation ---

void move_to(Point target, bool pick) {
    while (current_pos.r < target.r) { output_moves += 'D'; current_pos.r++; }
    while (current_pos.r > target.r) { output_moves += 'U'; current_pos.r--; }
    while (current_pos.c < target.c) { output_moves += 'R'; current_pos.c++; }
    while (current_pos.c > target.c) { output_moves += 'L'; current_pos.c--; }
    if (pick) output_moves += 'Z';
}

void dfs_solve(int u) {
    Point start = get_start_pos(u);
    Point end = get_end_pos(u);
    Point center = tree[u].drop_pos;

    // 1. 親(Prev) -> Start(u) [PICK]
    if (u != root_id) move_to(start, true);

    // 2. Start(u) -> Drop(u) [PLACE]
    if (u != root_id) {
        if (start.r != center.r || start.c != center.c) {
            // 移動 (pick=false)
            while (current_pos.r < center.r) { output_moves += 'D'; current_pos.r++; }
            while (current_pos.r > center.r) { output_moves += 'U'; current_pos.r--; }
            while (current_pos.c < center.c) { output_moves += 'R'; current_pos.c++; }
            while (current_pos.c > center.c) { output_moves += 'L'; current_pos.c--; }
            // 実際に置く
            output_moves += 'X';
        }
    }
    
    // 3. 子供たちを処理 (DropPos から出発)
    for (int child : tree[u].children) {
        dfs_solve(child);
    }
    
    // 4. Drop(u) [PICK] -> End(u) [PICK]
    if (u != root_id) {
        if (start.r != center.r || start.c != center.c) {
            // Pick
            output_moves += 'Z';
        }
        move_to(end, true);
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

    for(int mode=0; mode<=5; ++mode) {
        build_initial_solution(mode);
        int score = calc_total_score();
        if(score < best_initial_score){
            best_initial_score = score;
            best_initial_tree = tree;
        }
        try_count++;
    }
    while (true) {
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
        if (elapsed > multi_start_limit_ms) break;
        build_initial_solution(1); 
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
    
    rep(i, output_moves.size()) cout << output_moves[i] << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}