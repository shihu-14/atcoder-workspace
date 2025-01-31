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
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
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
    int n, k; ll x; cin >> n >> k >> x;
    vector<ll> t(n+1, -LINF), ts(n+2);
    rep(i, n) cin >> t[i+1];
    rep(i, n+1) ts[i+1] = ts[i]+t[i+1];
    vector<ll> d;
    map<ll, int> used, id;
    multiset<ll> mst;
    rep(i, n) mst.insert(t[i]);
    mst.insert(-LINF);
    d.emplace_back(-LINF);
    rep(i, n)rep(j, n+1){
        ll dd = t[i]+j*x;
        if (used[dd] == 0) d.emplace_back(dd);
        used[dd] = 1;
    }
    sort(rng(d));
    int m = d.size();
    fenwick_tree<ll> fw(m);
    rep(i, m){
        id[d[i]] = i;
        int c = mst.count(d[i]);
        if (c) fw.add(i, c);
    }

    vector dp(m, vector<ll>(n+1, LINF)); dp[0][0] = 0;
    rep(i, m)rep(j, n+1){
        if (d[id[t[j]]] > d[i]) break;
        if (i+1 < m) chmin(dp[i+1][j], dp[i][j]);
        int it = lower_bound(rng(d), d[i]+x) - d.begin();
        if (it == m) break;
        ll res = 0;
        for (int nj = j+1; nj <= min(j+k, (ll)n); nj++){
            if (d[id[t[nj]]] > d[it]) break;
            res += d[it]-t[nj];
            chmin(dp[it][nj], dp[i][j]+res);
        }
    }

    printvv(dp);
    cout << dp[m-1][n] << endl;
    return 0;
}