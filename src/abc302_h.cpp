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

struct Upd {
    int& a; int x;
    Upd(int& a): a(a), x(a) {}
};
struct RollbackUnionFind {
    int ans;
    vector<int> d, e, snap; // d: parent or -size
    vector<Upd> hist;
    RollbackUnionFind(int n=0): ans(0), d(n,-1), e(n, 0){}
    int find(int x) {
        if(d[x] < 0) return x;
        return find(d[x]);
    }
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    int f(int v){return min(-d[v], e[v]);}
    bool merge(int x, int y) { // if merge successed, return true
        x = find(x);
        y = find(y);
        hist.emplace_back(ans);
        if (x == y){
            hist.emplace_back(e[x]);
            e[x]++;
            if (e[x] == -d[x]) ans++;
            return false;
        }
        if (d[x] > d[y]) swap(x, y);
        hist.emplace_back(d[x]);
        hist.emplace_back(d[y]);
        hist.emplace_back(e[x]);
        ans -= f(x);
        ans -= f(y);
        d[x] += d[y];
        d[y] = x;
        e[x] += e[y]+1;
        ans += f(x);
        return true;
    }
    int size(int x){
        return -d[find(x)];
    }
    void snapshot(){ // save the history from this point onward
        snap.emplace_back(hist.size());
    }
    void rollback(){
        while (hist.size() > snap.back()){
            auto [a, x] = hist.back(); hist.pop_back();
            a = x;
        }
        snap.pop_back();
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n), b(n);
    G g(n);
    rep(i, n){
        cin >> a[i] >> b[i];
        a[i]--; b[i]--;
    }
    rep(i, n-1){
        int u, v; cin >> u >> v; u--, v--;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }

    vector<int> ans(n);
    RollbackUnionFind uf(n);
    auto dfs = [&](auto f, int v, int p=-1) -> void{
        uf.snapshot();
        uf.merge(a[v], b[v]);
        ans[v] = uf.ans;
        for (int u: g[v]){
            if (u == p) continue;
            f(f, u, v);
            uf.rollback();
        }
    };
    dfs(dfs, 0);
    rep2(i, 1, n) cout << ans[i] << " ";
    cout << endl;
    return 0;
}
