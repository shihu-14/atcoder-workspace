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

// compressed suffix tree 
struct CST
{
    struct Node
    {
        int id;             
        int depth;          
        int parent;         
        vector<int> children;
        Node(int id, int depth, int parent) : id(id), depth(depth), parent(parent) {}
    };

    int n;
    string s;
    vector<Node> nodes;

    CST(const std::string& _s) : s(_s), n(_s.size())
    {
        build();
    }

private:
    void build()
    {
        auto sa = suffix_array(s);
        auto lcp = lcp_array(s, sa);

        nodes.reserve(2*n);
        nodes.emplace_back(0, 0, -1); 

        int v = 0;
        for (int i = 0; i < n; ++i)
        {
            int d = n - sa[i];
            // 新しい葉を追加
            if (nodes[v].depth < d)
            {
                int u = nodes.size();
                nodes.emplace_back(u, d, v);
                nodes[v].children.emplace_back(u);
                v = u;
            }
            if (i == n - 1) break;
            // lcpを参照して次のsuffixの追加位置を決定
            d = lcp[i];
            while (d < nodes[v].depth) v = nodes[v].parent;
            // v-->u-->old_child のパスを分割して新しい内部ノードuを挿入
            if (nodes[v].depth < d)
            {
                int u = nodes.size();
                nodes.emplace_back(u, d, v);
                int old_child = nodes[v].children.back();
                nodes[u].children.emplace_back(old_child);
                nodes[old_child].parent = u;
                nodes[v].children.back() = u;
                nodes[u].parent = v;
                v = u;
            }
        }
    }
};
// 参考 https://atcoder.jp/contests/abc433/submissions/71178201
/* testcase
  abc433_g
*/


void solve()
{
    string s; cin >> s;
    CST cst(s);
    auto dfs = [&](auto f, int u) -> int
    {
        for (auto v : cst.nodes[u].children)
        {
            int d = (cst.nodes[v].depth - cst.nodes[u].depth)%2;
            if (f(f, v)^d) return 1;
        }
        return 0;
    };

    if (dfs(dfs, 0)) cout << "Alice" << endl;
    else cout << "Bob" << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    cin >> t;
    while(t--)
    {
        solve();
    }
    return 0;
}