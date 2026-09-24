// #pragma GCC target("avx2")
// #pragma GCC optimize("O3")
// #pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using namespace boost::multiprecision;

// using mint = modint;
using mint = modint998244353;
// using mint = modint1000000007;

using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;

#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)
#define rep2(i, a, b) for (ll i = (ll)(a); i < (ll)(b); ++i)
#define rrep(i, n) for (ll i = (ll)(n) - 1; i >= 0; --i)
#define rrep2(i, a, b) for (ll i = (ll)(a) - 1; i >= (ll)(b); --i)
#define rep3(i, a, b, c) for (ll i = (ll)(a); i < (ll)(b); i += (ll)(c))
#define rng(a) (a).begin(), (a).end()
#define rrng(a) (a).rbegin(), (a).rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), (v).erase(unique(rng(v)), (v).end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v), 0LL)
#define IN(v, x) (find(rng(v), (x)) != (v).end())

template<class T> bool chmin(T& a, const T& b){if(a>b){a=b;return true;}return false;}
template<class T> bool chmax(T& a, const T& b){if(a<b){a=b;return true;}return false;}
template<class T> void printv(const vector<T>& v){rep(i,v.size()) cout<<v[i]<<" \n"[i==(ll)v.size()-1];}
template<class T> void printvv(const vector<vector<T>>& v){for(const auto& row:v) printv(row);}

constexpr ll dx[] = {-1, 0, 1, 0};
constexpr ll dy[] = {0, 1, 0, -1};
constexpr ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
constexpr ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
constexpr ll LINF = 3001002003004005006LL;
constexpr int INF = 1001001001;

// Trie木
struct Trie {
    struct Node { // Edit here (Define information for each node)
        array<int, 2> to = {-1, -1}; // next node
        int cnt = 0; // Node weight
    };
    vector<Node> g;
    Trie(): g(1) {}
    void add(const vector<int>& s) {
        int v = 0;
        g[v].cnt++;
        for (auto c: s){
            if (g[v].to[c] == -1){
                int sz = g.size();
                g.emplace_back(Node());
                g[v].to[c] = sz;
            }
            v = g[v].to[c];
            g[v].cnt++;
        }
        // Edit here (processing for v)
        // return v; // void -> int
    }
    void erase(const vector<int> &s)
    {
        int v = 0;
        g[v].cnt--;
        for (auto c: s){
            if (g[v].to[c] == -1){
                int sz = g.size();
                g.emplace_back(Node());
                g[v].to[c] = sz;
            }
            v = g[v].to[c];
            g[v].cnt--;
        }
        // Edit here (processing for v)
        // return v; // void -> int
    }
    // Edit here (DFS on the trie)
};

void solve()
{
    int n, m, K; cin >> n >> m >> K;
    vector<int> t(K);
    {
        string s; cin >> s;
        rep(i, s.size()) if (s[i] == 'x') t[i] = 1;
    }
    vector<vector<int>> g(n, vector<int>(K));
    Trie trie;
    rep(i, n)
    {
        string s; cin >> s;
        rep(j, s.size()) if (s[j] == 'x') g[i][j] = 1;
        trie.add(g[i]);
    }
    int q; cin >> q;
    rep(qi, q)
    {
        int i, j; cin >> i >> j; i--, j--;
        trie.erase(g[i]);
        g[i][j] ^= 1;
        trie.add(g[i]);
        int now = 0, sum = 0, flag = -1;
        rep(k, K)
        {
            int cnt = 0;
            if (trie.g[now].to[t[k]] != -1)
            {
                cnt = trie.g[trie.g[now].to[t[k]]].cnt;
            }
            if (sum + cnt <= m)
            {
                if (g[i][k] == t[k])
                {
                    flag = 1;
                    break;
                }
                sum += cnt;
            }
            else
            {
                if (g[i][k] != t[k])
                {
                    flag = 0;
                    break;
                }
            }
            now = trie.g[now].to[g[i][k]];
        }
        cout << (flag == 1 ? "Yes": "No") << '\n';
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--)
    {
        solve();
    }

    return 0;
}