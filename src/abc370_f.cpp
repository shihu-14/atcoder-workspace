// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
    vector<ll> a(n), s(2*n+1);
    rep(i, n) cin >> a[i];
    rep(i, n) s[i+1] = s[i]+a[i];
    rep2(i, n, 2*n) s[i+1] = s[i]+a[i-n];

    auto make_func = [&](int w) -> vector<pll>{
        vector<pll> res;
        rep(i, n){
            int it = lower_bound(rng(s), s[i]+w) - s.begin(); 
            res.emplace_back(it%n, it-i);
        }
        return res;
    };

    auto calc = [&](int w) -> vector<pll>{
        vector<pll> g = make_func(w);
        int t = k;
        vector<pll> res;
        rep(i, n) res.emplace_back(i, 0);
        while(t){
            if (t&1){
                vector<pll> tmp;
                rep(i, n) tmp.emplace_back(g[res[i].fi].fi, g[res[i].fi].se+res[i].se);
                res = tmp;
            }  
            {
                vector<pll> tmp;
                rep(i, n) tmp.emplace_back(g[g[i].fi].fi, g[g[i].fi].se+g[i].se);
                g = tmp;
            }
            t >>= 1;
        }
        return res;
    };

    int ac = 0, wa = INF;
    while(wa-ac > 1){
        int wj = (ac+wa)/2;
        auto v = calc(wj);
        bool flag = false;
        rep(i, n) if (v[i].se <= n) flag = true;
        if (flag) ac = wj;
        else wa = wj;
    }

    int ans = 0;
    auto v = calc(ac);
    rep(i, n) ans += v[i].se > n;
    
    cout << ac << " " << ans << endl;
    return 0;
}