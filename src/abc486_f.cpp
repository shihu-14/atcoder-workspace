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
    int n;
    ll k;
    cin >> n >> k;

    vector<pair<ll, ll>> v(n);
    for (auto& [a, b] : v) cin >> a >> b;
    sort(rng(v));

    // 同じ A_i をまとめる
    vector<pair<ll, ll>> g;
    for (auto [a, b] : v)
    {
        if (!g.empty() && g.back().first == a)
            g.back().second += b;
        else
            g.emplace_back(a, b);
    }

    int m = g.size();
    using i128 = __int128_t;

    // 限界費用 T 以下で増やせる種類数と総コスト
    auto calc = [&](ll T) -> pair<i128, i128>
    {
        vector<pair<ll, ll>> st;
        i128 cnt = 0, cost = 0;

        rep(i, m)
        {
            auto [a, b] = g[i];
            if (b > 1) st.emplace_back(a, b - 1);

            i128 x = (i128)a + 1;
            i128 r = (i + 1 < m
                ? (i128)g[i + 1].first - 1
                : (i128)a + T);

            while (x <= r)
            {
                while (!st.empty() && st.back().second == 0)
                    st.pop_back();

                if (st.empty()) break;

                auto [src, left] = st.back();
                if (x - src > T) break;

                i128 c = min({
                    (i128)left,
                    r - x + 1,
                    (i128)src + T - x + 1
                });

                cnt += c;
                cost += c * (x - src) + c * (c - 1) / 2;

                st.back().second -= (ll)c;
                x += c;
            }
        }
        return {cnt, cost};
    };

    ll lo = 0, hi = k + 1;
    while (hi - lo > 1)
    {
        ll mid = lo + (hi - lo) / 2;
        if (calc(mid).second <= k) lo = mid;
        else hi = mid;
    }

    auto [cnt, cost] = calc(lo);
    auto [nextCnt, nextCost] = calc(lo + 1);

    i128 extra = min(
        nextCnt - cnt,
        ((i128)k - cost) / (lo + 1)
    );

    cout << (ll)((i128)m + cnt + extra) << '\n';
}


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    cin >> t;
    while (t--)
    {
        solve();
    }

    return 0;
}