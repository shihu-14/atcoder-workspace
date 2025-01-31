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
    int n, q; cin >> n >> q;
    vector<ll> dp(n, LINF);
    dp[1] = 0;
    char ph = 'L'; int pt = 0;
    rep(qi, q){
        vector<ll> pre(n, LINF); swap(dp, pre);
        char h; int t; cin >> h >> t; t--;
        rotate(pre.begin(), pre.begin()+pt, pre.end());
        rotate(dp.begin(), dp.begin()+pt, dp.end());
        int tmp = t;
        t = (t-pt+n)%n;
        rep(cw, 2){
            rep2(x,1,n){
                if (ph == h){
                    if (x <= t) chmin(dp[(t+1)%n], pre[x]+t+(t+1-x));
                    else chmin(dp[x], pre[x]+t);
                }
                else{
                    if (x <= t) chmin(dp[0], pre[x]+t-x);
                    else chmin(dp[(t+1)%n], pre[x]+n-(x-t)+t+1);
                }
            }
            t = (n-t)%n;
            reverse(pre.begin()+1, pre.end());
            reverse(dp.begin()+1, dp.end());
        }

        rotate(pre.begin(), pre.begin()+n-pt, pre.end());
        rotate(dp.begin(), dp.begin()+n-pt, dp.end());
        pt = tmp, ph = h;
    }

    ll ans = LINF;
    rep(i, n) chmin(ans, dp[i]);
    cout << ans << endl;
    return 0;
}