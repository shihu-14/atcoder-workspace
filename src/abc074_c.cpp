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

// fraction(a/b)
struct frac {
  ll a, b;
  frac(ll _a=-1, ll _b=1): a(_a), b(_b) {
    if (b == 0) a = -1, b = 1; else if (b < 0) a = -a, b = -b;
    // ll g = gcd(abs(a),b);
    // a /= g; b /= g;
  }
//   frac inv() const { return frac(b,a);}
//   frac operator+(const frac& x) const { return frac(a*x.b + x.a*b, b*x.b);}
//   frac operator-(const frac& x) const { return frac(a*x.b - x.a*b, b*x.b);}
//   frac operator*(const frac& x) const { return frac(a*x.a, b*x.b);}
//   frac operator/(const frac& x) const { return frac(a*x.b, b*x.a);}
//   frac& operator+=(const frac& x) { return *this = *this + x;}
//   frac& operator-=(const frac& x) { return *this = *this - x;}
//   frac& operator*=(const frac& x) { return *this = *this * x;}
//   frac& operator/=(const frac& x) { return *this = *this / x;}
  bool operator<(const frac& x) const { return a*x.b < x.a*b;}
  bool operator<=(const frac& x) const { return a*x.b <= x.a*b;}
//   bool operator==(const frac& x) const { return a == x.a && b == x.b;}
//   bool operator!=(const frac& x) const { return a != x.a || b != x.b;}
//   ll floor() const { return a/b;}
//   ll ceil() const { return (a+b-1)/b;}
//   ll gcd(ll a, ll b){ return (b ? gcd(b, a%b) : a); }
};
// istream& operator>>(istream&i,frac&a){i>>a.a>>a.b;return i;}
ostream& operator<<(ostream&o,const frac&a){o<<a.a+a.b<<" "<<a.a;return o;}


void solve()
{
    int A, B, C, D, E, F; cin >> A >> B >> C >> D >> E >> F;
    frac max_r;
    rep(a, 31)
    {
        rep(b, 31)
        {
            int water = 100*A*a+100*B*b;
            if (water > F) break;
            rep(c, 3001)
            {
                if (water+C*c > F) break;
                if (E*water < 100*C*c) break;
                int d = (E*water-100*C*c)/(100*D);
                chmin(d, (int)(F-water-C*c)/D);
                int sugar = C*c+D*d;
                // cout << a << " " << b << " " << c << " " << d << " | " << sugar << ", " << water <<'\n'; 
                assert(water+sugar <= F);
                assert(100*sugar <= E*water);
                frac r(sugar, water);
                chmax(max_r, r);
            }
        }
    }
    cout << max_r << '\n';
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