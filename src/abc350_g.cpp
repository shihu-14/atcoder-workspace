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
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;
int mod=998244353;


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    int pre_ans = 0;
    G g(n);
    vector<int> par(n, -1);
    dsu uf(n);
    rep(qi, q){
        ll a, b, c; cin >> a >> b >> c;
        int type = 1+(a*(1+pre_ans)%mod)%2;
        int u = 1+(b*(1+pre_ans)%mod)%n; u--;
        int v = 1+(c*(1+pre_ans)%mod)%n; v--;
        if (type == 1){
            if (uf.size(u) < uf.size(v)) swap(u, v);
            auto f = [&](auto f, int u, int p=-1) -> void{
                for (auto v: g[u]){
                    if (v == p) continue;
                    par[v] = u;
                    f(f, v, u);
                }
            };
            f(f, v);
            par[v] = u;
            g[u].emplace_back(v), g[v].emplace_back(u);
            uf.merge(u, v);
        }
        else{
            int ans = -1;
            if (par[v] != -1 && par[par[v]] == u){
                ans = par[v];
            }
            else if (par[u] != -1 && par[par[u]] == v){
                ans = par[u];
            }
            else if (par[u] != -1 && par[v] != -1 && par[u] == par[v]){
                ans = par[u];
            }
            cout << ans+1 << "\n";
            pre_ans = ans+1;
        }
    }
    cout.flush();
    return 0;
}

/*

2 1 3
1 2 6
1 2 4
1 1 3
2 4 6
2 1 4
1 5 6
1 1 2
2 1 4
2 2 5
2 3 4
2 2 3

*/