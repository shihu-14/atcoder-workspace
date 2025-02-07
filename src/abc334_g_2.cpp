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
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

struct RollbackUnionFind {
    vector<int> d, snap; // d: parent or -size
    vector<pii> hist;
    RollbackUnionFind(int n=0): d(n,-1) {}
    int find(int x) {
        if(d[x] < 0) return x;
        return find(d[x]);
    }
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    bool merge(int x, int y) { // if merge successed, return true
        x = find(x);
        y = find(y);
        if (x == y) return false;
        if (d[x] > d[y]) swap(x, y);
        hist.emplace_back(x, d[x]);
        hist.emplace_back(y, d[y]);
        d[x] += d[y];
        d[y] = x;
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
            auto [i, sz] = hist.back(); hist.pop_back();
            d[i] = sz;
        }
        snap.pop_back();
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w; cin >> h >> w;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    int n = 0;
    vector<pii> nodes;
    vector<vector<int>> id(h, vector<int>(w, -1));
    rep(i, h)rep(j, w){
        if (g[i][j] == '#') nodes.emplace_back(i, j), id[i][j] = n++;
    }
    RollbackUnionFind uf(n);
    mint ans;
    int cnt = 0;
    vector<bool> used(n);
    auto add = [&](int k){
        cnt++;
        auto [i, j] = nodes[k];
        rep(t, 4){
            int ni = i+dx[t], nj = j+dy[t];
            if (ni < 0 || ni >= h || nj < 0 || nj >= w || g[ni][nj] != '#') continue;
            if (!used[id[ni][nj]]) continue;
            if (uf.merge(id[i][j], id[ni][nj])) cnt--;
        }
        used[k] = true;
    };
    auto dfs = [&](auto f, int l, int r){
        if (r-l == 1){
            ans += cnt;
            return;
        }
        int m = (l+r)/2;
        {
            int tmp = cnt;
            uf.snapshot();
            rep2(i, m, r) add(i);
            f(f, l, m);
            uf.rollback();
            rep2(i, m, r) used[i] = false;
            cnt = tmp;
        }
        {
            int tmp = cnt;
            uf.snapshot();
            rep2(i, l, m) add(i);
            f(f, m, r);
            uf.rollback();
            rep2(i, l, m) used[i] = false;
            cnt = tmp;
        }
    };  
    dfs(dfs, 0, n);
    ans /= n;
    cout << ans.val() << endl;
    return 0;
}