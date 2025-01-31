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

// i文字目以降で(1-index)文字cが初めて登場する0-indexを返す。(存在しない場合は-1を返す)
vector<vector<int>> nex(string s){
    int n = s.size();
    vector<vector<int>> res(n+1, vector<int>(26, -1));
    for(int i = n-1; i >= 0; i--){
        res[i] = res[i+1];
        res[i][s[i]-'a'] = i+1; // 適宜1-indexに変更可能i->i+1
    }
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s; cin >> s;
    int n = s.size();
    auto nxt = nex(s);
    mint ans = 0;
    rep2(t, 1, n){
        vector<vector<mint>> dp(n+1, vector<mint>(n+1));
        dp[0][t] = 1;
        rep2(i, 0, t)rep2(j, t, n){
            rep(c, 26){
                int to1 = nxt[i][c], to2 = nxt[j][c];
                if (to1 == -1 || to2 == -1 || t < to1) continue;
                dp[to1][to2] += dp[i][j];
            }
        }
        rep2(i, t+1, n+1) ans += dp[t][i];
    }
    cout << ans.val() << endl;
    return 0;
}   