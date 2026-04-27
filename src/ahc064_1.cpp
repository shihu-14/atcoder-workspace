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

    vector<vector<int>> D(R);
    vector<vector<int>> S(R);
    rep(i, R) {
        rep(j, 10) {
            int y;
            cin >> y;
            D[i].push_back(y);
        }
    }

    vector<vector<string>> all_turns;

    // 内部シミュレータ兼出力記録
    // D線は「末尾(end)」で出し入れ、S線は「先頭(begin)」で出し入れするよう修正
    auto do_turn = [&](vector<tuple<int, int, int, int>> moves) {
        if (moves.empty()) return;
        vector<string> turn_strs;
        for(auto [type, i, j, k] : moves) {
            if (type == 0) {
                // D[i] の末尾から S[j] の先頭へ
                vector<int> block(D[i].end() - k, D[i].end());
                D[i].erase(D[i].end() - k, D[i].end());
                S[j].insert(S[j].begin(), block.begin(), block.end());
            } else {
                // S[j] の先頭から D[i] の末尾へ
                vector<int> block(S[j].begin(), S[j].begin() + k);
                S[j].erase(S[j].begin(), S[j].begin() + k);
                D[i].insert(D[i].end(), block.begin(), block.end());
            }
            turn_strs.push_back(to_string(type) + " " + to_string(i) + " " + to_string(j) + " " + to_string(k));
        }
        all_turns.push_back(turn_strs);
    };

    // フェーズ1: 全車両を目の前の待避線(S線)へ移動 (平行移動なので1ターンで同時実行)
    vector<tuple<int,int,int,int>> phase1;
    rep(i, R) {
        phase1.push_back({0, i, i, 10});
    }
    do_turn(phase1);

    // フェーズ2: 車両0から99までを順番に探し、D線へ配置する
    rep(X, 100) {
        int s_idx = -1;
        int pos_in_s = -1;
        
        // ターゲットXの現在の位置(待避線のどこにあるか)を探索
        // S[i][0] が先頭(出発線側)
        rep(i, R) {
            rep(p, S[i].size()) {
                if (S[i][p] == X) {
                    s_idx = i;
                    pos_in_s = p;
                    break;
                }
            }
            if (s_idx != -1) break;
        }

        // Xの上に乗っている障害物の数 (先頭からの距離)
        int depth = pos_in_s;

        // Xが先頭に来るまで、上にある障害物を別のS線に退避させる
        while (depth > 0) {
            // 移動先のS線 (自身以外で最も空き容量が大きいもの)
            int t_idx = -1;
            int max_s_free = -1;
            rep(i, R) {
                if (i == s_idx) continue;
                int free_sp = 20 - S[i].size();
                if (free_sp > max_s_free) {
                    max_s_free = free_sp;
                    t_idx = i;
                }
            }

            // 一時経由するD線 (最も空き容量が大きいもの)
            int d_idx = -1;
            int max_d_free = -1;
            rep(i, R) {
                int free_sp = 15 - D[i].size();
                if (free_sp > max_d_free) {
                    max_d_free = free_sp;
                    d_idx = i;
                }
            }

            // 一度に動かせる安全なブロックサイズ
            int k = min({(int)depth, max_s_free, max_d_free});

            // 障害物の退避 (Sの先頭 -> Dの末尾 -> 別のSの先頭)
            do_turn({{1, d_idx, s_idx, k}}); // Type 1: S -> D
            do_turn({{0, d_idx, t_idx, k}}); // Type 0: D -> S
            
            depth -= k;
        }

        // 目的のD線へXを移動
        int target_d = X / 10;
        do_turn({{1, target_d, s_idx, 1}});
    }

    // 出力
    cout << all_turns.size() << "\n";
    for(const auto& turn_strs : all_turns) {
        cout << turn_strs.size() << "\n";
        for(const auto& s : turn_strs) {
            cout << s << "\n";
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