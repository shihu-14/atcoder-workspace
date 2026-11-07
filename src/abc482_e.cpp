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


int n; 
vector<vector<int>> g;
vector<int> deg;

// find tree centers (at most 2, -1 if only one)
pii find_center(){
    int res_n = n;
    vector<int> tmp_deg = deg;
    queue<int> q;
    rep(i, n) if (deg[i] == 1) q.emplace(i); // initialize queue with deg1 vertices
    while(res_n > 2){
        res_n -= q.size();
        queue<int> tmp_q;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for (auto v: g[u]){ // enqueue vertices that become deg1 after leaf removal
                tmp_deg[v]--;
                if (tmp_deg[v] == 1) tmp_q.emplace(v);
            }
        }
        q = move(tmp_q);
    }
    if (q.size() == 1) return {q.front(), -1};
    else return {q.front(), q.back()};
}

void solve()
{
    cin >> n;
    if (n == 1)
    {
        cout << 0 << '\n';
        return;
    }
    g.resize(n);
    deg.resize(n);
    rep(i, n-1)
    {
        int u, v; cin >> u >> v; u--, v--;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
        deg[u]++;
        deg[v]++;
    }
    vector<int> c(n);
    rep(i, n) cin >> c[i], c[i]--;

    auto [c1, c2] = find_center();
    int from = 0, to = 0, max_dist = 0;
    {
        auto dfs = [&](auto f, int u, int p=-1, int s=0) -> void
        {
            if (g[u].size() == 1 && p != -1)
            {
                if (max_dist < s) max_dist = s, to = u;
                return;
            }
            for (auto v: g[u]) if (v != p) f(f, v, u, s+1);
        };
        dfs(dfs, from);
        from = to; max_dist = 0;
        dfs(dfs, from);
    }

    vector<ll> cnt(n);
    {
        vector<int> dist(n);
        auto dfs = [&](auto f, int s, int u, int p=-1) -> void
        {
            for (auto v: g[u])
            {
                if (v == p) continue;
                if (s == c1 && v == c2) continue;
                if (s == c2 && v == c1) continue;
                dist[v] = dist[u]+1;
                f(f, s, v, u);
            }
        };
        dfs(dfs, c1, c1);
        rep(i, n)
        {
            if (dist[i] == max_dist/2)
            {
                cnt[c[i]]++;
            }
        }
        if (c2 != -1)
        {
            dist.assign(n, 0);
            dfs(dfs, c2, c2);
            vector<ll> cnt2(n);
            rep(i, n)
            {
                if (dist[i] == max_dist/2)
                {
                    cnt2[c[i]]++;
                }
            }
            ll ans = 0;
            rep(i, n)
            {
                ans += cnt[i]*cnt2[i];
            }
            cout << ans << '\n';
        }
        else
        {
            ll ans = 0;
            rep(i, n)
            {
                ans += cnt[i]*(cnt[i]-1)/2;
            }
            cout << ans << '\n';
        }
    }
    
    // // cout << from << " " << to << " " << max_dist << '\n';
    // vector<ll> left(n), right(n), dd(n);
    // {
    //     vector<int> dist(n);
    //     vector<bool> used(n);
    //     auto dfs = [&](auto f, int u, int p=-1) -> void
    //     {
    //         for (auto v: g[u])
    //         {
    //             if (v == p) continue;
    //             dist[v] = dist[u]+1;
    //             f(f, v, u);
    //         }
    //     };   
    //     dfs(dfs, from);
    //     rep(i, n) 
    //     {
    //         if (dist[i] == max_dist)
    //         {
    //             left[c[i]]++;
    //             used[i] = true;
    //         }
    //     }
    //     dist.assign(n, 0);
    //     dfs(dfs, to);
    //     rep(i, n) 
    //     {
    //         if (dist[i] == max_dist)
    //         {
    //             if (used[i])
    //             {
    //                 dd[c[i]]++;
    //                 left[c[i]]--;
    //             }
    //             else right[c[i]]++;
    //         }
    //     }
    // }
    // ll ans = 0;
    // rep(i, n)
    // {
    //     ans += left[i]*right[i];
    //     ans += left[i]*dd[i];
    //     ans += right[i]*dd[i];
    // }
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