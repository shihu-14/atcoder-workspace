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
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;
const ll b = 1e12;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, k; cin >> n >> k;
    mcf_graph<ll, ll> g(2*n+2);
    rep2(i, 1, n+1){
        rep2(j, n+1, 2*n+1){
            int a; cin >> a;
            g.add_edge(i, j, 1, b-a);
        }
    }
    rep2(i, 1, n+1){
        g.add_edge(0, i, k, 0);
    }
    rep2(i, n+1, 2*n+1){
        g.add_edge(i, 2*n+1, k, 0);
    }
    g.add_edge(0, 2*n+1, LINF, b);
    auto p = g.flow(0, 2*n+1, n*k);
    cout << b*n*k-p.se << endl;   
    vector ans(n, vector<char>(n, '.'));
    for (auto e: g.edges()){
        if (e.from == 0 || e.to == 2*n+1 || e.flow == 0) continue;
        ans[e.from-1][e.to-n-1] = 'X';
    }   
    rep(i, n){
        rep(j, n){
            cout << ans[i][j];
        }
        cout << endl;
    }
    return 0;
}