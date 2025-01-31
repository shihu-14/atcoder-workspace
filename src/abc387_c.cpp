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
    ll L, R; cin >> L >> R; L--;

    auto f = [&](ll x) -> ll{
        if (x < 10) return 0;
        vector<int> d;
        while(x) d.emplace_back(x%10), x/=10;
        reverse(rng(d));

        int n = d.size();
        vector dp(10, vector<ll>(2, 0)); dp[d[0]][0] = 1;
        rep2(i, 1, n){
            vector pre(10, vector<ll>(2, 0)); swap(pre, dp);
            rep(j, 10)rep(k, 2){
                rep(nj, 10){
                    if (d[0] <= nj) continue;
                    if (k){
                        dp[nj][k] += pre[j][k];
                        continue;
                    }
                    if (d[i] < nj) continue;
                    int nk = k;
                    if (d[i] > nj) nk = 1;
                    dp[nj][nk] += pre[j][k];
                }
            }
        }

        ll res = 0;
        rep(i, 10)rep(j, 2) res += dp[i][j];
        rep2(i, 1, d[0]){
            ll tmp = 1;
            rep(j, n-1) tmp *= i;
            res += tmp;
        }
        rep2(i, 1, n-1){
            rep2(j, 1, 10){
                ll tmp = 1;
                rep(k, i) tmp *= j;
                res += tmp;
            }
        }
        return res;
    };
    cout << f(R)-f(L) << endl;
    return 0;
}