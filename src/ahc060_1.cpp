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
vector<int> visit_counts;
int current_red_count = 0;
int total_trees = 0;

// 座標
struct Point { int x, y; };
vector<Point> coords;

bool is_shop(int v) { return v < K; }
bool is_tree(int v) { return v >= K; }
char get_tree_color(int v) { return node_colors[v]; }

struct TargetCandidate {
    int target_node;
    int first_move; 
    double score;   
    int distance;
    string generated_str;
};

// BFS評価関数
vector<TargetCandidate> evaluate_targets(char assume_current_color_override = 0) {
    vector<TargetCandidate> candidates;
    
    queue<int> q;
    q.push(current_pos);
    
    vector<int> dist(N, -1);
    vector<int> par(N, -1);
    vector<int> first_step(N, -1);
    
    dist[current_pos] = 0;
    
    // 短い文字列を優先するため、探索範囲を少し狭める
    // これにより遠くの複雑な文字列より、近くの確実な未回収を優先する
    const int DEPTH_LIMIT = 12; 

    while(!q.empty()) {
        int u = q.front();
        q.pop();

        if (dist[u] >= DEPTH_LIMIT) continue;

        if (is_shop(u) && u != current_pos) {
            // パス復元
            string s = current_cone;
            vector<int> path;
            int curr = u;
            while(curr != current_pos) {
                path.push_back(curr);
                curr = par[curr];
            }
            reverse(path.begin(), path.end());
            
            double path_congestion = 0;
            for(int v : path) {
                // 訪問回数のペナルティをさらに強化
                // 同じ道を3回以上通ったらもう価値はないとみなすレベル
                path_congestion += (visit_counts[v] * visit_counts[v]); 
            }

            for (int v : path) {
                if (is_tree(v)) {
                    if (v == current_pos && assume_current_color_override != 0) {
                        s += assume_current_color_override;
                    } else {
                        s += get_tree_color(v);
                    }
                }
            }

            double val = 0.0;
            bool is_new = (shop_inventory[u].find(s) == shop_inventory[u].end());
            
            if (is_new) {
                // 基本点
                double base_score = 10000.0;
                
                // 【修正1】長さボーナス撤廃 -> 短いパス優先
                // 距離が短いほど分母が小さくなりスコアが高くなる。
                // これにより「近くの未回収文字列」を最優先する。
                double dist_penalty = (double)dist[u]; 
                
                // 混雑ペナルティ（既知パス排除）
                double congestion_penalty = 1.0 + path_congestion * 5.0;

                val = base_score / (dist_penalty + 1.0) / congestion_penalty;
                
                // 長すぎる文字列（例えば7文字以上）は、まだ時期尚早としてスコアを下げる
                // これにより、まずは1~4文字程度を埋める動きになる
                if (s.length() > 6) {
                    val *= 0.1;
                }

            } else {
                // 既知の文字列は徹底的にスコアを下げる
                // ただし0にすると移動先がなくて困るので、極小値を残す
                val = 0.0001 / (dist[u] + 1.0);
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
    visit_counts.assign(N, 0);
    total_trees = N - K;

    for(int i=0; i<M; ++i){
        int u,v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for(int i=0; i<N; ++i) cin >> coords[i].x >> coords[i].y;

    for(int turn=0; turn<T; ++turn) {
        
        visit_counts[current_pos]++;
        
        bool action2_performed = false;
        
        // --- Action 2 (色変更) の制御 ---
        
        if (is_tree(current_pos) && get_tree_color(current_pos) == 'W') {
            
            // 【修正2】赤色キャップ（総量規制）
            // ターンの進行度に合わせて、許可される赤色の最大数を決める
            // 最初は0、最後は全木数になるようにリニアに増加
            double progress = (double)turn / T;
            int max_allowed_reds = (int)(progress * total_trees);
            
            // まだ赤に変える余裕がある場合のみ検討
            if (current_red_count < max_allowed_reds) {
                
                auto cands_W = evaluate_targets('W');
                auto cands_R = evaluate_targets('R');
                
                double best_score_W = cands_W.empty() ? 0.0 : cands_W[0].score;
                double best_score_R = cands_R.empty() ? 0.0 : cands_R[0].score;

                // 新規性が高い（スコアが高い）場合に変更
                // ただし、Wでも十分稼げるならWを温存する
                bool should_change = false;

                // 序盤(Wが多い): Rにするハードルは高い (劇的に良くなるなら変える)
                // 中盤: ハードルを下げる
                // 終盤: ほぼ無条件で変えていく
                
                double threshold = 2.0; // 基本はWの2倍の価値が必要
                if (progress > 0.5) threshold = 1.2;
                if (progress > 0.8) threshold = 0.8; // 終盤はWより悪くても変える（Wはもう不要）

                if (best_score_R > best_score_W * threshold + 100.0) {
                    should_change = true;
                }
                
                // 「詰み」回避: Wだとスコアがほぼ0だが、Rなら少しでも稼げる場合
                if (best_score_W < 1.0 && best_score_R > 100.0) {
                    should_change = true;
                }

                if (should_change) {
                     cout << -1 << endl;
                     node_colors[current_pos] = 'R';
                     current_red_count++;
                     action2_performed = true;
                }
            }
        }

        if (action2_performed) continue;

        // --- Action 1 (移動) ---
        
        auto candidates = evaluate_targets(0);
        int next_node = -1;

        // 【修正3】有効な候補がない場合の強力なランダムムーブ
        // スコアが極小（既知のものしかない）場合、
        // 「最も訪問していない」かつ「現在地から離れる方向」へ移動する
        if (candidates.empty() || candidates[0].score < 1.0) {
            
            vector<pair<int, int>> neighbors;
            for(int v : adj[current_pos]) {
                if(v != prev_pos) {
                    neighbors.push_back({visit_counts[v], v});
                }
            }
            sort(neighbors.begin(), neighbors.end()); // 訪問回数昇順
            
            if(!neighbors.empty()) {
                // 訪問回数が最も少ないものの中からランダム
                int min_v = neighbors[0].first;
                vector<int> best_neighbors;
                for(auto p : neighbors) {
                    if(p.first == min_v) best_neighbors.push_back(p.second);
                    else if(best_neighbors.size() < 2) best_neighbors.push_back(p.second); // 少し多いやつも候補に
                }
                next_node = best_neighbors[rng() % best_neighbors.size()];
            } else {
                 if (!adj[current_pos].empty()) next_node = adj[current_pos][0];
            }
        } else {
            // Softmax選択（ただしトップの優位性を高める）
            int selection_width = min((int)candidates.size(), 3);
            vector<double> weights;
            double total_weight = 0;
            
            for(int i=0; i<selection_width; ++i) {
                // スコアの3乗くらいで重み付けし、良い手をより強く選ぶ
                // しかし確率的ゆらぎは残す
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