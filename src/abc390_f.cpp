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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    vector<vector<int>> b(n), b2(n);
    rep(i, n) b[i].emplace_back(-1);
    rep(i, n) b2[i].emplace_back(-1);
    rep(i, n){
        cin >> a[i], a[i]--;
        b[a[i]].push_back(i);
        b2[a[i]].emplace_back(i);
        if (a[i] > 0) b2[a[i]-1].emplace_back(i);
    }
    rep(i, n) b[i].emplace_back(n);
    rep(i, n) b2[i].emplace_back(n);
    ll ans = 0;
    rep(i, n){
        ll res = (ll)n*(n+1)/2;
        rep(j, b[i].size()-1){
            ll c = b[i][j+1]-b[i][j]-1;
            res -= c*(c+1)/2;
        }
        ans += res;
    }
    rep(i, n-1){
        ll res = (ll)n*(n+1);
        rep(j, b[i].size()-1){
            ll c = b[i][j+1]-b[i][j]-1;
            res -= c*(c+1)/2;
        }
        rep(j, b[i+1].size()-1){
            ll c = b[i+1][j+1]-b[i+1][j]-1;
            res -= c*(c+1)/2;
        }
        rep(j, b2[i].size()-1){
            ll c = b2[i][j+1]-b2[i][j]-1;
            res += c*(c+1)/2;
        }
        res -= (ll)n*(n+1)/2;
        ans -= res;
    }
    cout << ans << endl;
    return 0;
}