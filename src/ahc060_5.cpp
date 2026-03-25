#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <algorithm>
#include <random>
#include <map>
#include <cmath>
#include <iomanip>
#include <chrono> // 追加: 時間計測用
#include <atcoder/all>

using namespace std;
using namespace atcoder;

// 乱数生成
mt19937 rng(42);

// グローバル変数
int N, M, K, T;
vector<vector<int>> adj;
vector<set<string>> shop_inventory;
string current_cone = "";
int current_pos = 0;
int prev_pos = -1;
vector<char> node_colors;

// 統計情報
int current_red_count = 0;
int total_trees = 0;

// 辺コスト管理
// 1. ショップごとの管理（機会損失防止）
vector<map<pair<int, int>, int>> shop_edge_visits;
// 2. 全体共有の管理（物理的なループ防止）
map<pair<int, int>, int> global_edge_visits;

// パス履歴
vector<pair<int, int>> current_delivery_path_edges;

// 座標
struct Point { int x, y; };
vector<Point> coords;

bool is_shop(int v) { return v < K; }
bool is_tree(int v) { return v >= K; }
char get_tree_color(int v) { return node_colors[v]; }

pair<int, int> get_edge_key(int u, int v) {
    if (u > v) swap(u, v);
    return {u, v};
}

// 候補を表す構造体
struct TargetCandidate {
    int target_node; 
    int first_move;  
    int incoming_node; 
    double score;    
    double total_score; 
    int distance;
    string generated_str;
    vector<pair<int, int>> path_edges;
};

// ---------------------------------------------------------
// 経路生成関数 (ハイブリッド: BFS + ランダムウォーク)
// ---------------------------------------------------------
vector<TargetCandidate> get_candidates(int start_node, int forbidden_node, const string& cone_content) {
    // 重複除去用マップ: {target_node, generated_str} -> candidate_index
    map<pair<int, string>, int> seen_candidates;
    vector<TargetCandidate> candidates;
    
    // 候補追加用のラムダ関数
    auto add_candidate = [&](const vector<int>& path_nodes) {
        if (path_nodes.empty()) return;
        int u = path_nodes.back(); // 目的地
        
        string s = cone_content;
        vector<pair<int, int>> path_edges;
        double local_dirty = 0;
        double global_dirty = 0;
        
        // パス解析
        int curr = start_node;
        for (int next_node : path_nodes) {
            auto edge = get_edge_key(curr, next_node);
            path_edges.push_back(edge);
            
            // 文字列生成
            if (is_tree(next_node)) s += get_tree_color(next_node);
            
            // コスト計算
            local_dirty += shop_edge_visits[u][edge];
            global_dirty += global_edge_visits[edge];
            
            curr = next_node;
        }
        
        // 直前の頂点（次の探索での禁止手）
        int incoming = start_node;
        if (path_nodes.size() >= 2) incoming = path_nodes[path_nodes.size() - 2];

        // 既知の文字列は「絶対禁止」
        if (shop_inventory[u].count(s)) {
            return; 
        }

        int dist = (int)path_nodes.size();

        // スコア計算
        double base = 1e10;
        double dist_penalty = (double)dist * 1e8; 
        double global_penalty = global_dirty * 5000.0; 
        double local_penalty = local_dirty; 

        double val = base - dist_penalty - global_penalty - local_penalty;
        
        if (s.length() > 6) val -= 5e9; 

        // 重複チェックと更新ロジック
        if (seen_candidates.count({u, s})) {
            int idx = seen_candidates[{u, s}];
            if (val > candidates[idx].score) {
                candidates[idx] = {u, path_nodes[0], incoming, val, val, dist, s, path_edges};
            }
            return;
        }

        // 新規追加
        candidates.push_back({u, path_nodes[0], incoming, val, val, dist, s, path_edges});
        seen_candidates[{u, s}] = (int)candidates.size() - 1;
    };

    const int DEPTH_LIMIT = 8;

    // 1. BFS (最短経路の確保)
    {
        queue<int> bq;
        bq.push(start_node);
        vector<int> dist_map(N, -1);
        vector<int> par(N, -1);
        dist_map[start_node] = 0;
        
        while(!bq.empty()){
            int u = bq.front(); bq.pop();
            if(dist_map[u] >= DEPTH_LIMIT) continue;
            
            if(is_shop(u) && u != start_node){
                vector<int> path;
                int curr = u;
                while(curr != start_node){
                    path.push_back(curr);
                    curr = par[curr];
                }
                reverse(path.begin(), path.end());
                add_candidate(path);
            }
            
            if(is_shop(u) && u != start_node) continue;
            
            for(int v : adj[u]){
                if(u == start_node && v == forbidden_node) continue;
                if(dist_map[v] == -1){
                    dist_map[v] = dist_map[u] + 1;
                    par[v] = u;
                    bq.push(v);
                }
            }
        }
    }

    // 2. Random Walk (迂回経路・裏道の探索)
    int RANDOM_TRIALS = 100; 
    for(int i=0; i<RANDOM_TRIALS; ++i) {
        int curr = start_node;
        int prev = forbidden_node;
        vector<int> path;
        
        for(int step=0; step<DEPTH_LIMIT; ++step) {
            vector<int> valid_nexts;
            for(int v : adj[curr]) {
                if(v != prev) valid_nexts.push_back(v);
            }
            if(valid_nexts.empty()) break;
            
            int next_node = valid_nexts[rng() % valid_nexts.size()];
            path.push_back(next_node);
            
            if(is_shop(next_node)) {
                add_candidate(path);
                break;
            }
            
            prev = curr;
            curr = next_node;
        }
    }
    
    // スコア降順ソート
    sort(candidates.begin(), candidates.end(), [](const TargetCandidate& a, const TargetCandidate& b){
        return a.score > b.score;
    });

    return candidates;
}

// ---------------------------------------------------------
// D回連続貪欲 (再帰)
// ---------------------------------------------------------
TargetCandidate solve_recursive_greedy(int current_node, int forbidden_node, int depth, set<pair<int, string>>& temp_delivered, string current_cone_content) {
    
    if (depth == 0) return {-1, -1, -1, 0.0, 0.0, 0, "", {}};

    auto candidates = get_candidates(current_node, forbidden_node, current_cone_content);

    if (candidates.empty()) return {-1, -1, -1, -1e18, -1e18, 0, "", {}};

    int beam_width = 3;
    int count = 0;
    TargetCandidate best_sequence = {-1, -1, -1, -1e18, -1e18, 0, "", {}};
    bool any_valid = false;

    for (auto& cand : candidates) {
        if (count++ >= beam_width) break;

        if (temp_delivered.count({cand.target_node, cand.generated_str})) {
            continue; 
        }

        // --- 仮更新 ---
        for (auto& edge : cand.path_edges) {
            shop_edge_visits[cand.target_node][edge]++;
            global_edge_visits[edge]++;
        }
        temp_delivered.insert({cand.target_node, cand.generated_str});

        // --- 再帰呼び出し ---
        TargetCandidate next_step = solve_recursive_greedy(cand.target_node, cand.incoming_node, depth - 1, temp_delivered, "");
        
        if (next_step.target_node != -1) {
            cand.total_score = cand.score + next_step.total_score;
        } else {
            cand.total_score = cand.score; 
        }

        // --- 巻き戻し ---
        temp_delivered.erase({cand.target_node, cand.generated_str});
        for (auto& edge : cand.path_edges) {
            shop_edge_visits[cand.target_node][edge]--;
            global_edge_visits[edge]--;
        }

        if (!any_valid || cand.total_score > best_sequence.total_score) {
            best_sequence = cand;
            any_valid = true;
        }
    }
    
    if (!any_valid) return {-1, -1, -1, -1e18, -1e18, 0, "", {}};
    
    return best_sequence;
}

void solve() {
    // 計測開始
    auto start_clock = chrono::high_resolution_clock::now();

    cin >> N >> M >> K >> T;
    adj.resize(N);
    shop_inventory.resize(K);
    coords.resize(N);
    node_colors.assign(N, 'W');
    
    shop_edge_visits.resize(K);
    global_edge_visits.clear();
    current_delivery_path_edges.clear();
    
    total_trees = N - K;

    for(int i=0; i<M; ++i){
        int u,v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        global_edge_visits[{min(u,v), max(u,v)}] = 0;
    }
    for(int i=0; i<N; ++i) cin >> coords[i].x >> coords[i].y;

    for(int turn=0; turn<T; ++turn) {
        
        bool action2_performed = false;

        // --- Action 2 (Dispersal Strategy) ---
        long long target_reds = (long long)turn * total_trees / T;

        if (current_red_count < target_reds) {
            if (is_tree(current_pos) && get_tree_color(current_pos) == 'W') {
                int neighbors = 0;
                int red_neighbors = 0;
                for (int neighbor : adj[current_pos]) {
                    if (is_tree(neighbor)) {
                        neighbors++;
                        if (get_tree_color(neighbor) == 'R') red_neighbors++;
                    }
                }
                bool do_change = false;
                if (neighbors == 0) do_change = true;
                else if ((double)red_neighbors / neighbors < 0.5) do_change = true;

                if (!do_change) {
                    long long lag = target_reds - current_red_count;
                    if (lag > 5 || turn > T * 0.9) do_change = true;
                }

                if (do_change) {
                    cout << -1 << endl;
                    node_colors[current_pos] = 'R';
                    current_red_count++;
                    action2_performed = true;
                }
            }
        }

        if (action2_performed) continue;

        // --- Action 1 ---
        set<pair<int, string>> temp_delivered;
        int LOOKAHEAD_DEPTH = 2; 
        
        TargetCandidate best_move = solve_recursive_greedy(current_pos, prev_pos, LOOKAHEAD_DEPTH, temp_delivered, current_cone);
        
        int next_node = -1;

        if (best_move.target_node != -1) {
            next_node = best_move.first_move;
        } else {
            // 脱出ロジック
            vector<pair<int, int>> neighbors;
            long long min_cost = 2e18;
            
            for(int v : adj[current_pos]) {
                if(v != prev_pos) {
                    long long cost = global_edge_visits[get_edge_key(current_pos, v)];
                    if (cost < min_cost) min_cost = cost;
                    neighbors.push_back({(int)cost, v});
                }
            }
            
            vector<int> escape_candidates;
            for(auto p : neighbors) {
                if (p.first == min_cost) {
                    escape_candidates.push_back(p.second);
                }
            }
            
            if(!escape_candidates.empty()) {
                next_node = escape_candidates[rng() % escape_candidates.size()];
            } else if (!adj[current_pos].empty()) {
                next_node = adj[current_pos][0];
            }
        }

        cout << next_node << endl;
        
        auto edge_key = get_edge_key(current_pos, next_node);
        current_delivery_path_edges.push_back(edge_key);
        global_edge_visits[edge_key]++;

        prev_pos = current_pos;
        current_pos = next_node;
        
        if (is_tree(current_pos)) {
            current_cone += get_tree_color(current_pos);
        } else {
            shop_inventory[current_pos].insert(current_cone);
            current_cone = "";
            for (auto edge : current_delivery_path_edges) {
                shop_edge_visits[current_pos][edge]++;
            }
            current_delivery_path_edges.clear();
        }
    }

    // 計測終了 & 出力
    auto end_clock = chrono::high_resolution_clock::now();
    double duration = chrono::duration_cast<chrono::milliseconds>(end_clock - start_clock).count();
    cerr << "Execution Time: " << duration << " ms" << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}