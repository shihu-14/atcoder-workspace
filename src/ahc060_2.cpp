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
map<pair<int, int>, int> edge_visit_counts; // 辺ごとの使用回数

// 座標
struct Point { int x, y; };
vector<Point> coords;

bool is_shop(int v) { return v < K; }
bool is_tree(int v) { return v >= K; }
char get_tree_color(int v) { return node_colors[v]; }

// 辺のキーを取得 (u < v に正規化)
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

// 探索関数
vector<TargetCandidate> evaluate_targets() {
    vector<TargetCandidate> candidates;
    
    queue<int> q;
    q.push(current_pos);
    
    vector<int> dist(N, -1);
    vector<int> par(N, -1);
    vector<int> first_step(N, -1);
    
    dist[current_pos] = 0;
    
    // 近場を優先するため、あまり深く探索しすぎない
    const int DEPTH_LIMIT = 10; 

    while(!q.empty()) {
        int u = q.front();
        q.pop();

        if (dist[u] >= DEPTH_LIMIT) continue;

        if (is_shop(u) && u != current_pos) {
            // パス復元 & コスト計算
            string s = current_cone;
            vector<int> path;
            int curr = u;
            
            // パス上の辺の使用回数合計（汚れ具合）
            double path_dirty_score = 0;

            while(curr != current_pos) {
                path.push_back(curr);
                int p = par[curr];
                // 辺の使用回数をペナルティとして加算
                path_dirty_score += edge_visit_counts[get_edge_key(curr, p)];
                curr = p;
            }
            reverse(path.begin(), path.end());
            
            // 文字列生成
            for (int v : path) {
                if (is_tree(v)) {
                    s += get_tree_color(v);
                }
            }

            double val = 0.0;
            bool is_new = (shop_inventory[u].find(s) == shop_inventory[u].end());
            
            if (is_new) {
                // 基本点
                double base_score = 1000.0;
                
                // 【重要】辺の使用回数ペナルティ
                // 何度も使われた辺を通るパスはスコアを激減させる
                // これにより「いつもと違う道」を強制的に選ばせる
                double dirty_penalty = 1.0 + (path_dirty_score * 2.0); 
                
                // 距離ペナルティ（近いほうが偉い -> 短い文字列を優先）
                double dist_penalty = 1.0 + dist[u];

                val = base_score / (dist_penalty * dirty_penalty);
                
                // 極端に長い文字列はまだ作らなくていい（偶然できたらラッキー程度）
                // 確実に短いものを埋めるため、長すぎるものは評価を下げる
                if (s.length() > 6) val *= 0.5;

            } else {
                // 既知の文字列の場合
                // ほぼ価値なしだが、完全に0にすると動けなくなるので極小値
                // 遠くの既知より近くの既知へ（リセットのため）
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
    
    // スコア降順ソート
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
    edge_visit_counts.clear();
    total_trees = N - K;

    for(int i=0; i<M; ++i){
        int u,v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        edge_visit_counts[{min(u,v), max(u,v)}] = 0;
    }
    for(int i=0; i<N; ++i) cin >> coords[i].x >> coords[i].y;

    for(int turn=0; turn<T; ++turn) {
        
        bool action2_performed = false;

        // --- Action 2 (色変更: 線形スケジュール) ---
        // 序盤から終盤にかけて、赤色の数が線形に増えるように制御する
        // 目標数 = (現在のターン / 総ターン) * 木の総数
        
        long long target_reds = (long long)turn * total_trees / T;

        if (current_red_count < target_reds) {
            // ノルマ未達の場合、現在地がWの木なら「即座に」変える
            // これにより、よく通るルート上の木から順に赤くなっていく
            if (is_tree(current_pos) && get_tree_color(current_pos) == 'W') {
                cout << -1 << endl;
                node_colors[current_pos] = 'R';
                current_red_count++;
                action2_performed = true;
            }
        }

        if (action2_performed) continue;

        // --- Action 1 (移動: 貪欲 + 辺ペナルティ) ---
        
        auto candidates = evaluate_targets();
        int next_node = -1;

        // 良い候補がない（全て既知 or 遠すぎる）場合
        if (candidates.empty() || candidates[0].score < 0.1) {
            // 「最も使われていない辺」を通って脱出を図る（未踏エリアへの誘導）
            vector<tuple<int, int>> neighbors;
            for(int v : adj[current_pos]) {
                if(v != prev_pos) {
                    int cost = edge_visit_counts[get_edge_key(current_pos, v)];
                    neighbors.emplace_back(cost, v);
                }
            }
            sort(neighbors.begin(), neighbors.end());
            
            if(!neighbors.empty()) {
                // コスト最小の辺を選択（複数あればランダム）
                vector<int> best_nexts;
                int min_cost = get<0>(neighbors[0]);
                for(auto p : neighbors) {
                    if(get<0>(p) == min_cost) best_nexts.push_back(get<1>(p));
                }
                next_node = best_nexts[rng() % best_nexts.size()];
            } else {
                if (!adj[current_pos].empty()) next_node = adj[current_pos][0];
            }
        } else {
            // 有効な候補がある場合、上位から確率的に選択
            // スコア差を強めに反映して、良い手を優先しつつも多少のバラつきを持たせる
            int selection_width = min((int)candidates.size(), 3);
            vector<double> weights;
            double total_weight = 0;
            
            for(int i=0; i<selection_width; ++i) {
                double w = pow(candidates[i].score, 3.0); // 3乗して差を強調
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

        // 移動実行
        cout << next_node << endl;
        
        // 辺の使用回数を更新
        edge_visit_counts[get_edge_key(current_pos, next_node)]++;

        prev_pos = current_pos;
        current_pos = next_node;
        
        if (is_tree(current_pos)) {
            current_cone += get_tree_color(current_pos);
        } else {
            shop_inventory[current_pos].insert(current_cone);
            current_cone = "";
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}