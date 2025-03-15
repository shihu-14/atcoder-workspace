// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

ll dp[7][7][7][7][40];
ll s[7][7][7][7];
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w, t; cin >> h >> w >> t;
    vector<vector<ll>> a(h, vector<ll>(w, 0));
    rep(i, h)rep(j, w) cin >> a[i][j];
    vector<ll> Ls;
    rep(sx, h)rep(sy, w)rep(gx, h)rep(gy, w){
        ll res = 0;
        rep2(x, sx, gx+1)rep2(y, sy, gy+1){
            res += a[x][y];
        }
        s[sx][sy][gx][gy] = res;
        Ls.emplace_back(res);
    }
    UNIQUE(Ls);
    ll ans = LINF;
    for (auto L: Ls){
        rep(i1, h)rep(j1, w)rep(i2, h)rep(j2, w)rep(k, t+1){
            if (k == 0) dp[i1][j1][i2][j2][k] = s[i1][j1][i2][j2];
            else dp[i1][j1][i2][j2][k] = LINF;
        }
        rep2(k, 1, t+1)rep(sx, h)rep(sy, w)rep2(gx, sx, h)rep(gy, w){
            if (gx == sx && sy > gy) continue;  
            rep(k2, k){
                rep2(bx, sx+1, gx+1){
                    if (min(dp[sx][sy][bx-1][gy][k2], dp[bx][sy][gx][gy][k-1-k2]) >= L){
                        chmin(dp[sx][sy][gx][gy][k], max(dp[sx][sy][bx-1][gy][k2], dp[bx][sy][gx][gy][k-1-k2]));
                    }
                }
                rep2(by, sy+1, gy+1){
                    if (min(dp[sx][sy][gx][by-1][k2], dp[sx][by][gx][gy][k-1-k2]) >= L){
                        chmin(dp[sx][sy][gx][gy][k], max(dp[sx][sy][gx][by-1][k2], dp[sx][by][gx][gy][k-1-k2]));   
                    }
                }
            }
        }
        chmin(ans, dp[0][0][h-1][w-1][t]-L);
    }
    cout << ans << endl;
    return 0;
}