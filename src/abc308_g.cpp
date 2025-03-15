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

// mapping: xor, op: min
template<class T=int, int k=30>
struct BinaryTrie{
    struct Node{
        array<int, 2> to;
        int cnt, dp; // Edit here
        Node(): to({-1, -1}), cnt(0), dp(0) {}
    };
    vector<Node> d; 
    BinaryTrie(): d(1){}
    int go(int v, int x){
        if (d[v].to[x] == -1){
            d[v].to[x] = d.size();
            d.emplace_back(Node());
        }
        return d[v].to[x];
    }
    void add(T x, int a){
        int v = 0;
        vector<int> vs;
        rrep2(i, k, 0){
            d[v].cnt += a;
            vs.emplace_back(v);
            v = go(v, x>>i&1);
        }
        // Edit below 
        d[v].cnt += a;
        if (d[v].cnt == 1) d[v].dp = x;
        else d[v].dp = 0;
        reverse(rng(vs));
        for (int v: vs){
            if (d[v].cnt == 0) continue;
            else if (d[v].cnt == 1){
                rep(i, 2){
                    int u = d[v].to[i];
                    if (u != -1 && d[u].cnt == 1) d[v].dp = d[u].dp;
                }
            }
            else{
                d[v].dp = 1<<k; // Edit here(1U, 1LL, 1ULL..)
                rep(i, 2){
                    int u = d[v].to[i];
                    if (u != -1 && d[u].cnt >= 2){
                        chmin(d[v].dp, d[u].dp); // Edit here (op for dp)
                    }
                }
                if (d[v].dp == 1<<k){
                    d[v].dp = 0;
                    rep(i, 2) d[v].dp ^= d[d[v].to[i]].dp; // Edit here (op for node)
                }
            }
        }
    }
}; 
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q; cin >> q;
    BinaryTrie bt; 
    rep(qi, q){
        int type; cin >> type;
        if (type == 1){
            int x; cin >> x;
            bt.add(x, 1);
        }
        else if (type == 2){
            int x; cin >> x;
            bt.add(x, -1);
        }
        else{
            cout << bt.d[0].dp << endl;
        }
    }
    return 0;
}