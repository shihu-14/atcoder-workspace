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
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
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
    int t; cin >> t;
    rep(i, t){
        string s; ll l, r; cin >> s >> l >> r; l--;
        int n = s.size();
        auto f = [&](ll x) -> ll{
            vector<int> a;
            while(x) a.push_back(x%10), x /= 10;
            int m = a.size();
            reverse(rng(a));
            ll res = 0;
            rep(si, m-n+1){
                vector dp(2, vector<ll>(2, 0)); dp[0][0] = 1;
                rep(i, m){
                    vector pre(2, vector<ll>(2, 0)); swap(dp, pre);
                    rep(j, 2)rep(k, 2)rep(d, 10){
                        if (si <= i && i < si+n){
                            if (s[i-si]-'0' != d) continue;
                            if (d == 0 && k == 0) continue;
                        }
                        int nj = j, nk = k;
                        if (j == 0){
                            if (d < a[i]) nj = 1;
                            else if (d > a[i]) continue;
                        }
                        if (d) nk = 1;
                        dp[nj][nk] += pre[j][k];
                    }
                }
                res += dp[1][1] + dp[0][1];
            }
            return res;
        };
        cout << f(r)-f(l) << "\n";
    }
    cout.flush();
    return 0;
}