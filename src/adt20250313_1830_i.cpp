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
    ll max_sum = 0, max_x, max_y;
    rep(i, h)rep(j, w){
        if (i+h1 > h || j+w1 > w) continue;
        if (max_sum < cs.query(i, j, i+h1, j+w1)){
            max_sum = cs.query(i, j, i+h1, j+w1);
            max_x = i;
            max_y = j;
        }
    }
    cerr << max_sum << endl;
    ll max_sum2 = 0;
    rep2(i, max_x, h)rep2(j, max_y, w){
        if (i+h2 > max_x+h1 || j+w2 > max_y+w1) continue;
        cerr << i << " " << j << endl;
        if (max_sum2 < cs.query(i, j, i+h2, j+w2)){
            max_sum2 = cs.query(i, j, i+h2, j+w2);
        }
    }
    cerr << max_sum2 << endl;
    cout << max_sum-max_sum2 << endl;
    return 0;
}