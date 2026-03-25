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
#include <chrono>
#include <cstring> 
#include <tuple>
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
int shop_edge_visits[15][105][105];   
int global_edge_visits[105][105];

// タブーリスト
deque<pair<int, int>> recent_moves;
const int TABOO_SIZE = 15; 

// パス履歴
vector<pair<int, int>> current_delivery_path_edges;

// 座標
struct Point { int x, y; };
vector<Point> coords;

bool is_shop(int v) { return v < K; }
bool is_tree(int v) { return v >= K; }
char get_tree_color(int v) { return node_colors[v]; }

// 探索結果
struct HopResult {
    int target_node;
    int dist;
    long long dirt_cost;
    string generated_str;
    vector<pair<int, int>> path_edges;
    int first_move; 
};

// ---------------------------------------------------------
// 単発ダイクストラ探索 (高速化版)
// ---------------------------------------------------------
// context_delivered: このシミュレーション内ですでに納品済みのリスト(vectorで高速化)
vector<HopResult> search_next_shops(int start_node, int forbidden_node, const string& start_cone, 
                                    const vector<pair<int, string>>& context_delivered) {
    using P = pair<long long, int>;
    priority_queue<P, vector<P>, greater<P>> pq;
    
    // 【高速化】static配列でメモリ確保コスト削減
    static long long min_cost[105];
    static int parent[105];
    
    // 初期化 (N <= 100 なのでループで十分高速)
    for(int i=0; i<N; ++i) min_cost[i] = 2e18;
    
    min_cost[start_node] = 0;
    parent[start_node] = -1;
    pq.push({0, start_node});
    
    vector<int> found_shops;
    found_shops.reserve(K);

    while(!pq.empty()){
        auto [c, u] = pq.top(); pq.pop();
        if(c > min_cost[u]) continue;
        
        // ショップ到達で打ち切り
        if(is_shop(u) && u != start_node) {
            found_shops.push_back(u);
            continue;
        }
        
        for(int v : adj[u]) {
            if(u == start_node && v == forbidden_node) continue;
            
            // コスト計算: 距離優先だが、汚れも少し強めに意識
            int u_idx = (u < v) ? u : v;
            int v_idx = (u < v) ? v : u;
            long long dirt = global_edge_visits[u_idx][v_idx];
            
            // 距離1あたり 10^9、汚れ1あたり 1000 の重み
            // これにより、距離が同じなら確実に汚れが少ない方を選ぶ
            long long edge_weight = 1000000000LL + dirt * 1000; 
            
            if(min_cost[v] > min_cost[u] + edge_weight) {
                min_cost[v] = min_cost[u] + edge_weight;
                parent[v] = u;
                pq.push({min_cost[v], v});
            }
        }
    }
    
    vector<HopResult> results;
    results.reserve(found_shops.size());

    for(int shop : found_shops) {
        vector<int> path_nodes;
        int curr = shop;
        while(curr != -1) {
            path_nodes.push_back(curr);
            curr = parent[curr];
        }
        reverse(path_nodes.begin(), path_nodes.end());
        
        string s = start_cone;
        vector<pair<int, int>> edges;
        edges.reserve(path_nodes.size());
        
        int first_move = -1;
        if(path_nodes.size() > 1) first_move = path_nodes[1];
        
        long long total_dirt = 0;
        curr = path_nodes[0];
        for(size_t i=1; i<path_nodes.size(); ++i) {
            int next_node = path_nodes[i];
            if(is_tree(next_node)) s += get_tree_color(next_node);
            
            int u = (curr < next_node) ? curr : next_node;
            int v = (curr < next_node) ? next_node : curr;
            edges.push_back({u, v});
            total_dirt += global_edge_visits[u][v];
            
            curr = next_node;
        }
        
        if(s.empty()) continue;
        if(shop_inventory[shop].count(s)) continue;
        
        // 【高速化】vector線形探索 (要素数が少ないのでsetより速い)
        bool already_planned = false;
        for(const auto& p : context_delivered) {
            if(p.first == shop && p.second == s) {
                already_planned = true;
                break;
            }
        }
        if(already_planned) continue;
        
        results.push_back({shop, (int)edges.size(), total_dirt, s, edges, first_move});
    }
    
    return results;
}

// ---------------------------------------------------------
// D段階探索 (State管理付きビームサーチ)
// ---------------------------------------------------------
int find_best_multi_step_move(int start_node, int prev_node, int steps) {
    
    struct State {
        int current_node;
        int forbidden_node;
        long long total_score;
        int first_move;
        vector<pair<int, string>> delivered; // 【高速化】set -> vector
    };

    vector<State> beam;
    beam.reserve(100);
    beam.push_back({start_node, prev_node, 0, -1, {}});

    // 【高速化】フィルタリング用配列
    static long long best_score_at_node[105];

    for(int i = 0; i < steps; ++i) {
        vector<State> next_candidates;
        next_candidates.reserve(beam.size() * 10); // ざっくり予約
        
        for(const auto& st : beam) {
            string cone = (i == 0) ? current_cone : "";
            
            auto hops = search_next_shops(st.current_node, st.forbidden_node, cone, st.delivered);
            
            for(const auto& hop : hops) {
                // スコア計算
                long long step_score = (long long)hop.dist * 1000000000LL + hop.dirt_cost;
                long long new_total_score = st.total_score + step_score;
                
                int move = (i == 0) ? hop.first_move : st.first_move;
                
                int next_forbidden = -1;
                if(!hop.path_edges.empty()) {
                    pair<int,int> last_edge = hop.path_edges.back();
                    next_forbidden = (last_edge.first == hop.target_node) ? last_edge.second : last_edge.first;
                } else {
                    next_forbidden = st.current_node;
                }

                // 履歴更新
                auto next_delivered = st.delivered;
                next_delivered.push_back({hop.target_node, hop.generated_str});

                next_candidates.push_back({hop.target_node, next_forbidden, new_total_score, move, next_delivered});
            }
        }

        if(next_candidates.empty()) {
            if (i == 0) return -1; 
            break; 
        }

        // 【高速化】Mapを使わず配列でフィルタリング
        // 到達ノードごとにベストスコアを記録
        for(int j=0; j<N; ++j) best_score_at_node[j] = -1;
        
        // 1回走査して各ノードのベストスコアを確定
        for(const auto& cand : next_candidates) {
            int u = cand.current_node;
            if(best_score_at_node[u] == -1 || cand.total_score < best_score_at_node[u]) {
                best_score_at_node[u] = cand.total_score;
            }
        }

        // ベストスコアを持つ候補だけ抽出
        vector<State> next_beam;
        next_beam.reserve(N);
        
        // 念のため同じノードで同じスコアの重複を防ぐため、採用済みフラグも併用
        static bool node_added[105];
        for(int j=0; j<N; ++j) node_added[j] = false;

        // スコアが良い順に並べることで、同じベストスコアなら最初のを採用するようにする
        sort(next_candidates.begin(), next_candidates.end(), [](const State& a, const State& b){
            return a.total_score < b.total_score;
        });

        for(const auto& cand : next_candidates) {
            int u = cand.current_node;
            if(cand.total_score == best_score_at_node[u] && !node_added[u]) {
                next_beam.push_back(cand);
                node_added[u] = true;
            }
        }

        // ビーム幅調整 (少し広げる)
        int BEAM_WIDTH = 50; 
        if(next_beam.size() > BEAM_WIDTH) next_beam.resize(BEAM_WIDTH);
        
        beam = next_beam;
    }

    if(beam.empty()) return -1;
    return beam[0].first_move;
}

// ---------------------------------------------------------
// 広域脱出サーチ (Dijkstra + Taboo)
// ---------------------------------------------------------
int find_escape_move(int start_node, int forbidden_node, const string& cone_content) {
    auto is_taboo = [&](int u, int v) {
        for(auto& edge : recent_moves) {
            int u1 = min(u, v), v1 = max(u, v);
            int u2 = min(edge.first, edge.second), v2 = max(edge.first, edge.second);
            if(u1 == u2 && v1 == v2) return true;
        }
        return false;
    };

    using P = pair<long long, int>;
    priority_queue<P, vector<P>, greater<P>> pq;
    
    static long long dist[105];
    static int first_move[105];
    for(int i=0; i<N; ++i) dist[i] = 2e18;
    
    dist[start_node] = 0;
    first_move[start_node] = -1;
    pq.push({0, start_node});

    while(!pq.empty()){
        auto [d, u] = pq.top(); pq.pop();
        if(d > dist[u]) continue;
        if(d > 2000000000LL) continue; 

        for(int v : adj[u]) {
            if (u == start_node && v == forbidden_node) continue;
            
            int u_idx = (u < v) ? u : v;
            int v_idx = (u < v) ? v : u;
            
            // 汚れに対するペナルティを強化
            long long edge_cost = 100 + global_edge_visits[u_idx][v_idx] * 500; 
            if (is_taboo(u, v)) edge_cost += 100000000; 

            if(dist[v] > dist[u] + edge_cost) {
                dist[v] = dist[u] + edge_cost;
                if (u == start_node) first_move[v] = v;
                else first_move[v] = first_move[u];
                pq.push({dist[v], v});
            }
        }
    }

    int best_move = -1;
    double max_target_score = -1e18;

    for (int i = 0; i < K; ++i) { 
        if (i == start_node) continue; 
        if (dist[i] >= 1e18) continue;

        double score = 0;
        // 在庫少ない店ほど高得点
        score += (2000.0 - (double)shop_inventory[i].size()) * 100.0;
        
        // 有効納品
        if (!cone_content.empty() && !shop_inventory[i].count(cone_content)) {
            score += 500000.0; 
        }
        // 距離ペナルティ
        score -= (double)dist[i] * 1.0; 

        if (score > max_target_score) {
            max_target_score = score;
            best_move = first_move[i];
        }
    }

    if (best_move != -1) return best_move;
    
    vector<int> rnd_cands;
    for(int v : adj[start_node]) {
        if(v != forbidden_node) rnd_cands.push_back(v);
    }
    if(!rnd_cands.empty()) return rnd_cands[rng() % rnd_cands.size()];
    
    return -1;
}


void solve() {
    // 時間計測は競技では不要なら削除可、デバッグ用に残す
    auto start_clock = chrono::high_resolution_clock::now();

    cin >> N >> M >> K >> T;
    adj.resize(N);
    shop_inventory.resize(K);
    coords.resize(N);
    node_colors.assign(N, 'W');
    
    memset(shop_edge_visits, 0, sizeof(shop_edge_visits));
    memset(global_edge_visits, 0, sizeof(global_edge_visits));

    total_trees = N - K;

    for(int i=0; i<M; ++i){
        int u,v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for(int i=0; i<N; ++i) cin >> coords[i].x >> coords[i].y;

    for(int turn=0; turn<T; ++turn) {
        
        bool action2_performed = false;

        // Action 2 (Dispersal)
        // 変更なし
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

        // Action 1
        int next_node = -1;
        
        // -------------------------------------------------------
        // [設定] D段階探索
        // 高速化により D=5 でも十分間に合うはず
        // -------------------------------------------------------
        int LOOKAHEAD_STEPS = 5; 
        
        int best_move = find_best_multi_step_move(current_pos, prev_pos, LOOKAHEAD_STEPS);
        
        if (best_move != -1) {
            next_node = best_move;
        } else {
            next_node = find_escape_move(current_pos, prev_pos, current_cone);
        }

        cout << next_node << endl;
        
        int u = (current_pos < next_node) ? current_pos : next_node;
        int v = (current_pos < next_node) ? next_node : current_pos;
        current_delivery_path_edges.push_back({u, v});
        global_edge_visits[u][v]++;
        
        recent_moves.push_back({current_pos, next_node});
        if (recent_moves.size() > TABOO_SIZE) recent_moves.pop_front();

        prev_pos = current_pos;
        current_pos = next_node;
        
        if (is_tree(current_pos)) {
            current_cone += get_tree_color(current_pos);
        } else {
            if (!current_cone.empty()) {
                shop_inventory[current_pos].insert(current_cone);
            }
            current_cone = "";
            for (auto edge : current_delivery_path_edges) {
                shop_edge_visits[current_pos][edge.first][edge.second]++;
            }
            current_delivery_path_edges.clear();
        }
    }

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