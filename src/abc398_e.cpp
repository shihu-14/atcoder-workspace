// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
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
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    G g(n);
    vector<int> a(n);
    vector<vector<bool>> used(n, vector<bool>(n));
    rep(i, n-1){
        int u, v; cin >> u >> v; u--, v--;
        g[u].push_back(v);
        g[v].push_back(u);
        used[u][v] = 1;
    }
    {
        auto dfs = [&](auto f, int u, int p=-1) -> void{
            for (auto v: g[u]){
                if (v == p) continue;
                a[v] = a[u]^1;
                f(f, v, u);
            }
        };
        dfs(dfs, 0);
    }

    vector<pii> ans;
    rep(i, n)rep2(j, i+1, n){
        if (a[i] == a[j]) continue;
        if (used[i][j]) continue;
        ans.emplace_back(i, j);
        used[i][j] = 1;
    }
    int m = ans.size();
    cout << (m&1 ? "First": "Second") << endl;
    vector<vector<bool>> used2(n, vector<bool>(n));
    if (~m&1){
        int nu, nv;
        cin >> nu >> nv;
        if (nu == -1 && nv == -1) return 0;
        nu--, nv--;
        used2[nu][nv] = 1;
    }
    for (auto [u, v]: ans){
        if (u > v) swap(u, v);
        if (used2[u][v]) continue;
        used2[u][v] = 1;
        cout << u+1 << " " << v+1 << endl;
        int nu, nv;
        cin >> nu >> nv;
        if (nu == -1 && nv == -1) break;
        nu--, nv--;
        used2[nu][nv] = 1;
    }
    return 0;
}