// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
// using mint = modint998244353;
using mint = modint1000000007;
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
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j].val()<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;
mint dp[1000][4][4][4];
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    rep(i, 4)rep(j, 4)rep(k, 4){
        if(k == 0 && j == 2 && i == 1) continue;
        if(k == 2 && j == 0 && i == 1) continue;
        if(k == 0 && j == 1 && i == 2) continue;
        dp[0][i][j][k] = 1;
    }
    rep(i, n-3){
        rep(j, 4)rep(k, 4)rep(l, 4)rep(c, 4){
            if(l == 0 && k == 2 && j == 1) continue;
            if(l == 2 && k == 0 && j == 1) continue;
            if(l == 0 && k == 1 && j == 2) continue;

            if (j == 2 && k == 0){
                if (c == 1) continue;
                dp[i+1][c][j][k] += dp[i][j][k][l];
            }
            else if (j == 0 && k == 2){
                if (c == 1) continue;
                dp[i+1][c][j][k] += dp[i][j][k][l];
            }
            else if (j == 1 && k == 0){
                if (c == 2) continue;
                dp[i+1][c][j][k] += dp[i][j][k][l];
            }
            else if (j == 2 && l == 0){
                if (c == 1) continue;
                dp[i+1][c][j][k] += dp[i][j][k][l];
            }
            else if (k == 2 && l == 0){
                if (c == 1) continue;
                dp[i+1][c][j][k] += dp[i][j][k][l];
            }
            else{
                dp[i+1][c][j][k] += dp[i][j][k][l];
            }
        }
    }
    mint ans;
    rep(i, 4)rep(j, 4)rep(k, 4) ans += dp[n-3][i][j][k];
    cout << ans.val() << endl;
    return 0;
}