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
    vector<string> s(n);
    rep(i, n) cin >> s[i];
    vector<ll> cnt(3);
    {
        unordered_map<string, ll> mp;
        rep(i, n) mp[s[i]]++;
        for (auto& [_, v]: mp)
        {
            cnt[0] += v*(v-1)/2;
        }
    }
    {
        unordered_map<string, ll> mp;
        rep(i, n)rep(j, m)
        {
            string t = s[i];
            t[j] = '#';
            mp[t]++;
        }
        for (auto& [_, v]: mp)
        {
            cnt[1] += v*(v-1)/2;
        }
    }
    {
        unordered_map<string, ll> mp;
        rep(i, n)rep(j, m)rep2(k, j+1, m)
        {
            string t = s[i];
            t[j] = '#';
            t[k] = '#';
            mp[t]++;
        }
        for (auto& [_, v]: mp)
        {
            cnt[2] += v*(v-1)/2;
        }
    }
    ll d1 = cnt[1]-m*cnt[0];
    ll d2 = cnt[2]-(m-1)*d1-m*(m-1)/2*cnt[0];
    cout << cnt[0]+d1+d2 << '\n';
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