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

template <class T>
bool chmin(T &a, const T &b)
{
    if (a > b)
    {
        a = b;
        return true;
    }
    return false;
}
template <class T>
bool chmax(T &a, const T &b)
{
    if (a < b)
    {
        a = b;
        return true;
    }
    return false;
}
template <class T>
void printv(const vector<T> &v) { rep(i, v.size()) cout << v[i] << " \n"[i == (ll)v.size() - 1]; }
template <class T>
void printvv(const vector<vector<T>> &v)
{
    for (const auto &row : v)
        printv(row);
}

constexpr ll dx[] = {-1, 0, 1, 0};
constexpr ll dy[] = {0, 1, 0, -1};
constexpr ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
constexpr ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
constexpr ll LINF = 3001002003004005006LL;
constexpr int INF = 1001001001;

pll op(pll a, pll b)
{
    return min(a, b);
}
pll e()
{
    return {INF, -1};
}
void solve()
{
    int n, q;
    cin >> n >> q;
    segtree<pll, op, e> seg_l(n), seg_r(n);
    vector<ll> L(n), R(n);
    L[0] = R[n - 1] = INF;
    rep2(i, 1, n)
    {
        cin >> L[i];
        seg_l.set(i, {L[i], i});
    }
    rep(i, n - 1)
    {
        cin >> R[i];
        seg_r.set(i, {R[i], i});
    }
    // printv(L);
    // printv(R);
    rep(qi, q)
    {
        int s, t;
        cin >> s >> t;
        s--, t--;
        if (s < t)
        {
            ll ans = R[s];
            auto [mn, id] = seg_r.prod(0, s);
            // cout << ans << ' ' << mn << ' ' << id << '\n';
            if (id != -1)
            {
                chmin(ans, mn + L[s]);
            }
            cout << ans << '\n';
        }
        else
        {
            ll ans = L[s];
            auto [mn, id] = seg_l.prod(s + 1, n);
            if (id != -1)
            {
                chmin(ans, mn + R[s]);
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