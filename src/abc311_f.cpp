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
template<class T> void printvv2(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j].val()<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<char>> g(n+m, vector<char>(m+1));
    rep2(i, m-1, n+m-1)rep(j, m){
        char c; cin >> c;
        g[i-j][j] = c;
    }
    rep(i, n+m-1){
        rep(j, m){
            if (g[i][j] == '#'){
                g[i][j+1] = g[i+1][j] = '#';
            }
        }
    }
    rep(i, n+m) g[i][m] = '#';
    rep(j, m+1) g[n+m-1][j] = '#';
    rep(i, m-1)rep(j, m-1-i) g[i][j] = '#';
    rep2(i, n, n+m-1)rrep2(j, m, m-(i+1-n)) g[i][j] = '#';

    int sx=-1, sy, gx, gy;
    {
        rep(i, n+m){
            bool flag = false;
            rrep2(j, m, 0){
                if (g[i][j] == '.'){
                    sx =  i, sy = j+1;
                    flag = true;
                    break;
                }
            }
            if (flag) break;
        }
        rrep2(i, n+m-1, 0){
            bool flag = false;
            rep(j, m+1){
                if (g[i][j] == '.'){
                    gx = i+1, gy = j;
                    flag = true;
                    break;
                }
            }
            if (flag) break;
        }
    }
    if (sx == -1){
        cout << 1 << endl;
        return 0;
    }
    // printvv(g);
    // printf("%d,%d,%d,%d\n", sx, sy, gx, gy);
    vector<vector<mint>> dp(n+m, vector<mint>(m+1));
    dp[sx][sy] = 1;
    rep(i, n+m)rrep2(j, m+1, 0){
        if (dp[i][j] == 0) continue;
        if (i+1 < n+m){
            dp[i+1][j] += dp[i][j];
        }
        if (j-1 >= 0){
            if (j == m-i || (i && g[i-1][j-1] == '.')){
                dp[i][j-1] += dp[i][j];
            }
        }
    }
    // printvv2(dp);
    cout << dp[gx][gy].val() << endl;
    return 0;
}   
/*
....#.
..#...
.....#
......
##..##
*/