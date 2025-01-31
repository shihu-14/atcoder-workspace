// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
// using mint = modint998244353;
using mint = modint1000000007;
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
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i].val()<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j].val()<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

// ラグランジュ補間による多項式の復元(与えられる点が初項0,公差1の等差数列になっている前提でO(n))
struct Interpolation{
    int n;
    vector<mint> a; // 多項式の係数(a0,a1..an-1)
    Interpolation() {}
    Interpolation(vector<mint> &p){ // 異なるn個(x=0,1,,,の等差数列)からO(n)で、n-1次多項式を求める。
        n = p.size(); a.resize(n);
        vector<mint> invf(n, 1); // 係数aiを求めるために割り算を行う必要があるので前計算しておく。
        mint f = 1;
        for (int i = 1; i < n; ++i) f *= i;
        invf[n-1] = mint(1)/f;
        for (int i = n-1; i >= 1; --i) invf[i-1] = invf[i]*i;
        for (int i = 0; i < n; ++i) {
            mint b = invf[i]*invf[n-1-i];
            a[i] = p[i] * ((n-1-i)&1 ? -b : b); // 偶奇で符号を調整する。
        }
    }
    // 多項式が求まった前提で、一般にf(x)の値を求める。
    mint operator()(mint x) const {
        vector<mint> r(n,1); // 右からの累積和rと、左からの累積和lを求めることで、O(n)でf(x)を求めることができる。
        for (int i = n-1; i >= 1; --i) r[i-1] = r[i]*(x-i);
        mint l = 1, res;
        for (int i = 0; i < n; ++i) {
            res += a[i]*l*r[i];
            l *= x-i;
        }
        return res;
    }
};


mint power(mint a, ll n){
    mint res = 1;
    for(; n; n>>=1, a*=a) if(n&1) res *= a;
    return res;
}

ll power2(ll a, ll n, ll mod){
    ll res = 1;
    for(; n; n>>=1, a=(a*a)%mod) if(n&1) res = (res*a)%mod;
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m, k; cin >> n >> m >> k;
    vector<vector<mint>> table(k+m+1, vector<mint>(m+1));
    rep(i, k+m+1) table[i][0] = power(i, k);
    rep2(i, 1, k+m+1)rep2(j, 1, m+1){
        table[i][j] = table[i-1][j] + table[i][j-1];
    }
    vector<mint> a(k+m+1);
    rep(i, k+m+1) a[i] = table[i][m];
    Interpolation f(a);
    cout << f(n).val() << endl;
    return 0;
}