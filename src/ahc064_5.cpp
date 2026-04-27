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

void solve()
{
    int R;
    if (!(cin >> R)) return;

    // シミュレータの内部状態
    vector<vector<int>> cur_D(R);
    vector<vector<int>> cur_S(R);
    rep(i, R) {
        rep(j, 10) {
            int y;
            cin >> y;
            cur_D[i].push_back(y);
        }
    }

    // デバッグ用統計情報
    int phase1_moves = 0;
    int surface_collect_count = 0;
    int dig_count = 0;
    int dig_blocks_total = 0;
    int stack_bonus_apply_count = 0;

    // ステップ1: 直列な操作リストの生成
    vector<tuple<int, int, int, int>> raw_moves;

    auto add_move = [&](int type, int i, int j, int k) {
        if (k <= 0) return;
        if (type == 0) { // D[i] 末尾 -> S[j] 先頭
            vector<int> block(cur_D[i].end() - k, cur_D[i].end());
            cur_D[i].erase(cur_D[i].end() - k, cur_D[i].end());
            cur_S[j].insert(cur_S[j].begin(), block.begin(), block.end());
        } else { // S[j] 先頭 -> D[i] 末尾
            vector<int> block(cur_S[j].begin(), cur_S[j].begin() + k);
            cur_S[j].erase(cur_S[j].begin(), cur_S[j].begin() + k);
            cur_D[i].insert(cur_D[i].end(), block.begin(), block.end());
        }
        raw_moves.push_back({type, i, j, k});
    };

    // フェーズ1: D線からS線への分散退避 (初期展開の最適化)
    // 1両ずつ、目的地に近く、かつスタック原則を満たすS線へ移動させる
    while(true) {
        bool any_left = false;
        rep(i, R) {
            if(!cur_D[i].empty()) {
                any_left = true;
                break;
            }
        }
        if(!any_left) break;

        rep(i, R) {
            if(cur_D[i].empty()) continue;
            
            int move_id = cur_D[i].back();
            int target_r = move_id / 10;
            
            int best_s = -1;
            int max_score = -INF;
            rep(j, R) {
                if (cur_S[j].size() >= 20) continue;
                
                int score = 1000;
                score -= abs((int)j - target_r) * 10; // 目的地に近いほど高得点
                
                if (!cur_S[j].empty()) {
                    int top_id = cur_S[j][0];
                    int top_target = top_id / 10;
                    
                    if (target_r == top_target) score += 50;
                    
                    // スタック原則: 小さいIDが上になるように積む
                    if (move_id < top_id) {
                        score += 100;
                    } else {
                        score -= 50;
                    }
                } else {
                    score += 20; // 空のレーン
                }
                
                if (score > max_score) {
                    max_score = score;
                    best_s = (int)j;
                }
            }
            add_move(0, i, best_s, 1);
            phase1_moves++;
        }
    }

    // フェーズ2: 各D線の要求リストを管理し、貪欲に配置
    vector<int> req(R);
    rep(i, R) req[i] = 10 * i; // 各D線の最初の要求ID

    while (true) {
        bool moved = false;

        // 1. 表面回収
        rep(i, R) {
            if (cur_S[i].empty()) continue;
            int top = cur_S[i][0];
            int target_r = top / 10;
            
            if (req[target_r] == top) {
                add_move(1, target_r, (int)i, 1); // S -> D
                req[target_r]++;
                moved = true;
                surface_collect_count++;
            }
        }
        
        if (moved) continue;

        // 終了判定
        bool all_done = true;
        rep(r, R) {
            if (req[r] < 10 * r + 10) {
                all_done = false;
                break;
            }
        }
        if (all_done) break;

        // 2. 掘り起こし
        int best_s_idx = -1;
        int min_depth = INF;

        rep(r, R) {
            if (req[r] >= 10 * r + 10) continue;
            int want = req[r];
            
            rep(i, R) {
                rep(p, cur_S[i].size()) {
                    if (cur_S[i][p] == want) {
                        if (p < min_depth) {
                            min_depth = (int)p;
                            best_s_idx = (int)i;
                        }
                        break; 
                    }
                }
            }
        }

        dig_count++;

        // 掘り起こし実行: 目的車両の上に乗っている障害物を退避
        int obstacle_top_id = cur_S[best_s_idx][0];
        int target_d_for_obs = obstacle_top_id / 10;

        int best_t_idx = -1;
        int max_t_score = -INF;

        // 退避先のS線をスタック原則に基づきスコアで選択
        rep(i, R) {
            if (i == best_s_idx) continue;
            int free_sp = 20 - (int)cur_S[i].size();
            if (free_sp == 0) continue;

            int score = free_sp * 10 - abs((int)i - target_d_for_obs) * 10;

            if (!cur_S[i].empty()) {
                int top_id = cur_S[i][0];
                int top_target = top_id / 10;
                
                if (top_target == target_d_for_obs) score += 50;
                
                // スタック原則 (小さいIDが上になるように)
                if (obstacle_top_id < top_id) {
                    score += 100;
                } else {
                    score -= 50;
                }
            } else {
                score += 20;
            }

            if (score > max_t_score) {
                max_t_score = score;
                best_t_idx = (int)i;
            }
        }

        if (!cur_S[best_t_idx].empty()) {
            if (obstacle_top_id < cur_S[best_t_idx][0]) {
                stack_bonus_apply_count++;
            }
        }

        int best_d_idx = -1;
        int max_d_score = -INF;

        // 経由する一時D線をスコアで選択
        rep(i, R) {
            int free_sp = 15 - (int)cur_D[i].size();
            if (free_sp == 0) continue;

            int score = free_sp * 100 - (abs((int)i - best_s_idx) + abs((int)i - best_t_idx)) * 5;

            if (score > max_d_score) {
                max_d_score = score;
                best_d_idx = (int)i;
            }
        }

        int free_t = 20 - (int)cur_S[best_t_idx].size();
        int free_d = 15 - (int)cur_D[best_d_idx].size();
        int k = min({min_depth, free_t, free_d});

        dig_blocks_total += k;

        add_move(1, best_d_idx, best_s_idx, k); // S -> 一時D
        add_move(0, best_d_idx, best_t_idx, k); // 一時D -> 退避先S
    }

    // ステップ3: スケジューラによるターンマージ (並列化)
    vector<vector<tuple<int,int,int,int>>> turns;
    vector<int> D_last(R, -1);
    vector<int> S_last(R, -1);

    for (auto [type, i, j, k] : raw_moves) {
        int t = max({0, D_last[i] + 1, S_last[j] + 1});
        
        while (true) {
            if (t >= (int)turns.size()) {
                turns.push_back({});
            }
            
            bool can_add = true;
            for (auto [t_type, t_i, t_j, t_k] : turns[t]) {
                if (i == t_i || j == t_j) {
                    can_add = false; break;
                }
                if ((t_i - i) * (t_j - j) < 0) {
                    can_add = false; break;
                }
            }
            
            if (can_add) {
                turns[t].push_back({type, i, j, k});
                D_last[i] = t;
                S_last[j] = t;
                break;
            } else {
                t++;
            }
        }
    }

    // デバッグ情報の出力 (標準エラー出力)
    cerr << "=== Debug Information ===\n";
    cerr << "Phase 1 Distribute Moves: " << phase1_moves << "\n";
    cerr << "Surface Collect Count   : " << surface_collect_count << " / 100\n";
    cerr << "Digging Count           : " << dig_count << "\n";
    cerr << "Digging Blocks Total    : " << dig_blocks_total << "\n";
    cerr << "Stack Bonus Applied     : " << stack_bonus_apply_count << "\n";
    cerr << "Raw Moves (Serial)      : " << raw_moves.size() << "\n";
    cerr << "Merged Turns (Parallel) : " << turns.size() << "\n";
    cerr << "Compression Ratio       : " << fixed << setprecision(2) << (double)turns.size() / raw_moves.size() * 100.0 << " %\n";
    cerr << "=========================\n";

    // 結果出力 (標準出力)
    cout << turns.size() << "\n";
    rep(t, turns.size()) {
        cout << turns[t].size() << "\n";
        for (auto [type, i, j, k] : turns[t]) {
            cout << type << " " << i << " " << j << " " << k << "\n";
        }
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