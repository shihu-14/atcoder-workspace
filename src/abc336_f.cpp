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
// const ll dx[] = {-1, 0, 1, 0};
// const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w; cin >> h >> w;
    G a(h, vector<int>(w));
    rep(i, h)rep(j, w) cin >> a[i][j];
    
    auto rotate = [&](int sx, int sy){
        G res = a;
        rep(i, h-1)rep(j, w-1) res[sx+i][sy+j] = a[sx+h-2-i][sy+w-2-j];
        a = res;
    };

    int dx[4] = {0, 0, 1, 1}, dy[4] = {0, 1, 1, 0};
    map<G, int> mp, mp2;
    auto f = [&](auto f, map<G, int> &memo, int d=0) -> void{
        if (d > 10) return;
        if (memo.count(a)){
            chmin(memo[a], d);
            return;
        }
        memo[a] = d;
        rep(t, 4){
            rotate(dx[t], dy[t]);
            f(f, memo, d+1);
            rotate(dx[t], dy[t]);
        }
    };

    f(f, mp);
    rep(i, h)rep(j, w) a[i][j] = i*w+j+1;
    f(f, mp2);
    int ans = INF;
    for (auto [g, c]: mp2){
        // rep(i, h)rep(j, w) cout << g[i][j] << " \n"[j==w-1];
        // cout << endl;
        if (mp.count(g)) chmin(ans, c+mp[g]);
    }
    cout << ((ans == INF) ? -1 : ans) << endl;
    return 0;
}
