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
    reverse(rng(s));
    s += '0';
    int n = s.size();
    vector<ll> dp(2, INF); dp[0] = 0;
    rep(i, n){
        vector<ll> pre(2, INF); swap(pre, dp);
        rep(j, 2){
            char c = s[i];
            if (j == 0){
                chmin(dp[0], pre[j] + (c-'0'));
                chmin(dp[1], pre[j] + 10-(c-'0'));
            }
            else{
                int d = 1+(c-'0');
                if (d >= 10){
                    chmin(dp[1], pre[j]);
                }
                else{
                    chmin(dp[0], pre[j] + d);
                    chmin(dp[1], pre[j] + 10-d);
                }
            }
        }
    }
    cout << min(dp[0], dp[1]) << endl;
    return 0;
}