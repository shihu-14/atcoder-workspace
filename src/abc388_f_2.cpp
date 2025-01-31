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
#define popcount_ll __builtin_popcountll
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

struct bit64_Matrix{
    int row, col=1;
    vector<uint64_t> mat;
    vector<vector<uint64_t>> exp, exp_trans;
    bit64_Matrix(vector<uint64_t> &v): row(v.size()), mat(v), exp(64, vector<uint64_t>(row)), exp_trans(64, vector<uint64_t>(row)){
        assert(row <= 64);
        vector<uint64_t> mat_trans(row);
        for (int i = 0; i < row; i++){
            for (int j = 0; j < row; j++){
                mat_trans[row-1-i] |= ((mat[j]>>i)&1ULL)<<(row-1-j);
            }
        }
        exp[0] = mat, exp_trans[0] = mat_trans;
        for (int i = 1; i < 64; i++){
            for (int j = 0; j < row; j++){
                for (int k = 0; k < row; k++){
                    exp[i][j] |= (ull)((exp[i-1][j]&exp_trans[i-1][k])!=0)<<(row-1-k);
                    exp_trans[i][j] |= (ull)((exp_trans[i-1][j]&exp[i-1][k])!=0)<<(row-1-k);
                }
            }
        }
    }
    uint64_t power(uint64_t a, ull b) const{ // O(row*logb) 前処理 O(row^2*64)
        for (int i = 0; b ; b>>=1, i++) if(b&1) {
            uint64_t na = 0;
            for (int j = 0; j < row; j++){
                na |= (ull)((exp[i][j]&a)!=0)<<(row-1-j);
            }
            a = na;
        }
        return a;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n; int m, a, b; cin >> n >> m >> a >> b;
    vector<uint64_t> p(b), q(b);
    for (int i = b-1; i >= 1; i--){
        p[i] = q[i] = 1ULL<<(b-i);
    }
    rep2(i, a-1, b) p[0] |= 1ULL<<(b-1-i);

    bit64_Matrix X(p), Y(q);
    uint64_t ans = 1ULL<<(b-1);
    ll nx = 0;
    rep(i, m){
        ll l, r; cin >> l >> r; l--;
        ans = X.power(ans, l-nx-1);
        ans = Y.power(ans, r-l);
        nx = r-1;
    }
    ans = X.power(ans, n-nx-1);
    cout << ((ans>>(b-1)&1) == 1 ? "Yes" : "No") << endl;
    return 0;
}