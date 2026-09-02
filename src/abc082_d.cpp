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
    string s; cin >> s;
    int gx, gy; cin >> gx >> gy;
    int n = s.size();
    vector<int> query;
    {
        query.emplace_back(0);
        rep(i, n)
        {
            if (s[i] == 'T')
            {
                query.emplace_back(0);
            }
            else
            {
                query[query.size()-1]++;
            }
        }
    }
    // printv(query);
    bool ans = true;
    { // for x axis
        map<int, int> dp;
        dp[0] = true;
        rep3(i, 0, query.size(), 2)
        {
            map<int, int> old; swap(dp, old);
            int dx = query[i];
            for (auto [x, v]: old)
            {
                if (i != 0)
                {
                    dp[x-dx] = v;
                }
                dp[x+dx] = v;
            }
        }
        if (dp[gx] == false) ans = false; 
    }
    { // for y axis
        map<int, int> dp;
        dp[0] = true;
        rep3(i, 1, query.size(), 2)
        {
            map<int, int> old; swap(dp, old);
            auto dy = query[i];
            for (auto [y, v]: old)
            {
                dp[y+dy] = v;
                dp[y-dy] = v;
            }
        }
        if (dp[gy] == false) ans = false; 
    }
    cout << (ans ? "Yes": "No") << '\n';
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