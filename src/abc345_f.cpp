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

void solve(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    G g(n);
    map<pii, int> mp;
    vector<pii> edge;
    rep(i, m){
        int u, v; cin >> u >> v; u--, v--;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
        edge.emplace_back(u, v);
        mp[{u, v}] = mp[{v, u}] = i+1;
    }
    if (k&1){
        cout << "No" << endl;
        return;
    }
    int on_count = 0;
    vector<int> ans, on_list(n);
    vector<bool> used(n);
    rep(i, n){
        if (used[i]) continue;
        auto f = [&](auto f, int u, int p=-1) -> void{
            used[u] = true;
            for (int v: g[u]){
                if (used[v]) continue;
                f(f, v, u);
            }
            if (on_count < k){
                if (on_list[u]) on_count++;
                else if (p != -1){
                    on_count++;
                    on_list[u] ^= 1;
                    on_list[p] ^= 1;
                    ans.emplace_back(mp[{u, p}]);
                }
            }
            // if (on_count < k && !on_list[u] && p != -1){
            //     on_count += 2;
            //     on_list[u] ^= 1;
            //     on_list[p] ^= 1;
            //     if (!on_list[p]) on_count -= 2;
            //     ans.emplace_back(mp[{p, u}]);
            // }
        };
        f(f, i);
    }
    int c = 0;
    rep(i, n) if (on_list[i]) c++;
    // assert(on_count <= k && on_count % 2 == 0 && c == on_count);
    if (c != on_count){
        cout << n << " " << m << " " << k << endl;
        for (auto [u, v]: edge) cout << u+1 << " " << v+1 << endl;
        cout << "result" << endl;
        printv(on_list);
        cout << on_count << endl;
        cout << ans.size() << endl;
        rep(i, ans.size()) cout << ans[i] << " ";
        cout << endl;
        assert(0);
    }
    if (on_count != k){
        cout << "No" << endl;
        return;
    }
    
    cout << "Yes" << endl;
    // cout << ans.size() << endl;
    // printv(ans);
    return;
}

int main(){
    int t = 50;
    while(t--){
        solve();
    }
}
