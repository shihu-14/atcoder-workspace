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
    int n; cin >> n;
    vector<int> d1x, d1y, d2x, d2y; 
    rep(i, n){
        int a, b; cin >> a >> b;
        if ((a+b)&1) d1x.emplace_back(a+b), d1y.emplace_back(a-b);
        else d2x.emplace_back(a+b), d2y.emplace_back(a-b);
    }
    sort(rng(d1x));
    sort(rng(d1y));
    sort(rng(d2x));
    sort(rng(d2y));
    ll ans = 0;
    ll s = 0;
    rep(i, d1x.size()){
        ans -= s; 
        ans += (ll)d1x[i]*i;
        s += d1x[i];
    }
    s = 0;
    rep(i, d1y.size()){
        ans -= s; 
        ans += (ll)d1y[i]*i;
        s += d1y[i];
    }
    s = 0;
    rep(i, d2x.size()){
        ans -= s; 
        ans += (ll)d2x[i]*i;
        s += d2x[i];
    }
    s = 0;
    rep(i, d2y.size()){
        ans -= s; 
        ans += (ll)d2y[i]*i;
        s += d2y[i];
    }
    cout << ans/2 << endl;
    return 0;
}