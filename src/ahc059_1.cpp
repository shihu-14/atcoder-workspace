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

// ACLが使える環境なら有効化、なければコメントアウトのままで動作します
// #include <atcoder/all>
// using namespace atcoder;

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

// あるノード（部分木）の内部移動コストのみを計算する
// Start(u) -> Child1 -> ... -> ChildK -> End(u) の距離の総和
// ただし、Start(u)への流入、End(u)からの流出は含まない
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

// 全体のスコア計算
int calc_total_score() {
    return calc_subtree_cost(root_id);
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
    // ペアごとの距離を計算して降順ソート
    vector<pair<int, int>> pairs_by_dist;
    rep(i, M) {
        int d = dist(card_pos[i][0], card_pos[i][1]);
        pairs_by_dist.push_back({d, (int)i});
    }
    sort(rng(pairs_by_dist), greater<pair<int, int>>());

    // 木の初期化
    root_id = M;
    tree.assign(M + 1, Node());
    tree[root_id] = {root_id, -1, false, {}};
    rep(i, M) {
        tree[i].id = i;
        tree[i].is_flipped = false; 
    }

    // 1つずつ挿入
    for (auto p : pairs_by_dist) {
        int target_id = p.second;
        
        // 最適な挿入位置を探す (Best Insertion)
        int best_parent = -1;
        int best_idx = -1;
        int min_increase = INF;
        bool best_flip = false;

        // 木に含まれているノードを列挙するためのキュー
        queue<int> q;
        q.push(root_id);
        
        // 挿入候補のノードリスト
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
                     if (u != root_id || i < sz) {
                         current_link_cost = dist(prev_p, next_p);
                     }
                }

                // Flip 2パターン試す
                for (int f = 0; f < 2; ++f) {
                    bool flip = (f == 1);
                    Point t_start = card_pos[target_id][flip ? 1 : 0];
                    Point t_end = card_pos[target_id][flip ? 0 : 1];
                    
                    int increase = dist(prev_p, t_start) + d_pair; // target内部
                    
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

        // 決定した位置に挿入
        tree[target_id].parent = best_parent;
        tree[target_id].is_flipped = best_flip;
        tree[target_id].children.clear();
        tree[best_parent].children.insert(tree[best_parent].children.begin() + best_idx, target_id);
    }
}

// --- Hill Climbing ---

// グローバル乱数
mt19937 engine(12345);

void hill_climbing() {
    auto start_time = chrono::steady_clock::now();
    int current_score = calc_total_score();
    int best_score = current_score;
    vector<Node> best_tree = tree;

    int iter = 0;
    
    while (true) {
        iter++;
        if ((iter & 0xFF) == 0) {
            auto now = chrono::steady_clock::now();
            double elapsed = chrono::duration_cast<chrono::milliseconds>(now - start_time).count();
            if (elapsed > 1850) break;
        }

        // 近傍選択
        // 0: Smart Re-grafting (Move Subtree) (High freq)
        // 1: Flip (High freq)
        // 2: Swap Siblings (Low freq)
        
        int type = engine() % 100;
        
        if (type < 50) { // Smart Re-grafting (50%)
            // ランダムにノードを選ぶ (root以外)
            int target = engine() % M;
            int old_parent = tree[target].parent;
            
            // 現在の木から取り外す
            auto& siblings = tree[old_parent].children;
            auto it = find(rng(siblings), target);
            int old_idx = distance(siblings.begin(), it);
            siblings.erase(it);
            
            // 挿入先を探す
            int best_p = -1;
            int best_i = -1;
            int min_incr = INF; // 増分
            
            // 候補親を選ぶ
            vector<int> parent_candidates;
            parent_candidates.push_back(old_parent); // 元の親も候補
            rep(k, 30) { // ランダムに30個選ぶ
                int p = (engine() % (M + 1));
                if (p == M) p = root_id;
                if (p == target) continue;
                if (is_ancestor(target, p)) continue; // targetの子孫は親になれない
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
            
            // 移動実行
            tree[target].parent = best_p;
            tree[best_p].children.insert(tree[best_p].children.begin() + best_i, target);
            
            // スコア計算
            int new_score = calc_total_score();
            if (new_score < current_score) {
                current_score = new_score;
                if (current_score < best_score) {
                    best_score = current_score;
                    best_tree = tree;
                }
            } else {
                // 戻す (Revert)
                auto& new_sibs = tree[best_p].children;
                new_sibs.erase(new_sibs.begin() + best_i);
                
                // 元に戻す
                tree[target].parent = old_parent;
                tree[old_parent].children.insert(tree[old_parent].children.begin() + old_idx, target);
            }

        } else if (type < 90) { // Flip (40%)
            int target = engine() % M;
            
            bool old_flip = tree[target].is_flipped;
            tree[target].is_flipped = !old_flip;
            
            int new_score = calc_total_score();
            if (new_score < current_score) {
                current_score = new_score;
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
                    swap(tree[p].children[i1], tree[p].children[i2]);
                    
                    int new_score = calc_total_score();
                    if (new_score < current_score) {
                        current_score = new_score;
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
    cerr << iter << " iterations in hill climbing." << endl;
    
    // ベスト解を復元
    tree = best_tree;
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
    cin >> n_in;
    // N = n_in; // 固定20
    M = N * N / 2;
    
    vector<vector<int>> grid(N, vector<int>(N));
    card_pos.resize(M);
    // count変数をリネームしてshadowing回避（動作は元のままでも問題ないが念のため）
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

    // 山登り
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