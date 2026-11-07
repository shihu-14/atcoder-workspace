// #pragma GCC target("avx2")
// #pragma GCC optimize("O3")
// #pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using namespace boost::multiprecision;

// using mint = modint;
using mint = modint998244353;
// using mint = modint1000000007;

using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;

#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)
#define rep2(i, a, b) for (ll i = (ll)(a); i < (ll)(b); ++i)
#define rrep(i, n) for (ll i = (ll)(n) - 1; i >= 0; --i)
#define rrep2(i, a, b) for (ll i = (ll)(a) - 1; i >= (ll)(b); --i)
#define rep3(i, a, b, c) for (ll i = (ll)(a); i < (ll)(b); i += (ll)(c))
#define rng(a) (a).begin(), (a).end()
#define rrng(a) (a).rbegin(), (a).rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), (v).erase(unique(rng(v)), (v).end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v), 0LL)
#define IN(v, x) (find(rng(v), (x)) != (v).end())

template<class T> bool chmin(T& a, const T& b){if(a>b){a=b;return true;}return false;}
template<class T> bool chmax(T& a, const T& b){if(a<b){a=b;return true;}return false;}
template<class T> void printv(const vector<T>& v){rep(i,v.size()) cout<<v[i]<<" \n"[i==(ll)v.size()-1];}
template<class T> void printvv(const vector<vector<T>>& v){for(const auto& row:v) printv(row);}

constexpr ll dx[] = {-1, 0, 1, 0};
constexpr ll dy[] = {0, 1, 0, -1};
constexpr ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
constexpr ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
constexpr ll LINF = 3001002003004005006LL;
constexpr int INF = 1001001001;
ll op_min(ll a, ll b)
{
    return min(a, b);
}
ll op_max(ll a, ll b)
{
    return max(a, b);
}
ll e_min(){return LINF;}
ll e_max(){return -LINF;}

ll mapping(ll f, ll x)
{
    return f+x;
}
ll composition(ll f, ll g)
{
    return f+g;
}
ll id(){return 0;}

void solve()
{
    int n; cin >> n;
    lazy_segtree<ll, op_min, e_min, ll, mapping, composition, id> seg_min(n); 
    lazy_segtree<ll, op_max, e_max, ll, mapping, composition, id> seg_max(n);
    vector<ll> a(n);
    {
        vector<ll> s(n);
        rep(i, n)
        {
            cin >> s[i];
            a[i] = s[i];
        }
        rep(i, n-1) s[i+1] += s[i];
        rep(i, n)
        {
            seg_min.set(i, s[i]);
            seg_max.set(i, s[i]);
        }
    } 

    ll ans = 0;
    auto f = [&]()
    {
        ll res = 0;
        ll mx = seg_max.all_prod() >= 0 ? 1: -1;
        ll mn = seg_min.all_prod() >= 0 ? 1: -1;
        if (mx*mn >= 0)
        {
            res = max(abs(seg_min.all_prod()), abs(seg_max.all_prod()));
        }
        else
        {
            res = abs(seg_min.all_prod())+abs(seg_max.all_prod());
        }
        chmax(ans, res);
    };
    f();

    rep(i, n)
    {
        ll v = a[i];
        seg_min.apply(i, n, -v*2);
        seg_max.apply(i, n, -v*2);
        // rep(j, n) cout << seg_min.get(j) << " ";
        // cout << endl;
        f();
        seg_min.apply(i, n, v*2);
        seg_max.apply(i, n, v*2);
        // rep(j, n) cout << seg_min.get(j) << " ";
        // cout << endl;
        // cout << "------------" << endl;
    }

    cout << ans+1 << '\n';
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--)
    {
        solve();
    }

    return 0;
}