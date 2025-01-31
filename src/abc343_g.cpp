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
#define popcount_ll __builtin_popcountll
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
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<string> s(n);
    rep(i, n) cin >> s[i];
    // UNIQUE(s);
    // n = s.size();
    // vector<string> ns;
    // rep(i, n){
    //     bool flag = true;
    //     rep(j, n){
    //         if (i == j) continue;
    //         if (s[j].find(s[i]) != string::npos){
    //             flag = false;
    //             break;
    //         }
    //     }
    //     if (flag) ns.emplace_back(s[i]);
    // }
    {
        sort(s.begin(), s.end(), [&](string& a, string& b) {
            return a.size() > b.size();
        });
        vector<string> new_s;
        for (string ns : s) {
            string t = ns + "$";
            for (string nt : new_s) t += nt + "#";
            bool ok = true;
            auto z = z_algorithm(t);
            for (int i = ns.size(); i < t.size(); i++) {
            if (z[i] == ns.size()) ok = false;
            }
            if (ok) new_s.push_back(ns);
        }
        s = new_s;
        n = s.size();
    }
    vector<vector<int>> g(n, vector<int>(n));
    rep(i, n){
        rep(j, n)if (i != j){
            string t = s[i]+"$"+s[j];
            auto za = z_algorithm(t);
            g[j][i] = s[i].size();
            rep2(k, s[i].size()+1, za.size()){
                if (za.size()-k == za[k]){
                    g[j][i] = s[i].size()-za[k];
                    break;
                }
            }
        }
    }
    vector dp(1<<n, vector<int>(n, INF));
    rep(i, n) dp[1<<i][i] = s[i].size();
    rep(s, 1<<n)rep(u, n){
        if (~s>>u&1) continue;
        rep(v, n){
            if (s>>v&1) continue;
            chmin(dp[s|(1<<v)][v], dp[s][u]+g[u][v]);
        }
    }
    int ans = INF;
    rep(v, n) chmin(ans, dp[(1<<n)-1][v]);
    cout << ans << endl;
    return 0;

}