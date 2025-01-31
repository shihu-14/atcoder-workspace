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
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;

// https://github.com/atcoder/live_library/blob/master/fps.cpp
// Formal Power Series
using vm = vector<mint>;
struct fps : vm {
#define d (*this)
#define s int(vm::size())
  template<class...Args> fps(Args...args): vm(args...) {}
  fps(initializer_list<mint> a): vm(a.begin(),a.end()) {}
  void rsz(int n) { if (s < n) resize(n);}
  fps& low_(int n) { resize(n); return d;}
  fps low(int n) const { return fps(d).low_(n);}
  mint& operator[](int i) { rsz(i+1); return vm::operator[](i);}
  mint operator[](int i) const { return i<s ? vm::operator[](i) : 0;}
  mint operator()(mint x) const {
    mint r;
    for (int i = s-1; i >= 0; --i) r = r*x+d[i];
    return r;
  }
  fps operator-() const { fps r(d); rep(i,s) r[i] = -r[i]; return r;}
  fps& operator+=(const fps& a) { rsz(a.size()); rep(i,a.size()) d[i] += a[i]; return d;}
  fps& operator-=(const fps& a) { rsz(a.size()); rep(i,a.size()) d[i] -= a[i]; return d;}
  fps& operator*=(const fps& a) { return d = convolution(d, a);}
  fps& operator*=(mint a) { rep(i,s) d[i] *= a; return d;}
  fps& operator/=(mint a) { rep(i,s) d[i] /= a; return d;}
  fps operator+(const fps& a) const { return fps(d) += a;}
  fps operator-(const fps& a) const { return fps(d) -= a;}
  fps operator*(const fps& a) const { return fps(d) *= a;}
  fps operator*(mint a) const { return fps(d) *= a;}
  fps operator/(mint a) const { return fps(d) /= a;}
  fps operator~() const {
    fps r({d[0].inv()});
    for (int i = 1; i < s; i <<= 1) r = r*mint(2) - (r*r*low(i<<1)).low(i<<1);
    return r.low_(s);
  }
  fps& operator/=(const fps& a) { int w = s; d *= ~a; return d.low_(w);}
  fps operator/(const fps& a) const { return fps(d) /= a;}
  fps integ() const {
    fps r;
    rep(i,s) r[i+1] = d[i]/(i+1);
    return r;
  }
#undef s
#undef d
};
ostream& operator<<(ostream&o,const fps&a) {
  rep(i,a.size()) o<<(i?" ":"")<<a[i].val();
  return o;
}


int main(){
    int n; cin >> n;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    fps f({1});
    int n2 = 0;
    while(1<<n2 < n) n2++;
    n2 = 1<<n2;
    vector<fps> g(n2);
    rep(i, n2) g[i] = (i < n ? fps{1, a[i]} : fps{1});
    // rep(i, n2) cout << g[i] << endl;
    for (int i = 0; 1<<i < n2; i++){
        // cout << i << "------" << endl;
        for (int j = 0; j < n2; j += 1<<(i+1)){
            g[j] *= g[j+(1<<i)];
            // cout << g[j] << endl;
        }
    }
    f *= g[0];
    mint s = 1;
    rep(i, f.size()) f[i] *= s, s *= i+1; 
    // cout << f << endl;
    int sum = SUM(a);
    vector<mint> fact(n+2, 1); 
    rep(i, n+1) fact[i+1] = fact[i]/(sum-i);
    mint ans;
    rep2(i, 1, n+2){
        ans += fact[i]*f[i];
        cout << f[i].val() << endl;
    }
    cout << ans.val() << endl;
    return 0;
}