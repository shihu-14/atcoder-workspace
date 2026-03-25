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
#include <atcoder/all>

using namespace std;
using namespace atcoder;


mt19937 rng(42);


int N, M, K, T;
vector<vector<int>> adj;
vector<set<string>> shop_inventory;
string current_cone = "";
int current_pos = 0;
int prev_pos = -1;
vector<char> node_colors;


int current_red_count = 0;
int total_trees = 0;



vector<map<pair<int, int>, int>> shop_edge_visits;


vector<pair<int, int>> current_delivery_path_edges;


struct Point { int x, y; };
vector<Point> coords;

bool is_shop(int v) { return v < K; }
bool is_tree(int v) { return v >= K; }
char get_tree_color(int v) { return node_colors[v]; }


pair<int, int> get_edge_key(int u, int v) {
    if (u > v) swap(u, v);
    return {u, v};
}

struct TargetCandidate {
    int target_node;
    int first_move; 
    double score;   
    int distance;
    string generated_str;
};

vector<TargetCandidate> evaluate_targets() {
    vector<TargetCandidate> candidates;
    
    queue<int> q;
    q.push(current_pos);
    
    vector<int> dist(N, -1);
    vector<int> par(N, -1);
    vector<int> first_step(N, -1);
    
    dist[current_pos] = 0;
    
    
    const int DEPTH_LIMIT = 12; 

    while(!q.empty()) {
        int u = q.front();
        q.pop();

        if (dist[u] >= DEPTH_LIMIT) continue;

        if (is_shop(u) && u != current_pos) {
            
            string s = current_cone;
            vector<int> path_nodes;
            int curr = u;
            
            
            double local_dirty_score = 0;

            while(curr != current_pos) {
                path_nodes.push_back(curr);
                int p = par[curr];
                local_dirty_score += shop_edge_visits[u][get_edge_key(curr, p)];
                curr = p;
            }
            reverse(path_nodes.begin(), path_nodes.end());
            
            
            for (int v : path_nodes) {
                if (is_tree(v)) {
                    s += get_tree_color(v);
                }
            }

            double val = 0.0;
            bool is_new = (shop_inventory[u].find(s) == shop_inventory[u].end());
            
            if (is_new) {
                double base_score = 1000.0;
                double dirty_penalty = 1.0 + (local_dirty_score * 2.0); 
                double dist_penalty = 1.0 + dist[u];

                val = base_score / (dist_penalty * dirty_penalty);
                
                if (s.length() > 6) val *= 0.5;

            } else {
                val = 0.001 / (dist[u] + 1.0);
            }

            candidates.push_back({u, first_step[u], val, dist[u], s});
        }

        if (is_shop(u) && u != current_pos) continue;

        for (int v : adj[u]) {
            if (u == current_pos && v == prev_pos) continue;

            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                par[v] = u;
                if (u == current_pos) first_step[v] = v;
                else first_step[v] = first_step[u];
                q.push(v);
            }
        }
    }
    
    sort(candidates.begin(), candidates.end(), [](const TargetCandidate& a, const TargetCandidate& b){
        return a.score > b.score;
    });

    return candidates;
}

void solve() {
    cin >> N >> M >> K >> T;
    adj.resize(N);
    shop_inventory.resize(K);
    coords.resize(N);
    node_colors.assign(N, 'W');
    
    shop_edge_visits.resize(K);
    current_delivery_path_edges.clear();
    
    total_trees = N - K;

    for(int i=0; i<M; ++i){
        int u,v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for(int i=0; i<N; ++i) cin >> coords[i].x >> coords[i].y;

    for(int turn=0; turn<T; ++turn) {
        
        bool action2_performed = false;

        
        long long target_reds = (long long)turn * total_trees / T;

        
        if (current_red_count < target_reds) {
            if (is_tree(current_pos) && get_tree_color(current_pos) == 'W') {
                
                
                int neighbors = 0;
                int red_neighbors = 0;

                for (int neighbor : adj[current_pos]) {
                    if (is_tree(neighbor)) {
                        neighbors++;
                        if (get_tree_color(neighbor) == 'R') {
                            red_neighbors++;
                        }
                    }
                }

                bool do_change = false;

                
                if (neighbors == 0) {
                    do_change = true;
                }
                else if ((double)red_neighbors / neighbors < 0.5) {
                    do_change = true;
                }

                if (!do_change) {
                    long long lag = target_reds - current_red_count;
                    if (lag > 5 || turn > T * 0.9) {
                        do_change = true;
                    }
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

        
        
        auto candidates = evaluate_targets();
        int next_node = -1;

        if (candidates.empty() || candidates[0].score < 0.1) {
            
            vector<int> valid_nexts;
            for(int v : adj[current_pos]) if(v != prev_pos) valid_nexts.push_back(v);
            
            if(!valid_nexts.empty()) {
                next_node = valid_nexts[rng() % valid_nexts.size()];
            } else {
                if (!adj[current_pos].empty()) next_node = adj[current_pos][0];
            }
        } else {
            
            int selection_width = min((int)candidates.size(), 3);
            vector<double> weights;
            double total_weight = 0;
            
            for(int i=0; i<selection_width; ++i) {
                double w = pow(candidates[i].score, 3.0);
                weights.push_back(w);
                total_weight += w;
            }

            double r = uniform_real_distribution<double>(0.0, total_weight)(rng);
            int selected_idx = 0;
            double current_sum = 0;
            for(int i=0; i<selection_width; ++i) {
                current_sum += weights[i];
                if (r <= current_sum) {
                    selected_idx = i;
                    break;
                }
            }
            next_node = candidates[selected_idx].first_move;
        }

        cout << next_node << endl;
        
        
        current_delivery_path_edges.push_back(get_edge_key(current_pos, next_node));

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
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}