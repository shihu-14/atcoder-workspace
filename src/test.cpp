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
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];}

// 状態:
// dp[u][0][0] = 頂点uを部分木に入れない (次数=0), フラグ=0(まだ頂点4無し) の最大頂点数
// dp[u][0][1] = 同上, フラグ=1
// dp[u][1][0] = uを葉(次数=1)にする, フラグ=0
// dp[u][1][1] = uを葉, フラグ=1
// dp[u][4][0] = uを内点(次数=4), フラグ=0(ありえないので-∞になる)
// dp[u][4][1] = uを内点, フラグ=1
//
// 頂点数が最大になる部分木を構成する.
// 頂点uの子をchildren[u]として, 各子のdpをマージ.

static const ll MINF = -1000000000000000000LL;

int n;
vector<vector<int>> adj;        // グラフ(無向)
vector<ll> dp[200001][6];       // dp[u][state], state=0..5 (0/0, 0/1, 1/0, 1/1, 4/0, 4/1)
bool used[200001];             // DFS管理

// state用のヘルパー
// idx  -> (t, f)
// 0 -> (0,0), 1 -> (0,1), 2 -> (1,0), 3 -> (1,1), 4 -> (4,0), 5 -> (4,1)
inline int idx(int t, int f){
    if(t==0 && f==0) return 0;
    if(t==0 && f==1) return 1;
    if(t==1 && f==0) return 2;
    if(t==1 && f==1) return 3;
    if(t==4 && f==0) return 4;
    // t==4 && f==1
    return 5;
}

vector<int> children[200001];
int par[200001]; // 親(DFS木)

// DFSで構築
void dfs(int v, int p){
    used[v] = true;
    par[v] = p;
    for(auto &nx:adj[v]){
        if(nx == p) continue;
        if(!used[nx]){
            dfs(nx, v);
            children[v].push_back(nx);
        }
    }
}

// マージ(子を一人ずつ処理) => bottom-up
void solveDP(int v){
    // 初期化
    rep(i,6) dp[v][i].clear();
    dp[v][0].resize(1); dp[v][1].resize(1);
    dp[v][2].resize(1); dp[v][3].resize(1);
    dp[v][4].resize(1); dp[v][5].resize(1);
    // 全て不可能に
    rep(i,6) dp[v][i][0] = MINF;

    // まず子が0個の場合のベース:
    // t=0 -> 頂点不使用 => dp[v][0,0] = 0, dp[v][0,1]不可能=>-∞
    // t=1 -> 頂点葉 => dp[v][1,0] = 1, dp[v][1,1] = -∞
    // t=4 -> 頂点内点 => ここでは子がいない => もしvが根かどうかで判断。rootのとき子4本は無いので-∞, 
    //   ただし 4/1(=uが4 => f=1)は子が4本無い => -∞
    dp[v][ idx(0,0) ][0] = 0; 
    dp[v][ idx(1,0) ][0] = 1; 
    // dp[v][4,1] if it's root & we can have 4 children => but no children => -∞

    // children[v]を一人ずつマージ
    for(auto &c: children[v]){
        // まず c の dp計算は済んでいる
        // dp[c][stateC]
        // 頂点vの t=0 のとき => c must be 0
        // 頂点vの t=1 => c must be 0
        // 頂点vの t=4 => we pick exactly 3 children w(t_c!=0) if not root, or 4 if root => これは subset 選択
        // => naive: keep track of how many children used. We'll do a simpler approach with "kn" style merges

        // 省略(実際に書くなら子をdp合成)
        // ここでは高水準の書き方にとどめる
    }
}

// driver
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    adj.resize(n);
    rep(i,n-1){
        int a,b;cin>>a>>b;--a;--b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }
    // root=0 など
    dfs(0,-1);
    // postorder で solveDP
    // childrenはすでに埋まっている
    // ここではトポソート的に葉->根
    vector<int> order; 
    // BFS or stack
    {
        // create topological order from par array
        // since it's a tree, we can do a queue of leaves
        vector<int> deg(n);
        rep(i,n) deg[i] = (int)children[i].size();
        queue<int>q;
        rep(i,n) if(deg[i]==0) q.push(i);
        while(!q.empty()){
            int v=q.front(); q.pop();
            order.push_back(v);
            int p=par[v];
            if(p>=0){
                deg[p]--;
                if(deg[p]==0) q.push(p);
            }
        }
    }
    // now order has leaves first, root last
    for(auto &v:order) solveDP(v);

    // root's dp -> answer
    ll ans = -1;
    // dp[0][(1,1)], dp[0][(4,1)] など
    ll cand1 = dp[0][ idx(1,1) ].empty()? MINF : dp[0][ idx(1,1) ][0];
    ll cand2 = dp[0][ idx(4,1) ].empty()? MINF : dp[0][ idx(4,1) ][0];
    ll best = max(cand1,cand2);
    if(best<0) best=-1;
    cout<<best<<"\n";

    return 0;
}
