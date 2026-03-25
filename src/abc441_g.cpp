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

struct S
{
    ll max_v;
    int cnt1, cnt2;
    S (ll max_v=-LINF, int cnt1=0, int cnt2=0) : max_v(max_v), cnt1(cnt1), cnt2(cnt2) {}
};
S op(S a, S b)
{
    S res;
    res.max_v = max(a.max_v, b.max_v);
    res.cnt1 = a.cnt1 + b.cnt1;
    res.cnt2 = a.cnt2 + b.cnt2;
    return res;
}
S e()
{
    return S();
}
struct F
{
    int flip;
    ll add;
    F (int flip=0, ll add=0) : flip(flip), add(add) {}
};
S mapping(F f, S x)
{
    S res;
    if (f.flip % 2 == 1)
    {
        res.cnt1 = x.cnt2;
        res.cnt2 = x.cnt1;
    }
    else
    {
        res.cnt1 = x.cnt1;
        res.cnt2 = x.cnt2;
    }
    if (res.cnt1 == 0)
    {
        res.max_v = 0;
    }
    else if (f.flip)
    {
        res.max_v = f.add;
    }
    else
    {
        res.max_v = x.max_v + f.add;
    }
    return res;
}
F composition(F f, F g)
{
    F res;
    if (f.flip)
    {
        res.flip = f.flip+g.flip;
        res.add = f.add;
    }
    else
    {
        res.flip = g.flip;
        res.add = f.add + g.add;
    }
    return res;
}
F id()
{
    return F();
}

void solve()
{
    int n, q; cin >> n >> q;
    lazy_segtree<S, op, e, F, mapping, composition, id> seg(n);
    rep(i, n) seg.set(i, S(0, 1, 0));
    rep(qi, q)
    {
        int type; cin >> type; 
        if (type == 1)
        {
            int l, r, x; cin >> l >> r >> x; l--;
            seg.apply(l, r, F(0, x));
        }
        else if (type == 2)
        {
            int l, r; cin >> l >> r; l--;
            seg.apply(l, r, F(1, 0));
        }
        else
        {
            int l, r; cin >> l >> r; l--;
            S res = seg.prod(l, r);
            cout << res.max_v << "\n";
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