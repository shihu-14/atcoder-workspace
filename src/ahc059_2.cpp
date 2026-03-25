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
using namespace atcoder;

using namespace std;

// using mint = modint998244353;
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
};

// 状態管理
vector<Node> tree; // tree[id] がペアidの情報。tree[M]をルートとする。
int root_id;

// デバッグ用グローバル変数
long long debug_iter_count = 0;
long long debug_update_count = 0;

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
    Point curr = get_start_pos(u);
    
    for (int child : tree[u].children) {
        Point next = get_start_pos(child);
        cost += dist(curr, next);
        cost += calc_subtree_cost(child); // 再帰
        curr = get_end_pos(child);
    }
    
    if (u != root_id) {
        cost += dist(curr, get_end_pos(u));
    }
    return cost;
}

// 全体のスコア計算 (初期計算用)
int calc_total_score() {
    return calc_subtree_cost(root_id);
}

// --- 差分計算のためのヘルパー関数 ---
// ノードuにおける「Start(u) -> 子1 -> 子2... -> End(u)」の配線コストだけを計算
// 再帰はせず、自身の直接の子との接続のみを見る (O(子ノード数))
int calc_local_wiring_cost(int u) {
    int cost = 0;
    Point curr = get_start_pos(u);
    
    for (int child : tree[u].children) {
        Point next = get_start_pos(child);
        cost += dist(curr, next);
        curr = get_end_pos(child);
    }
    
    // ルート以外は最後に自分のEndに戻るコストが必要
    // ルートは (0,0) に戻る必要はない（問題仕様）
    if (u != root_id) {
        cost += dist(curr, get_end_pos(u));
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

// 初期解構築: 距離が遠い順に貪欲挿入
void build_initial_solution() {
    vector<pair<int, int>> pairs_by_dist;
    rep(i, M) {
        int d = dist(card_pos[i][0], card_pos[i][1]);
        pairs_by_dist.push_back({d, (int)i});
    }
    sort(rng(pairs_by_dist), greater<pair<int, int>>());

    root_id = M;
    tree.assign(M + 1, Node());
    tree[root_id] = {root_id, -1, false, {}};
    rep(i, M) {
        tree[i].id = i;
        tree[i].is_flipped = false; 
    }

    // 1つずつ挿入 (簡易版Best Insertion)
    for (auto p : pairs_by_dist) {
        int target_id = p.second;
        
        int best_parent = -1;
        int best_idx = -1;
        int min_increase = INF;
        bool best_flip = false;

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
                Point prev_p = (i == 0) ? get_start_pos(u) : get_end_pos(tree[u].children[i-1]);
                Point next_p = (i == sz) ? ((u == root_id) ? get_start_pos(u) : get_end_pos(u)) : get_start_pos(tree[u].children[i]);
                
                int current_link_cost = 0;
                if (u != root_id || i < sz) {
                     current_link_cost = dist(prev_p, next_p);
                }

                for (int f = 0; f < 2; ++f) {
                    bool flip = (f == 1);
                    Point t_start = card_pos[target_id][flip ? 1 : 0];
                    Point t_end = card_pos[target_id][flip ? 0 : 1];
                    
                    int increase = dist(prev_p, t_start) + d_pair;
                    if (u != root_id || i < sz) {
                        increase += dist(t_end, next_p);
                        increase -= current_link_cost;
                    }
                    
                    if (increase < min_increase) {
                        min_increase = increase;
                        best_parent = u;
                        best_idx = i;
                        best_flip = flip;
                    }
                }
            }
        }

        tree[target_id].parent = best_parent;
        tree[target_id].is_flipped = best_flip;
        tree[target_id].children.clear();
        tree[best_parent].children.insert(tree[best_parent].children.begin() + best_idx, target_id);
    }
}

// --- Hill Climbing (差分計算版) ---

mt19937 engine(12345);

void hill_climbing() {
    auto start_time = chrono::steady_clock::now();
    
    // 最初に一度だけ全計算を行い、以降は差分で更新する
    int current_score = calc_total_score();
    int best_score = current_score;
    vector<Node> best_tree = tree;

    int iter = 0;
    debug_iter_count = 0;
    debug_update_count = 0;
    
    while (true) {
        debug_iter_count++;
        iter++;
        if ((iter & 0xFF) == 0) {
            auto now = chrono::steady_clock::now();
            double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
            if (elapsed > 1850) break;
        }

        int type = engine() % 100;
        
        if (type < 50) { // Smart Re-grafting (50%)
            // 1. ノードを取り外す
            int target = engine() % M;
            int old_parent = tree[target].parent;
            
            // 取り外し前の親の配線コストを計算
            int cost_before_removal = calc_local_wiring_cost(old_parent);

            // 木構造から一時的に削除
            auto& siblings = tree[old_parent].children;
            auto it = find(rng(siblings), target);
            int old_idx = distance(siblings.begin(), it);
            siblings.erase(it);

            // 取り外し後の親の配線コストを計算
            int cost_after_removal = calc_local_wiring_cost(old_parent);
            
            // 現在のスコアから、取り外しによる変化分を反映
            int score_after_removal = current_score + (cost_after_removal - cost_before_removal);

            // 2. 最適な挿入場所を探す
            int best_p = -1;
            int best_i = -1;
            int min_incr = INF; // 挿入による増分コストの最小値
            
            // 候補親を選ぶ (自分自身とその子孫以外)
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

            // 探索（ここはベクトルをいじらず、座標計算だけで行う）
            for (int p : parent_candidates) {
                int sz = tree[p].children.size();
                for (int i = 0; i <= sz; ++i) {
                    Point prev_p = (i == 0) ? get_start_pos(p) : get_end_pos(tree[p].children[i-1]);
                    Point next_p = (i == sz) ? ((p == root_id) ? get_start_pos(p) : get_end_pos(p)) : get_start_pos(tree[p].children[i]);
                    
                    int current_link_cost = 0;
                    if (p != root_id || i < sz) {
                        current_link_cost = dist(prev_p, next_p);
                    }
                    
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
            
            // 3. 採用判定
            int new_total_score = score_after_removal + min_incr;

            if (new_total_score < current_score) {
                // 採用: 実際に挿入する
                tree[target].parent = best_p;
                tree[best_p].children.insert(tree[best_p].children.begin() + best_i, target);
                
                current_score = new_total_score;
                debug_update_count++;
                if (current_score < best_score) {
                    best_score = current_score;
                    best_tree = tree;
                }
            } else {
                // 不採用: 元に戻す (Revert)
                tree[target].parent = old_parent;
                tree[old_parent].children.insert(tree[old_parent].children.begin() + old_idx, target);
                // current_score は変更していないのでそのままでOK
            }

        } else if (type < 90) { // Flip (40%)
            int target = engine() % M;
            int parent = tree[target].parent;
            
            // 変更前のコスト（自分＋親）
            int old_self_cost = calc_local_wiring_cost(target);
            int old_parent_cost = calc_local_wiring_cost(parent);

            // Flip実行
            bool old_flip = tree[target].is_flipped;
            tree[target].is_flipped = !old_flip;
            
            // 変更後のコスト
            int new_self_cost = calc_local_wiring_cost(target);
            int new_parent_cost = calc_local_wiring_cost(parent);
            
            // 差分計算
            int delta = (new_self_cost + new_parent_cost) - (old_self_cost + old_parent_cost);
            int new_score = current_score + delta;

            if (new_score < current_score) {
                current_score = new_score;
                debug_update_count++;
                if (current_score < best_score) {
                    best_score = current_score;
                    best_tree = tree;
                }
            } else {
                tree[target].is_flipped = old_flip; // Revert
            }

        } else { // Swap Siblings (10%)
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
                
                if (i1 != i2) {
                    // 変更前の親のコスト
                    int old_p_cost = calc_local_wiring_cost(p);

                    // Swap
                    swap(tree[p].children[i1], tree[p].children[i2]);
                    
                    // 変更後の親のコスト
                    int new_p_cost = calc_local_wiring_cost(p);

                    // 差分計算
                    int delta = new_p_cost - old_p_cost;
                    int new_score = current_score + delta;

                    if (new_score < current_score) {
                        current_score = new_score;
                        debug_update_count++;
                        if (current_score < best_score) {
                            best_score = current_score;
                            best_tree = tree;
                        }
                    } else {
                        swap(tree[p].children[i1], tree[p].children[i2]); // Revert
                    }
                }
            }
        }
    }
    
    // ベスト解を復元
    tree = best_tree;

    // デバッグ情報出力
    auto end_time = chrono::steady_clock::now();
    double debug_total_time_ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    cerr << "debug_iter_count: " << debug_iter_count << endl;
    cerr << "debug_update_count: " << debug_update_count << endl;
    cerr << "debug_total_time_ms: " << debug_total_time_ms << endl;
    cerr << "best_score: " << best_score << endl;
}

// --- Output Generation ---

string output_moves;
Point current_pos;

void move_to(Point target, bool pick) {
    while (current_pos.r < target.r) { output_moves += 'D'; current_pos.r++; }
    while (current_pos.r > target.r) { output_moves += 'U'; current_pos.r--; }
    while (current_pos.c < target.c) { output_moves += 'R'; current_pos.c++; }
    while (current_pos.c > target.c) { output_moves += 'L'; current_pos.c--; }
    if (pick) output_moves += 'Z';
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

void solve()
{
    // 入力
    int n_in;
    if (!(cin >> n_in)) return;
    // N = n_in; // 固定20
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

    // 初期解
    build_initial_solution();

    // 山登り (高速化版)
    hill_climbing();

    // 出力生成
    current_pos = {0, 0};
    output_moves = "";
    dfs_solve(root_id);
    
    // Output format
    rep(i, output_moves.size()) {
        cout << output_moves[i] << "\n";
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while(t--)
    {
        solve();
    }
    return 0;
}