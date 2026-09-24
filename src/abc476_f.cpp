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

void solve()
{
    ll n, m; cin >> n >> m;
    vector<ll> a(n), b(n);
    rep(i, n) cin >> a[i];
    rep(i, n) cin >> b[i];
    vector<ll> s1(2*n), s2(2*n);
    rep(i, n)rep(j, n)
    {
        s1[i+j+1] += a[i]*b[j]%m;
        s2[i-j+n] += a[i]*b[j]%m;
    }
    // printv(s2);
    vector<ll> s1x(2*n), s2x(2*n);
    rep(i, 2*n-1)
    {
        s1x[i+1] = s1x[i] + (i+1)*s1[i+1];
        s2x[i+1] = s2x[i] + (i+1)*s2[i+1];
        s1[i+1] += s1[i];
        s2[i+1] += s2[i];
    }
    vector<vector<ll>> dist(n, vector<ll>(n));
    rep(i, n)rep(j, n)
    {
        int u = i+j+1;
        int v = i-j+n;
        dist[i][j] += u*s1[u]-s1x[u]+(s1x[2*n-1]-s1x[u])-u*(s1[2*n-1]-s1[u]);
        
        dist[i][j] += v*s2[v]-s2x[v]+(s2x[2*n-1]-s2x[v])-v*(s2[2*n-1]-s2[v]);
        dist[i][j] /= 2;
        // if (i == 0 && j == 0)
        // {
        //     cout << dist[i][j] << endl;
        // }
    }
    ll ans = 0;
    rep(i, n)rep(j, n)
    {
        ans ^= dist[i][j]+i*n+j;
    }
    cout << ans << '\n';
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