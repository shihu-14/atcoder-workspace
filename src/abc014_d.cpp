#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < n; ++i)
#define rep2(i, a, b) for (ll i = a; i < b; ++i)
#define rep3(i, a, b, c) for (ll i = a; i < b; i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
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

using S = pair<int, int>;
S op(S a, S b){return min(a, b);}
S e(){return {INF, 0};}
// オイラーツアーとセグ木を用いてLCAを求める。
struct LCA{
    vector<pair<int,int>> eular_tour;
    vector<int> first_appear, dist;
    vector<vector<int>> g;
    segtree<S, op, e> seg;
    LCA(int n) : first_appear(n), dist(n), g(n), seg(2*n-1){}
    void add_edge(int a, int b){
        g[a].push_back(b);
        g[b].push_back(a);
    }
    // オイラーツアーを求める。
    void dfs(int now, int pre=-1, int d=0){
        first_appear[now] = eular_tour.size();
        dist[now] = d;
        eular_tour.emplace_back(d, now);
        for (auto to: g[now]){
            if (to == pre) continue;
            dfs(to, now, d+1);
            eular_tour.emplace_back(d, now);
        }
    };
    // 根を指定してオイラーツアー、seg木を初期化する。
    void init(int root){
        dfs(root);
        for(int i=0; i< eular_tour.size(); i++) seg.set(i, eular_tour[i]);
    }
    // LCAを求める。
    int lca(int a, int b){
        int l = first_appear[a], r = first_appear[b];
        if (l > r) swap(l, r);
        return seg.prod(l, r+1).second;
    }
    // 任意の2点間の距離を求める。
    int length(int a, int b){return dist[a] + dist[b] - 2*dist[lca(a, b)];}
    // ある点が指定したパス上(a,b)にあるかどうかの判定をする。
    bool is_on_pass(int a, int b, int c){return length(a, b) == length(a, c)+length(c, b);}
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n;
    LCA g(n);
    rep(i, n-1){
        int a, b; cin >> a >> b;
        --a, --b;
        g.add_edge(a, b);
    }
    g.init(0);
    cin >> q;
    rep(i, q){
        int a, b; cin >> a >> b;
        --a, --b;
        cout << g.length(a, b)+1 << endl;
    }
    return 0;
}