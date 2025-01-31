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
#define popcountll __builtin_popcountll
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
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

// Sparse Table 前処理 O(NlogN), クエリ O(1)
template<typename T>
struct SparseTable {
    int n;
    vector<vector<T>> table; // table[i][j] = dataの区間[i, i+2^j)の演算結果を格納
    vector<int> logtable; // floor(log2(i))を格納
    function<T(T,T)> op; // 2項演算子(結合性・冪等性を満たすもの)
    SparseTable() {}
    void build(const vector<T> &data, function<T(T,T)> f){
        n = data.size(); logtable.resize(n+1); op = f; // メンバ変数の初期化
        for(int i = 2; i <= n; i++) logtable[i] = logtable[i/2] + 1;
        int max_log = logtable[n];
        table.resize(n, vector<T>(max_log+1));
        for(int i = 0; i < n; i++) table[i][0] = data[i];
        for(int j = 1; j <= max_log; j++){
            for(int i = 0; i+(1<<(j-1)) < n; i++){
                if(i+(1<<j) > n) break;
                table[i][j] = op(table[i][j-1], table[i+(1<<(j-1))][j-1]);
            }
        }
    }
    // dataの区間[l, r)の演算結果を返す
    T query(int l, int r){
        int ql = r-l;
        assert(ql > 0 && ql <= n);
        int k = logtable[ql];
        return op(table[l][k], table[r-(1<<k)][k]);
    }
};

int op(int a, int b){return max(a, b);}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n;
    vector<int> a(n), x(n);
    rep(i, n) cin >> a[i];
    rep(i, n) x[i] = lower_bound(rng(a), 2*a[i]) - a.begin(), x[i] -= i;
    SparseTable<int> st; st.build(x, op);
    cin >> q;
    rep(i, q){
        int l, r; cin >> l >> r; l--;
        auto f = [&](int w) -> bool{
            int mx = st.query(l, l+w);
            return mx <= r-l-w;
        };
        int ac = 0, wa = (r-l)/2+1;
        while(wa-ac>1){
            int wj = (ac+wa)/2;
            if (f(wj)) ac = wj;
            else wa = wj;
        }
        cout << ac << '\n';
    }
    cout.flush();
    return 0;
}