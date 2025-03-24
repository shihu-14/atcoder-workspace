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
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w, h1, w1, h2, w2; cin >> h >> w >> h1 >> w1 >> h2 >> w2;
    vector a(h, vector<ll>(w, 0));
    rep(i, h)rep(j, w) cin >> a[i][j];
    CumSum cs(a);
    if (h1 <= h2 && w1 <= w2){
        cout << 0 << endl;
        return 0;
    }
    chmin(h2, h1);
    chmin(w2, w1);
    int L1 = h-h1+1, R1 = w-w1+1;
    int L2 = h-h2+1, R2 = w-w2+1;
    int L3 = h1-h2+1, R3 = w1-w2+1;
    vector<vector<ll>> s(L2, vector<ll>(R2));
    rep(i, L2)rep(j, R2) s[i][j] = cs.query(i, j, i+h2, j+w2);
    vector<vector<ll>> g(L2, vector<ll>(R2-R3+1)), g2(L2-L3+1, vector<ll>(R2-R3+1));
    rep(i, L2){
        multiset<ll> mst;
        rep(j, R3) mst.insert(s[i][j]);
        g[i][0] = *mst.rbegin();
        rep2(j, R3, R2){
            mst.insert(s[i][j]);
            mst.erase(mst.find(s[i][j-R3]));
            g[i][j-R3+1] = *mst.rbegin();
        }
    }
    rep(j, R2-R3+1){
        multiset<ll> mst;
        rep(i, L3) mst.insert(g[i][j]);
        g2[0][j] = *mst.rbegin();
        rep2(i, L3, L2){
            mst.insert(g[i][j]);
            mst.erase(mst.find(g[i-L3][j]));
            g2[i-L3+1][j] = *mst.rbegin();
        }
    }
    ll ans = 0;
    rep(i, L1)rep(j, R1) chmax(ans, cs.query(i, j, i+h1, j+w1)-g2[i][j]);
    cout << ans << endl;
    return 0;
}