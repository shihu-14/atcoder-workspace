// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
using mint = modint;
// using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
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

mint comb[40][40];
mint d[20][20][450];
mint dp[20][20][20][450];
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, p; cin >> n >> p;
    int n2 = n/2, m = n*(n-1)/2;
    mint::set_mod(p);

    rep(i, n+1) comb[i][0] = 1;
    rep2(i, 1, n+1)rep2(j, 1, i+1){
        comb[i][j] = comb[i-1][j]+comb[i-1][j-1];
    }

    rep2(k, 1, n2+1) d[k][0][0] = 1;
    rep2(k, 1, n2+1)rep(nk, n2)rep(ne, m+1){
        rep2(i, 1, k+1)rep(j, nk+1){
            int e = ne-i-j;
            if (e < 0) continue;
            d[k][nk+1][ne] += d[k][nk][e]*comb[k][i]*comb[nk][j];
        }
    }

    dp[1][0][1][0] = 1;
    rep2(ij, 1, n+1)rep2(i, 1, ij+1)rep2(k, 1, i+1)rep(e, ij*(ij-1)/2+1){
        int j = ij-i;
        if (i > n2 || j > n2) continue;
        rep2(nk, 1, n2-j+1)rep2(ne, 1, m-e+1){
            dp[j+nk][i][nk][e+ne] += dp[i][j][k][e]*comb[n-ij][nk]*d[k][nk][ne];
        }
    }

    rep2(e, n-1, m+1){
        mint ans;
        rep2(k, 1, n2+1) ans += dp[n2][n2][k][e];
        cout << ans.val() << " ";
    }
    cout << endl;
    return 0;
}

