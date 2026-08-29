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
    ll h, w, d; cin >> h >> w >> d;
    ll n = h*w;
    vector<vector<int>> a(h, vector<int>(w));
    vector<pii> id(n);
    rep(i, h)rep(j, w)
    {
        cin >> a[i][j], a[i][j]--;
        id[a[i][j]] = {i, j};
    }
    int q; cin >> q;
    vector<vector<int>> g;
    vector<int> id2(n);
    rep(i, d)
    {
        vector<int> vs(1);
        rep3(j, i, n, d)
        {
            auto [x, y] = id[j];
            auto [nx, ny] = id[(j+d)%n];
            id2[j] = vs.size();
            vs.emplace_back(abs(x-nx)+abs(y-ny));
        }
        rep(j, vs.size()-1) vs[j+1] += vs[j];
        g.emplace_back(vs);
    }
    // printv(id2);
    int D = sqrt(q);
    rep(qi, q)
    {
        int l, r; cin >> l >> r; l--, r--;
        if (d < D)
        {
            int li = id2[l], ri = id2[r];
            int i = l%d;
            // cout << qi << ": " << li << " " << ri << '\n';
            // printv(g[i]);
            cout << g[i][ri-1]-g[i][li-1] << '\n';
        }
        else
        {
            int ans = 0;
            while(l != r)
            {
                int nl = l + d;
                auto [x, y] = id[l];
                auto [nx, ny] = id[nl];
                ans += abs(x-nx)+abs(y-ny);
                l = nl;
            }
            cout << ans << '\n';
        }
    }
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