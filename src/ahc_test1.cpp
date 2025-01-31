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
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n = 26;
    int D; cin >> D;
    vector<int> c(n), last(n);
    rep(i, n) cin >> c[i];
    vector<vector<int>> s(D, vector<int>(n));
    rep(i, D)rep(j, n) cin >> s[i][j]; 
    vector<int> ans;
    rep(d, D){
        int max_res = -INF, max_id = -1;
        rep(j, n){
            int res = s[d][j];
            rep(k, n){
                if (k == j) continue;
                res -= c[k]*(d+1-last[k]);
            }
            if (res > max_res){
                max_res = res;
                max_id = j;
            }
        }
        last[max_id] = d+1;
        ans.emplace_back(max_id);
    }
    rep(i, D) cout << ans[i]+1 << endl;
    return 0;
}