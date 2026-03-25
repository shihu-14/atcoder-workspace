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

template<typename T=int>
struct CC {
    bool initialized;
    vector<T> xs;
    CC(): initialized(false) {}
    void add(T x) { xs.push_back(x);}
    void init() {
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(),xs.end()),xs.end());
        initialized = true;
    }
    // x以下の登録されている値のうち最大のもののインデックスを返す. 見つからなければ-1を返す.
    int operator()(T x) {
        if (!initialized) init();
        return upper_bound(xs.begin(), xs.end(), x) - xs.begin() - 1;
    }
    T operator[](int i) {
        if (!initialized) init();
        return xs[i];
    }
    int size() {
        if (!initialized) init();
        return xs.size();
    }
};


struct S
{
    ll val, num;
};
S op(S a, S b)
{
    if (a.num < b.num) return a;
    else if (a.num > b.num) return b;
    a.val += b.val;
    return a;
}
S e()
{
    return {0, LINF};
}
using F = ll;
S mapping(F f, S x)
{
    x.num += f;
    return x;
}
F composition(F f, F g)
{
    return f+g;
}
F id()
{
    return 0;
}
void solve()
{
    ll H, W, h, w, n; cin >> H >> W >> h >> w >> n;
    H -= h-1;
    W -= w-1;
    if (n == 0)
    {
        cout << H*W << endl;
        return;
    }
    CC<ll> cc;
    map<ll, vector<T3>> query;
    rep(i, n)
    {
        ll r, c; cin >> r >> c;
        ll rl = max(0LL, r-h), rr = min(H, r);
        ll cl = max(0LL, c-w), cr = min(W, c);
        query[rl].emplace_back(cl, cr, 1);
        query[rr].emplace_back(cl, cr, -1);
        cc.add(cl);
        cc.add(cr);
    }
    cc.add(0);
    cc.add(W);
    query[H].emplace_back(0, W, 0);

    int m = cc.size();
    lazy_segtree<S, op, e, F, mapping, composition, id> seg(m-1);
    rep(i, m-1)
    {
        ll w = cc[i+1] - cc[i];
        seg.set(i, {w, 0});
    }
    ll ans = 0;
    ll pr = 0;
    for (auto [r, events] : query)
    {
        auto [val, num] = seg.all_prod();
        if (num == 0) ans += val*(r-pr);
        pr = r;
        for (auto [cl, cr, t] : events)
        {
            seg.apply(cc(cl), cc(cr), t);
        }
    }
    cout << ans << endl;
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