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



int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
    double p = 1;
    rep(i, n) p *= 6;
    vector<double> dp1(1000), dp2(1000); dp1[0] = 1, dp2[0] = 1;
    rep(i, n){
        vector<double> pre(1000); swap(dp1, pre);
        rep(j, 700)rep2(k, 1, 7){
            dp1[j+k] += pre[j];
        }
    }
    rep(i, k){
        vector<double> pre(1000); swap(dp2, pre);
        rep(j, 700){
            rep2(k, 4, 7){
                dp2[j+k] += pre[j];
            }
            rep2(k, 4, 7){
                dp2[j+k] += pre[j];
            }
        }
    }
    rep(i, n-k){
        vector<double> pre(1000); swap(dp2, pre);
        rep(j, 700)rep2(k, 1, 7){
            dp2[j+k] += pre[j];
        }
    }
    rep(i, 700) dp1[i+1] += dp1[i]; 
    double ans = 0;
    rep(i, 700){
        if (dp2[i] == 0) continue;
        ans += dp1[i-1]*dp2[i]/p/p;
    }
    printf("%.16f\n", ans);
    return 0;
}