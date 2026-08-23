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
// ベクトル演算を行うライブラリ。誤差で落ちる場合は適宜dobue->longlong, exp->0に変更すること。
struct V {
  ll x, y;
  V(ll x=0, ll y=0): x(x), y(y) {}
  V operator-() const { return V(-x, -y); }
  V& operator+=(const V& v) { x += v.x; y += v.y; return *this;}
  V operator+(const V& v) const { return V(*this) += v;}
  V& operator-=(const V& v) { x -= v.x; y -= v.y; return *this;}
  V operator-(const V& v) const { return V(*this) -= v;}
  V& operator*=(ll s) { x *= s; y *= s; return *this;}
  V operator*(ll s) const { return V(*this) *= s;}
  V& operator/=(ll s) { x /= s; y /= s; return *this;}
  V operator/(ll s) const { return V(*this) /= s;}
  ll dot(const V& v) const { return x*v.x + y*v.y;} // 内積
  ll cross(const V& v) const { return x*v.y - v.x*y;} // 外積 
  ll norm1() const { return abs(x) + abs(y);} // L1ノルム
  ll norm2() const { return sqrt(norm2s());} // L2ノルム
  ll norm2s() const { return x*x + y*y;} // L2ノルムの二乗
  V normalize() const { return *this/norm2();} // 正規化(ノルムを1にスケールする)
  V rotate90c() const { return V(-y, x);} // 反時計回りに90度回転
  V rotate90() const { return V(y, -x);} // 時計回りに90度回転
};
istream& operator>>(istream& is, V& v) {
  is >> v.x >> v.y; return is;
}
ostream& operator<<(ostream& os, const V& v) {
  os<<"("<<v.x<<","<<v.y<<")"; return os;
}

void solve()
{
    int n, q; cin >> n >> q;
    vector<V> p(n);
    rep(i, n)
    {
        cin >> p[i];
    }
    vector<ll> s1(2*n+1);
    vector<V> s2(2*n+1);
    rep2(i, 1, n+1)
    {
        s1[i] = p[i-1].cross(p[i%n]);
        s2[i] = (p[i-1]+p[i%n])*s1[i];
        s1[i+n] = s1[i];
        s2[i+n] = s2[i];
    }
    rep(i, 2*n)
    {
        s1[i+1] += s1[i];
        s2[i+1] += s2[i];
    }
    rep(qi, q)
    {
        int u, v; cin >> u >> v;
        if (u > v) v += n;
        ll sum1 = s1[v-1]-s1[u-1];
        V sum2 = s2[v-1]-s2[u-1];
        ll area = p[(v-1)%n].cross(p[u-1]);
        sum1 += area;
        sum2 += (p[(v-1)%n]+p[u-1])*area;
        double ans_x = (double)sum2.x/(sum1*3);
        double ans_y = (double)sum2.y/(sum1*3);
        printf("%.016f %.016f\n", ans_x, ans_y);
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