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
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;
mint dp[60][4][60][60][60];
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
    string s; cin >> s;
    dp[0][0][1][0][0] = 1;
    rep(i, n)rep(j, 4)rep(a, n+2)rep(b, n+1)rep(c, n+1){
        if (a+b+c > i+1) continue;
        if (s[i] == '?'){
            rep(d, 3){
                int nj = j; nj ^= 1<<d;
                if (nj > 3) nj = 7-nj;
                int na = a, nb = b, nc = c;
                if (nj == 0) na++;
                else if (nj == 1) nb++;
                else if (nj == 2) nc++;
                dp[i+1][nj][na][nb][nc] += dp[i][j][a][b][c];
            }
        }
        else{
            int nj = j; nj ^= 1<<(s[i]-'A');
            if (nj > 3) nj = 7-nj;
            int na = a, nb = b, nc = c;
            if (nj == 0) na++;
            else if (nj == 1) nb++;
            else if (nj == 2) nc++;
            dp[i+1][nj][na][nb][nc] += dp[i][j][a][b][c];
        }
    }
    mint ans;
    rep(j, 4)rep(a, n+2)rep(b, n+1)rep(c, n+1){
        int res = 0;
        int d = n+1-a-b-c;
        if (d < 0) continue;
        res += a*(a-1)/2;
        res += b*(b-1)/2;
        res += c*(c-1)/2;
        res += d*(d-1)/2;
        if (res >= k) ans += dp[n][j][a][b][c];
    }
    cout << ans.val() << endl;
    return 0;
}