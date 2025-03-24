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
// Sparse Table 前処理 O(NlogN), クエリ O(1)
template<typename T>
struct SparseTable {
    int n;
    vector<vector<T>> table; // table[i][j] = dataの区間[i, i+2^j)の演算結果を格納
    vector<int> logtable; // floor(log2(i))を格納
    function<T(T,T)> op; // 2項演算子(結合性・冪等性を満たすもの)
    T e;
    SparseTable() {}
    void build(const vector<T> &data, function<T(T,T)> f, T _e){
        n = data.size(); logtable.resize(n+1); op = f; e = _e; // メンバ変数の初期化
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
        if (ql <= 0 || ql > n) return e;
        int k = logtable[ql];
        return op(table[l][k], table[r-(1<<k)][k]);
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
    vector<pii> p;
    rep(i, n){
        int v; cin >> v;
        p.emplace_back(v, i);
    }
    SparseTable<pii> st;
    st.build(p, [](pii a, pii b){return min(a, b);}, make_pair(INF, -1));
    vector<int> ans;
    {
        int l = 0, tmp_k = k;
        vector<int> tmp_ans;
        while(l < n){
            auto [mn, j] = st.query(l, min(n, l+tmp_k+1));
            if (mn == INF) break;
            tmp_ans.emplace_back(mn);
            tmp_k -= j-l;
            l = j+1;
        }
        if (tmp_k == 0) rep2(i, l, n) tmp_ans.emplace_back(p[i].fi);
        else if (l == n) rep(i, tmp_k) tmp_ans.pop_back();
        ans = tmp_ans;
    }
    {
        int l1 = 0, l2 = 0, r = 0, tmp_k = k;
        vector<int> tmp_ans;
        auto [mn, j] = st.query(n-k, n);
        if (mn == INF){
            printv(ans);
            return 0;
        }
        tmp_ans.emplace_back(mn);
        tmp_k -= n-j;
        r = j;
        l2 = j+1;
        while(l1 < r){
            auto [mn1, j1] = st.query(l1, min(r, l1+tmp_k+1));
            auto [mn2, j2] = st.query(l2, n);
            if (mn1 == INF) break;
            if (mn1 > mn2){
                tmp_ans.emplace_back(mn2);
                l2 = j2+1;
            }
            else{
                tmp_ans.emplace_back(mn1);
                l2 = n;
                tmp_k -= j1-l1;
                l1 = j1+1;
            }
        }
        if (tmp_k == 0) rep2(i, l1, r) tmp_ans.emplace_back(p[i].fi);
        else if (l1 == r) rep(i, tmp_k) tmp_ans.pop_back();
        chmin(ans, tmp_ans);
    }
    printv(ans);
    return 0;
}