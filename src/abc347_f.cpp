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
using G = vector<vector<ll>>;
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

struct CumSum{
    int h, w; 
    vector<vector<ll>> s;
    CumSum(vector<vector<ll>> &a): h(a.size()), w(a[0].size()){
        s.resize(h+1, vector<ll>(w+1));
        rep(i, h)rep(j, w) s[i+1][j+1] = s[i+1][j] + a[i][j];
        rep(j, w+1)rep(i, h) s[i+1][j] += s[i][j];
    }
    ll query(int lx, int ly, int rx, int ry){ // [ly, ry), [lx, rx)の総和
        if (lx < 0 || ly < 0 || rx > h || ry > w) return 0;
        return s[rx][ry] - s[lx][ry] - s[rx][ly] + s[lx][ly];
    }
};

struct Rectangle3{
    int h, w, m; // h*wの長方形をm*mの3つの正方形が走査する
    vector<vector<ll>> g; // 走査する盤面
    Rectangle3(vector<vector<ll>> a, int _m): g(a), h(a.size()), w(a[0].size()), m(_m){}
    void rotate90(vector<vector<ll>>& a) {
        int h = a.size(), w = a[0].size();
        vector<vector<ll>> res(h, vector<ll>(w));
        rep(i, h)rep(j, w) res[w-1-j][i] = a[i][j];
        a = move(res);
    }
    ll solve(){
        ll res = 0;
        rep(rt, 4){
            int n = h;
            CumSum cs(g);
            vector<ll> max_l(n+1), max_r(n+1), max_col(n+1);
            vector<vector<ll>> max_ru(n+2, vector<ll>(n+2)), max_rd(n+2, vector<ll>(n+2));
            {
                // Edit here (pre-processing)
                rep(j, n)rep(i, n){
                    chmax(max_col[j+1], cs.query(i, j, i+m, j+m));
                }
                rep(j, n){
                    max_l[j+1] = max_l[j];
                    rep(i, n){
                        chmax(max_l[j+1], cs.query(i, j, i+m, j+m));
                    }
                }
                rrep2(j, n, 0){
                    rep(i, n){
                        chmax(max_r[j+1], cs.query(i, j, i+m, j+m));
                    }
                    max_r[j] = max_r[j+1];
                }
                rep(i, n){
                    rrep2(j, n, 0){
                        chmax(max_ru[i+1][j+1], max({cs.query(i, j, i+m, j+m), max_ru[i][j+1], max_ru[i+1][j+2]}));
                    }
                }
                rrep2(i, n, 0){
                    rrep2(j, n, 0){
                        chmax(max_rd[i+1][j+1], max({cs.query(i, j, i+m, j+m), max_rd[i+2][j+1], max_rd[i+1][j+2]}));
                    }
                }
            }
            rep(j, w)rep(i, h){
                if (j+1 < m || w-1-j < m || i+1 < m || h-1-i < m) continue;
                // Edit here (calc)
                chmax(res, max_l[j+1-m+1]+max_ru[i+1-m+1][j+2]+max_rd[i+2][j+2]);
            }
            rep(j1, w){
                ll max_m = 0;
                rep2(j2, j1+1, w){
                    if (j1+1 < m || j2-j1 < m || w-1-j2 < m) continue;
                    // Edit here (calc)
                    chmax(max_m, max_col[j2+1-m+1]);
                    chmax(res, max_l[j1+1-m+1]+max_m+max_r[j2+2]);
                }
            }
            rotate90(g);
        }
        return res;
    }
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    G a(n, vector<ll>(n));
    rep(i, n)rep(j, n) cin >> a[i][j];
    Rectangle3 r(a, m);
    ll ans = r.solve();
    cout << ans << endl;
    return 0;
}
