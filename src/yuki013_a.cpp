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
    int h, w; cin >> w >> h; 
    vector<vector<int>> g(h, vector<int>(w)), to(h*w);
    rep(i, h)rep(j, w) cin >> g[i][j];
    rep(i, h)rep(j, w){
        rep(t, 4){
            int ni = i+dx[t], nj = j+dy[t];
            if(ni < 0 || ni >= h || nj < 0 || nj >= w) continue;
            if(g[i][j] != g[ni][nj]) continue;
            int u = i*w+j, v = ni*w+nj;
            to[u].emplace_back(v);
        }
    }
    vector used(h*w, false);
    auto f = [&](auto f, int u, int p=-1) -> bool{
        for(int v: to[u]){
            if (v == p) continue;
            if (used[v]) return true;
            used[v] = true;
            if (f(f, v, u)) return true;
        }
        return false;
    };
    rep(i, h)rep(j, w){
        if(used[i*w+j]) continue;
        used[i*w+j] = true;
        if (f(f, i*w+j)){
            cout << "possible" << endl;
            return 0;
        }
    }
    cout << "impossible" << endl;
    return 0;
}