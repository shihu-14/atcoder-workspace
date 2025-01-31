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
    int h, w; cin >> h >> w;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    vector<vector<int>> g1(h), g2(w);
    rep(i, h) g1[i].emplace_back(-1);
    rep(i, w) g2[i].emplace_back(-1);
    rep(i, h)rep(j, w){
        if (g[i][j] == '#') g1[i].emplace_back(j);
        if (g[i][j] == '#') g2[j].emplace_back(i);
    }
    rep(i, h) g1[i].emplace_back(w);
    rep(i, w) g2[i].emplace_back(h);

    int ans = 0;
    rep(i, h)rep(j, w){
        if (g[i][j] == '#') continue;
        int l1 = upper_bound(rng(g1[i]), j) - g1[i].begin();
        l1--;
        int l2 = upper_bound(rng(g1[i]), j) - g1[i].begin();
        int r1 = upper_bound(rng(g2[j]), i) - g2[j].begin();
        r1--;
        int r2 = upper_bound(rng(g2[j]), i) - g2[j].begin();
        int res = 0;
        res += (g1[i][l2]-g1[i][l1]-1);
        res += (g2[j][r2]-g2[j][r1]-1);
        res--;
        chmax(ans, res);
    }
    cout << ans << endl;
    return 0;
}