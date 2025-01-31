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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll k; cin >> k;
    ll sx, sy, gx, gy; cin >> sx >> sy >> gx >> gy;
    if (sx > gx){
        swap(sx, gx);
        swap(sy, gy);
    }
    if (k == 1){
        cout << abs(sx-gx) + abs(sy-gy) << endl;
        return 0;
    }
    ll ans = abs(sx-gx) + abs(sy-gy);
    ll ux = sx/k*k, uy = sy/k*k, vx = gx/k*k, vy = gy/k*k;
    bool in_s = (ux/k+uy/k)%2 == 1, in_g = (vx/k+vy/k)%2 == 1;

    vector<pll> ps, pg;
    for (int j = 0; j < 4; j++){
        ll tmp_ux = ux, tmp_uy = uy, tmp_vx = vx, tmp_vy = vy;
        tmp_ux += dx[j]*k, tmp_uy += dy[j]*k;
        tmp_vx += dx[j]*k, tmp_vy += dy[j]*k;
        ps.emplace_back(tmp_ux, tmp_uy);
        pg.emplace_back(tmp_vx, tmp_vy);
    }
    ll cross_cost = 4;
    if (k == 2) cross_cost--;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            ll res = 0;
            ll tmp_ux = ux, tmp_uy = uy, tmp_vx = vx, tmp_vy = vy;
            if (!in_s) tmp_ux += dx[i]*k, tmp_uy += dy[i]*k;
            if (!in_g) tmp_vx += dx[j]*k, tmp_vy += dy[j]*k;
            if (!in_s){
                ll min_cost = LINF;
                chmin(min_cost, abs(ps[0].fi+k-1-sx)+((abs(tmp_ux-ps[0].fi) + abs(tmp_uy-ps[0].se) == 0) ? 0 : abs(tmp_ux-ps[0].fi) == k && abs(tmp_uy-ps[0].se) == k ? 2 : cross_cost));
                chmin(min_cost, abs(ps[1].se-sy)+((abs(tmp_ux-ps[1].fi) + abs(tmp_uy-ps[1].se) == 0) ? 0 : abs(tmp_ux-ps[1].fi) == k && abs(tmp_uy-ps[1].se) == k ? 2 : cross_cost));
                chmin(min_cost, abs(ps[2].fi-sx)+((abs(tmp_ux-ps[2].fi) + abs(tmp_uy-ps[2].se) == 0) ? 0 : abs(tmp_ux-ps[2].fi) == k && abs(tmp_uy-ps[2].se) == k ? 2 : cross_cost));
                chmin(min_cost, abs(ps[3].se+k-1-sy)+((abs(tmp_ux-ps[3].fi) + abs(tmp_uy-ps[3].se) == 0) ? 0 : abs(tmp_ux-ps[3].fi) == k && abs(tmp_uy-ps[3].se) == k ? 2 : cross_cost));
                res += min_cost;
            }
            if (!in_g){
                ll min_cost = LINF;
                chmin(min_cost, abs(pg[0].fi+k-1-gx)+((abs(tmp_vx-pg[0].fi) + abs(tmp_vy-pg[0].se) == 0) ? 0 : abs(tmp_vx-pg[0].fi) == k && abs(tmp_vy-pg[0].se) == k ? 2 : cross_cost));
                chmin(min_cost, abs(pg[1].se-gy)+((abs(tmp_vx-pg[1].fi) + abs(tmp_vy-pg[1].se) == 0) ? 0 : abs(tmp_vx-pg[1].fi) == k && abs(tmp_vy-pg[1].se) == k ? 2 : cross_cost));
                chmin(min_cost, abs(pg[2].fi-gx)+((abs(tmp_vx-pg[2].fi) + abs(tmp_vy-pg[2].se) == 0) ? 0 : abs(tmp_vx-pg[2].fi) == k && abs(tmp_vy-pg[2].se) == k ? 2 : cross_cost));
                chmin(min_cost, abs(pg[3].se+k-1-gy)+((abs(tmp_vx-pg[3].fi) + abs(tmp_vy-pg[3].se) == 0) ? 0 : abs(tmp_vx-pg[3].fi) == k && abs(tmp_vy-pg[3].se) == k ? 2 : cross_cost));
                res += min_cost;
            }
            if (k > 2){
                ll dx = abs(tmp_ux-tmp_vx), dy = abs(tmp_uy-tmp_vy);
                ll min_d = min(dx/k, dy/k);
                res += min_d*2;
                dx -= min_d*k, dy -= min_d*k;
                res += dx/(2*k)*4;
                res += dy/(2*k)*4;
            }
            else{
                ll dx = abs(tmp_ux-tmp_vx), dy = abs(tmp_uy-tmp_vy);
                ll min_d = min(dx/k, dy/k);
                res += min_d*2;
                dx -= min_d*k, dy -= min_d*k;
                res += dx/(2*k)*3;
                res += dy/(2*k)*3;
            }
            chmin(ans, res);
        }
    }
    cout << ans << endl;
    return 0;
}