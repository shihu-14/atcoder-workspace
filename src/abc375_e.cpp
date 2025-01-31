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
int dp[110][510][510]; 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<pii> p;
    int s = 0;
    rep(i, n){
        int a, b; cin >> a >> b; a--;
        p.emplace_back(a, b);
        s += b;
    }
    if (s%3 != 0){
        cout << -1 << endl;
        return 0;
    }
    
    s /= 3;
    rep(i, 110)rep(j, 510)rep(k, 510) dp[i][j][k] = INF;
    dp[0][0][0] = 0;
    int tmp_s = 0;
    rep(i, n){
        auto [a, b] = p[i];
        rep(j, s+1)rep(k, s+1)rep(t, 3){
            int nj = j, nk = k, f = 0;
            if (t == 0){
                if (a != 0) f++;
                nj += b;
            }
            else if (t == 1){
                if (a != 1) f++;
                nk += b;
            }
            else if (a != 2) f++;
            chmin(dp[i+1][nj][nk], dp[i][j][k]+f);
        }
        tmp_s += b;
    }
    cout << (dp[n][s][s] == INF ? -1 : dp[n][s][s]) << endl;
    return 0;
}