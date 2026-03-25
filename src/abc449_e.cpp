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
    ll n, m; cin >> n >> m;
    vector<ll> a(n);
    rep(i, n) cin >> a[i], a[i]--;
    vector<ll> cnt(m);
    rep(i, n) cnt[a[i]]++;
    vector<vector<ll>> pos(n+1);
    rep(i, m) pos[cnt[i]].emplace_back(i);

    ll q; cin >> q;
    vector<pll> query;
    rep(qi, q)
    {
        ll x; cin >> x; x--;
        query.emplace_back(x, qi);
    }
    sort(rng(query));
    vector<ll> ans(q);
    fenwick_tree<ll> fw(m);
    ll sum = n;
    ll qi = 0;
    {
        int c = 0;
        while(qi < q && query[qi].fi < sum)
        {
            ans[query[qi].se] = a[query[qi].fi];
            qi++;
        }
    }
    rep(i, n+1)
    {
        auto f = [&](int c)
        {
            int ac = 0, wa = m;
            while(wa-ac > 1)
            {
                int wj = (ac+wa)/2;
                if (fw.sum(0, wj) < c) ac = wj;
                else wa = wj;
            }
            return ac;
        };
        for (int c : pos[i])
        {
            fw.add(c, 1);
        }
        ll X = fw.sum(0, m);
        while(qi < q && query[qi].fi < sum + X)
        {
            ll k = query[qi].fi - sum;
            ans[query[qi].se] = f(k+1);
            qi++;
        }
        sum += X;
    }
    {
        while(qi < q)
        {
            ll k = query[qi].fi - sum;
            ans[query[qi].se] = k%m;
            qi++;
        }
    }
    rep(qi, q) cout << ans[qi]+1 << endl;
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