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
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]+1<<" \n"[i==v.size()-1];}
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
    int n; cin >> n;
    scc_graph g(n);
    G g2(n), grev(n);
    vector<int> a(n);
    rep(i, n){
        int s; cin >> a[i] >> s; s--; a[i] *= 2;
        g.add_edge(s, i);
        g2[s].emplace_back(i);
        grev[i].emplace_back(s);
    }

    auto scc = g.scc();
    int ans = 0;
    for (auto vec: scc){
        if (vec.size() == 1){
            int u = vec[0];
            int flag = 0;
            for(auto v: grev[u]){
                if (v != u) flag++;
            }
            if (flag) ans += a[u]/2;
            else ans += a[u];
            continue;
        }
        int sum = 0, min_s = INF;
        for (auto v: vec) sum += a[v]/2;
        for (auto v: vec) chmin(min_s, sum+a[v]/2);
        ans += min_s;
    }
    if (ans&1) cout << ans/2 << ".5" << endl;
    else cout << ans/2 << ".0" << endl;
    return 0;
}