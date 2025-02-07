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
// bridge and articulation detection
struct Lowlink{
    struct Edge{
        int to, id;
        Edge() {}
        Edge(int to, int id=-1): to(to), id(id) {}
    };
    int n, m=0;
    vector<int> par, ord, low, bc_id; // bc_id; edge set name after partition
    vector<bool> art;
    vector<vector<Edge>> g;
    Lowlink(int n): n(n), par(n, -1), ord(n,-1), low(n), art(n), g(n){}
    void add_edge(int a, int b){
        if (a > b) swap(a, b);
        g[a].emplace_back(b, m);
        g[b].emplace_back(a, m++);
    }
    void build(){
        int c = 0;
        auto dfs = [&](auto f, int v, int p=-1) -> void{
            low[v] = ord[v] = c++;
            bool isart = false;
            for(auto e: g[v]){
                if (ord[e.to] == -1){
                    par[e.to] = v;
                    f(f, e.to, v);
                    chmin(low[v], low[e.to]);
                    isart |= ord[v] <= low[e.to];
                } 
                else if (e.to != p){
                    chmin(low[v], ord[e.to]);
                }
            }
            art[v] = isart;
        };
        rep(v, n) if (ord[v] == -1){
            int num_child = 0;
            low[v] = ord[v] = c++;
            for (auto e: g[v]) if (ord[e.to] == -1){
                par[e.to] = v;
                dfs(dfs, e.to, v);
                num_child++;
            }
            if (num_child >= 2) art[v] = true;
        }
    }
    bool isbridge(int a, int b){
        if (ord[a] > ord[b]) swap(a, b);
        return ord[a] < low[b];
    }
    bool isart(int v){return art[v];}
    // edge set partition by articulation
    vector<vector<int>> bcc(){
        vector<vector<int>> bc;
        bc_id = vector<int>(m,-1);
        auto add = [&](int ei, int k) {
            bc[k].push_back(ei);
            bc_id[ei] = k;
        };
        auto dfs = [&](auto f, int v, int k=-1) -> void {
            for (auto e : g[v]) if (e.to != par[v]){
                if (par[e.to] == v) {
                    int nk = k;
                    if (low[e.to] >= ord[v]) nk = bc.size(), bc.emplace_back();
                    add(e.id, nk);
                    f(f, e.to, nk);
                }
                else if (ord[e.to] < ord[v]) add(e.id, k);
            }
        };
        rep(v, n) if (par[v] == -1) dfs(dfs, v);
        return bc;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w; cin >> h >> w;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    int n = 0;
    map<pii, int> v_id;
    rep(i, h)rep(j, w){
        if (g[i][j] == '#'){
            v_id[{i, j}] = n++;
        }
    }
    Lowlink lnk(n);
    rep(i, h)rep(j, w){
        if (g[i][j] == '.') continue; 
        rep2(t, 1, 3){
            int ni = i+dx[t], nj = j+dy[t];
            if (ni < 0 || ni >= h || nj < 0 || nj >= w || g[ni][nj] == '.') continue;
            lnk.add_edge(v_id[{i, j}], v_id[{ni, nj}]);
        }
    }
    lnk.build();
    // lnk.print();
    lnk.bcc();
    mint ans;
    int bs = 0;
    rep(i, n) if (lnk.par[i] == -1) bs++;
    rep(v, n){
        int res = bs;
        set<int> st;
        for (auto e: lnk.g[v]){
            st.insert(lnk.bc_id[e.id]);
        }
        res += st.size()-1;
        ans += res;
    }
    ans /= n;
    cout << ans.val() << endl;
    return 0;
}