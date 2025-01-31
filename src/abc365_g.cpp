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
    int n, m; cin >> n >> m;
    vector<pii> a;
    vector<vector<int>> b(n);
    rep(i, m){
        int t, p; cin >> t >> p; p--;
        a.emplace_back(t, p);
        b[p].emplace_back(t);
    } 
    int q; cin >> q;
    int D = m/sqrt(q);
    vector<int> isBig(n);
    rep(i, n) isBig[i] = b[i].size() > D;
    vector<vector<int>> ans(n);
    rep(i, n) if (isBig[i]) {
        ans[i] = vector<int>(n, 0);
        vector<int> enter(n, -1);
        ll s = 0; int in = 0, pt = 0;
        for (auto [t, p]: a){
            if (in) s += t-pt;
            if (p == i){
                in ^= 1;
            }
            else{
                ans[i][p] += enter[p]*s;
                enter[p] *= -1;
            }
            pt = t;
        }
    }
    rep(qi, q){
        int u, v; cin >> u >> v; u--, v--;
        if (b[u].size() <= D && b[v].size() <= D){
            ll res = 0;
            vector<pii> merge_query;
            rep(i, b[u].size()) merge_query.emplace_back(b[u][i], i%2);
            rep(i, b[v].size()) merge_query.emplace_back(b[v][i], i%2);
            sort(rng(merge_query));
            int pt = 0, in = 0;
            for (auto [t, f]: merge_query){
                if (in == 2) res += t-pt;
                in += (f ? -1 : 1);
                pt = t;
            }
            cout << res << endl;
        }
        else if (b[u].size() > D){
            cout << ans[u][v] << endl;
        }
        else if (b[v].size() > D){
            cout << ans[v][u] << endl;
        }
    }
    return 0;
}