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

struct UnionFind {
    vector<int> d;
    UnionFind(int n=0): d(n,-1) {}
    int find(int x) {
        if(d[x] < 0) return x;
        return d[x] = find(d[x]);
    }
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    bool merge(int x, int y) { // if merge successed, return true
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (x < y) swap(x, y);
        d[x] += d[y];
        d[y] = x;
        return true;
    }
    int size(int x){
        return -d[find(x)];
    }
};

void solve()
{
    int n, m; cin >> n >> m;
    vector<T3> query;
    rep(i, m)
    {
        int u, v, c; cin >> u >> v >> c; u--, v--;
        query.emplace_back(u, v, c);
    }
    reverse(rng(query));

    UnionFind uf(n+1);
    vector<int> ans(n);
    for (auto [l, r, c] : query)
    {
        int x = uf.find(l);
        while(x <= r)
        {
            ans[x] = c;
            uf.merge(x, x+1);
            x = uf.find(x);
        }
    }
    printv(ans);
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


// #include <iostream>
// #include <iomanip>
// #include <fstream>
// #include <sstream>
// #include <cstdio>
// #include <cstdlib>
// #include <cmath>
// #include <cstring>
// #include <climits>
// #include <cfloat>
// #include <cassert>
// #include <ctime>
// #include <cctype>
// #include <cwctype>
// #include <cstdint>
// #include <type_traits>
// #include <initializer_list>
// #include <utility>
// #include <bitset>
// #include <vector>
// #include <deque>
// #include <list>
// #include <stack>
// #include <queue>
// #include <set>
// #include <map>
// #include <unordered_set>
// #include <unordered_map>
// #include <string>
// #include <algorithm>
// #include <functional>
// #include <numeric>
// #include <iterator>
// #include <tuple>
// #include <array>
// #include <new>
// #include <memory>
// #include <limits>
// #include <random>
// #include <exception>
// #include <stdexcept>
// #include <regex>
// #include <complex>
// #include <chrono>
// #include <future>
// #include <thread>
// #include <mutex>
// #include <atomic>
// #include <atcoder/all>
// // #include <boost/multiprecision/cpp_int.hpp>
// using namespace std;
// using namespace atcoder;
// // using mint = modint;
// using mint = modint998244353;
// // using mint = modint1000000007;
// // using namespace boost::multiprecision;
// using uint = unsigned int;
// using ll = long long;
// using ull = unsigned long long;
// using ld = long double;
// using pii = pair<int, int>;
// using pll = pair<ll, ll>;
// using T3 = tuple<int, int, int>;
// using G = vector<vector<int>>;
// #define rep(i, n) for (ll i = 0; i < (n); ++i)
// #define rep2(i, a, b) for (ll i = a; i < (b); ++i)
// #define rrep2(i, a, b) for (ll i = int(a)-1; i >= (b); --i)
// #define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
// #define rng(a) a.begin(),a.end()
// #define rrng(a) a.rbegin(),a.rend()
// #define popcount __builtin_popcount
// #define popcount_ll __builtin_popcountll
// #define fi first
// #define se second
// #define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
// #define MIN(v) *min_element(rng(v))
// #define MAX(v) *max_element(rng(v))
// #define SUM(v) accumulate(rng(v),0LL)
// #define IN(v, x) (find(rng(v),x) != v.end())
// template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
// template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
// template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
// template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
// const ll dx[] = {-1, 0, 1, 0};
// const ll dy[] = {0, 1, 0, -1};
// const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
// const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
// const ll LINF = 3001002003004005006ll;
// const int INF = 1001001001;

// using S = int;
// S op(S a, S b) 
// {
//     return max(a, b);
// }
// S e()
// {
//     return -INF;
// }
// struct F 
// {
//     int val;
//     bool flag;
//     F(int _val=0, bool _flag=false): val(_val), flag(_flag) {}
// };
// S mapping(F f, S x) 
// {
//     if (f.flag) return f.val;
//     else return x;
// }
// F composition(F f, F g)
// {
//     if (f.flag) return f;
//     else return g;
// }
// F id()
// {
//     return F();
// }

// void solve()
// {
//     int n, m; cin >> n >> m;
//     lazy_segtree<S, op, e, F, mapping, composition, id> seg(n);
//     rep(i, n) seg.set(i, 0);
//     rep(qi, m)
//     {
//         int l, r, c; cin >> l >> r >> c; l--;
//         seg.apply(l, r, F(c, true));
//     }
//     rep(i, n) cout << seg.get(i) << " ";
//     cout << endl; 
// }

// int main()
// {
//     ios::sync_with_stdio(false);
//     cin.tie(nullptr);
//     int t = 1;
//     // cin >> t;
//     while(t--)
//     {
//         solve();
//     }
//     return 0;
// }