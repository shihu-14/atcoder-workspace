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
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n);
    rep(i, m)
    {
        int u, v; cin >> u >> v; u--, v--;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }
    vector<int> used(n, -1);
    vector<int> stk, ans;
    auto dfs = [&](auto f, int v) -> bool
    {
        // cout << v << "--------" << used[v] << endl;
        // printv(stk);
        // printv(used);
        for (int u: g[v])
        {
            // cout << v << "-> " << u << endl;
            // if (v == 2 && u == 0)
            // {
            //     cout << used[u] << "****" << (used[v]-used[u])%2 << endl;
            // }
            if (used[u] == -1)
            {
                used[u] = used[v]+1;
                stk.emplace_back(u);
                if (f(f, u)) return true;
                stk.pop_back();
                used[u] = 0;
            }
            else if (used[u] > 0 && (used[v]-used[u])%2 == 0)
            {
                // printv(stk);
                while(stk.size() && stk.back() != u)
                {
                    ans.emplace_back(stk.back());
                    stk.pop_back();
                }
                ans.emplace_back(u);
                return true;
            }
        }
        return false;
    };
    used[0] = 1;
    stk.emplace_back(0);
    if (dfs(dfs, 0))
    {
        cout << ans.size() << endl;
        rep(i, ans.size())
        {
            cout << ans[i]+1 << " ";
        }
        cout << endl;
    }
    else
    {
        cout << -1 << endl;
    }
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