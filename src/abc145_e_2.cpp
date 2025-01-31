#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < n; ++i)
#define rep2(i, a, b) for (ll i = a; i < b; ++i)
#define rrep2(i, a, b) for (ll i = a; i > b; --i)
#define rep3(i, a, b, c) for (ll i = a; i < b; i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
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
    int n, t; cin >> n >> t;
    vector<pii> p(n);
    rep(i, n) cin >> p[i].first >> p[i].second;
    vector<vector<int>> dp1(n+1, vector<int>(t, -INF)); dp1[0][0] = 0;
    vector<vector<int>> dp2(n+1, vector<int>(t, -INF)); dp2[0][0] = 0;
    rep(i, n){
        auto [a, b] = p[i];
        rep(j, t){
            if (dp1[i][j] == -INF) continue;
            chmax(dp1[i+1][j], dp1[i][j]);
            int nj = j+a;
            if (nj < t) chmax(dp1[i+1][nj], dp1[i][j]+b);
        }
        rep(j, t-1){
            chmax(dp1[i+1][j+1], dp1[i+1][j]);
        }
    }
    rep(i, n){
        auto [a, b] = p[n-1-i];
        rep(j, t){
            if (dp2[i][j] == -INF) continue;
            chmax(dp2[i+1][j], dp2[i][j]);
            int nj = j+a;
            if (nj < t) chmax(dp2[i+1][nj], dp2[i][j]+b);
        }
        rep(j, t-1){
            chmax(dp2[i+1][j+1], dp2[i+1][j]);
        }
    }
    

    int ans = -INF;
    rep(i, n){
        int res = -INF;
        rep(j, t){
            chmax(res, dp1[i][j]+dp2[n-1-i][t-1-j]);
        }
        chmax(ans, res+p[i].second);
    }
    cout << ans << endl;
    return 0;
}