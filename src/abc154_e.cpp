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
    string s; cin >> s;
    int k; cin >> k;
    int n = s.size();
    ll ans = 0;
    auto f = [&](int n, int r) -> ll{
        if (n < r || n < 0) return 0;
        if (r == 0) return 1;
        if (r == 1) return n;
        if (r == 2) return n*(n-1)/2;
        return n*(n-1)/2*(n-2)/3;
    };
    auto g = [&](int a) -> ll{
        if (a == 0) return 1;
        if (a == 1) return 9;
        if (a == 2) return 9*9;
        return 9*9*9;
    };
    rep2(d, 1, n){
        if (d < k) continue;
        ans += f(d-1, k-1)*g(k);
    }
    vector dp(2, vector<ll>(k+1, 0)); dp[0][1] = 1, dp[1][1] = (s[0]-'0')-1;
    rep2(i, 1, n){
        vector pre(2, vector<ll>(k+1, 0)); swap(dp, pre);
        rep(j, 2){
            if (j == 0){
                int d = s[i] - '0';
                rep(c, k+1){
                    if (d != 0) dp[1][c] += pre[j][c];
                    else dp[j][c] += pre[j][c];

                    if (c+1 <= k && d != 0){
                        dp[1][c+1] += pre[j][c]*(d-1);
                        dp[0][c+1] += pre[j][c];
                    }
                }
            }
            else{
                rep(c, k+1){
                    dp[j][c] += pre[j][c];
                    if (c+1 <= k){
                        dp[j][c+1] += pre[j][c]*9;
                    }
                }
            }
        }
    }
    ans += dp[1][k];
    int tmp = 0;
    rep(i, n){
        if (s[i] != '0') tmp++;
    }
    if (tmp == k) ans++;
    cout << ans << endl;
    return 0;
}