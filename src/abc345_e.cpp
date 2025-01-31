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
struct D{
    // Edit here (data)
    ll val; int col; 
    D(ll _val=-LINF, int _col=-1): val(_val), col(_col) {} 
};
struct Top2{
    D a, b;
    Top2(D _a=D(), D _b=D()): a(_a), b(_b) {}
    void operator<=(D x){
        if (b.val < x.val){
            swap(b, x);
            if (a.val < b.val) swap(a, b);
        }
        if (a.col == b.col) swap(b, x); // Edit here (constrains)
    }
    void operator<=(Top2 x){
        *this <= x.a;
        *this <= x.b;
    }
};
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;

    vector<Top2> dp(k+1); dp[0] = Top2(D(0, -2), D(0, -3));
    rep(i, n){
        int c; ll v; cin >> c >> v; c--;
        vector<Top2> pre(k+1); swap(dp, pre);
        rep(j, k+1){
            if (pre[j].a.val == -LINF) continue;
            if (j+1 <= k) dp[j+1] <= pre[j];
            if (pre[j].a.col != c) dp[j] <= D(pre[j].a.val+v, c);
            if (pre[j].b.col != c && pre[j].b.val != -LINF) dp[j] <= D(pre[j].b.val+v, c);
        }
    }
    cout << (dp[k].a.val == -LINF ? -1 : dp[k].a.val) << endl;
    return 0;
}