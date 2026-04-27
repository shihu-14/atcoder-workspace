#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <climits>
#include <cfloat>
#include <cassert>
#include <ctime>
#include <cctype>
#include <cwctype>
#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <utility>
#include <bitset>
#include <vector>
#include <deque>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <tuple>
#include <array>
#include <new>
#include <memory>
#include <limits>
#include <random>
#include <exception>
#include <stdexcept>
#include <regex>
#include <complex>
#include <chrono>
#include <future>
#include <thread>
#include <mutex>
#include <atomic>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using mint = modint;
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
#define rrep2(i, a, b) for (ll i = int(a)-1; i >= (b); --i)
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

// binary trie
template<class T=ll, int k=60>
struct BinaryTrie
{
    struct Node
    {
        array<int, 2> to;
        int cnt;
        // add other variables
        Node(): to({-1, -1}), cnt(0) {}
    };
    vector<Node> d; 
    BinaryTrie(): d(1){}
    int go(int v, int x)
    {
        if (d[v].to[x] == -1)
        {
            d[v].to[x] = d.size();
            d.emplace_back(Node());
        }
        return d[v].to[x];
    }
    void add(T x, int a=1)
    {
        int v = 0;
        vector<int> vs;
        for (int i = k-1; i >= 0; --i){
            d[v].cnt += a;
            vs.emplace_back(v);
            v = go(v, x>>i&1);
        }
        d[v].cnt += a;
    }
    // add other functions
    long long f(T val, T K)
    {
        int v = 0;
        long long res = 0;
        for (int i = k - 1; i >= 0; --i) {
            if (v == -1) break; // もう辿れる枝がない
            
            int b_val = (val >> i) & 1;
            int b_K = (K >> i) & 1;

            if (b_K == 1) {
                // Kのビットが1のとき：XORが0になる方向の部分木は、全員Kより小さくなることが確定！
                if (d[v].to[b_val] != -1) {
                    res += d[d[v].to[b_val]].cnt; // 丸ごと足す（ショートカット）
                }
                // XORが1になる方向へ進んで、下の桁の判定へ望みを繋ぐ
                v = d[v].to[1 - b_val];
            } else {
                // Kのビットが0のとき：XORが0になる方向にしか進めない（1になるとKを超えて即アウト）
                v = d[v].to[b_val];
            }
        }
        // 最後まで辿り着けた奴らは「ぴったりK」になった奴ら
        if (v != -1) res += d[v].cnt; 
        return res;
    }
};


struct bit_Matrix {
    using BS = bitset<64>; // Edit bitsize (default 64)
    int h, w;
    vector<BS> d;
    vector<vector<BS>> exp_d;
    
    bit_Matrix(int h, int w): h(h), w(w), d(h), exp_d(64, vector<BS>(h)) {}
    
    BS operator[](int i) const { return d[i]; }
    BS& operator[](int i) { return d[i]; }

    void build() { // build for using power()
        assert(h == w); 
        exp_d[0] = d;
        vector<BS> d_trans(h);
        for(int i = 0; i < h; i++) for(int j = 0; j < h; j++) d_trans[i][j] = d[j][i];
        
        for(int t = 1; t < 64; t++) {
            for(int i = 0; i < h; i++) {
                for(int j = 0; j < h; j++) {
                    exp_d[t][i][j] = (exp_d[t-1][i] & d_trans[j]).any();
                }
            }
            for(int i = 0; i < h; i++) {
                for(int j = 0; j < h; j++) {
                    d_trans[j][i] = exp_d[t][i][j];
                }
            }
        }
    }

    BS power(BS x, long long n) { // calc d^(n)*x
        assert(h == w); 
        BS res = x;
        for (int i = 0; n > 0; n >>= 1, i++) {
            if (n & 1) {
                BS nxt;
                for(int j = 0; j < h; j++) nxt[j] = (exp_d[i][j] & res).any();
                res = nxt;
            }
        }
        return res;
    }

    // 行列を簡約化し、行列のrankを返す
    // limit_col: 掃き出しの対象とする列数の上限（デフォルトは w）
    int rref(int limit_col = -1) {
        if (limit_col == -1) limit_col = w;
        int rank = 0;
        for (int j = 0; j < limit_col; j++) {
            int pivot = -1;
            for (int i = rank; i < h; i++) {
                if (d[i][j]) { pivot = i; break; }
            }
            if (pivot == -1) continue;
            if (rank != pivot) swap(d[rank], d[pivot]);
            for (int i = 0; i < h; i++) {
                if (i != rank && d[i][j]) d[i] ^= d[rank];
            }
            rank++;
            if (rank == h) break;
        }
        return rank;
    }

    // treat matrix 'd' as augmented coefficient matrix and solve
    BS solve() { 
        int rank = rref(w - 1); 
        
        BS nan, res; 
        nan[w - 1] = 1;
        
        for (int i = 0; i < h; i++) {
            int j = 0;
            while (j < w - 1 && !d[i][j]) j++;
            
            if (j == w - 1) { 
                if (d[i][w - 1]) return nan; 
                continue;
            }
            res[j] = d[i][w - 1];
        }
        return res;
    }
};
ostream& operator<<(ostream&o,const bit_Matrix&a) {
    rep(i,a.h) o<<a[i]<<endl;
    return o;
}
/* testcase

abc388_f
abc366_g

*/

void solve() 
{
    ll n, m, k; cin >> n >> m >> k;
    vector<T3> edges;
    vector<vector<pll>> g(n);
    {
        dsu uf(n);
        rep(i, m)
        {
            int u, v; ll w; cin >> u >> v >> w;
            u--; v--;
            
            if (uf.same(u,v))
            {
                edges.emplace_back(u, v, w);
            }
            else
            {
                g[u].emplace_back(v, w);
                g[v].emplace_back(u, w);
                uf.merge(u, v);
            }
        }
    }
    vector<ll> dist(n), cycles;
    {
        auto dfs = [&](auto f, int v, int p=-1) -> void
        {
            for (auto &[u, w] : g[v])
            {
                if (u == p) continue;
                dist[u] = dist[v] ^ w;
                f(f, u, v);
            }
        };
        dfs(dfs, 0);
        for (auto [u, v, w] : edges)
        {
            cycles.emplace_back(dist[u] ^ dist[v] ^ w);
        }
    }

    // bit_Matrix mat(cycles.size(), 64);
    // rep(i, cycles.size())
    // {
    //     rep(j, 64)
    //     {
    //         if (cycles[i] >> j & 1) mat[i][j] = 1;
    //     }
    // }
    // mat.rref();
    // rep(i, n)
    // {
    //     rep(j, mat.d.size())
    //     {

    //     }
    // }
    vector<ll> basis;
    for (ll c : cycles)
    {
        for (ll b : basis)
        {
            chmin(c, c ^ b);
        }
        if (c)
        {
            basis.push_back(c);
            sort(rrng(basis)); 
        }
    }
    rep(i, n)
    { 
        for (ll b : basis)
        {
            chmin(dist[i], dist[i] ^ b);
        }
    }

    ll ans = 0;
    BinaryTrie trie;
    rep(i, n)
    {
        ans += trie.f(dist[i], k);
        trie.add(dist[i]);
    }
    cout << ans << endl;
}



int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    cin >> t;
    while(t--)
    {
        solve();
    }
    return 0;
}