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
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
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
    int n; cin >> n;
    vector<ll> a(n);
    rep(i,n) cin >> a[i];
    vector<vector<ll>> dp(2, vector<ll>(2, -LINF));
    dp[1][0] = 0;
    rep(i, n){
        vector<vector<ll>> pre(2, vector<ll>(2, -LINF)); swap(pre, dp);
        rep(j, 2){
            if (j == 0){
                chmax(dp[0][1], max(pre[1][1]+2*a[i], pre[0][0]+2*a[i]));
                chmax(dp[0][0], max(pre[1][1], pre[0][0]));
            }
            else{
                chmax(dp[1][1], max(pre[0][1]+a[i], pre[1][0]+a[i]));
                chmax(dp[1][0], max(pre[0][1], pre[1][0]));
            }
        }
    }
    ll ans = -LINF;
    rep(i, 2)rep(j, 2) chmax(ans, dp[i][j]);
    cout << ans << endl;
    return 0;
}