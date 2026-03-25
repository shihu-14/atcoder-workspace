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
// Gaussian Integer (Modなし)
struct GaussianInt
{
    ll x, y;
    GaussianInt(ll x = 0, ll y = 0) : x(x), y(y) {}
    // 基本演算
    GaussianInt operator+(const GaussianInt& o) const { return GaussianInt(x + o.x, y + o.y); }
    GaussianInt operator-(const GaussianInt& o) const { return GaussianInt(x - o.x, y - o.y); }
    GaussianInt operator*(const GaussianInt& o) const { return GaussianInt(x * o.x - y * o.y, x * o.y + y * o.x); }
    GaussianInt operator/(const GaussianInt& o) const
    {
        auto round = [](ll a, ll b)
        {
            if (a >= 0) return (a + b / 2) / b;
            else return (a - b / 2) / b;
        };
        ll den = o.norm();
        GaussianInt num = *this * o.conj();
        return GaussianInt(round(num.x, den), round(num.y, den));
    }
    GaussianInt operator%(const GaussianInt& o) const { return *this - (o * (*this / o)); }
    // 代入演算子
    GaussianInt& operator+=(const GaussianInt& o) { *this = *this + o; return *this; }
    GaussianInt& operator-=(const GaussianInt& o) { *this = *this - o; return *this; }
    GaussianInt& operator*=(const GaussianInt& o) { *this = *this * o; return *this; }
    GaussianInt& operator/=(const GaussianInt& o) { *this = *this / o; return *this; }
    GaussianInt& operator%=(const GaussianInt& o) { *this = *this % o; return *this; }
    // 等価比較
    bool operator==(const GaussianInt& o) const { return x == o.x && y == o.y; }
    bool operator!=(const GaussianInt& o) const { return !(*this == o); }
    // 共役
    GaussianInt conj() const { return GaussianInt(x, -y); }
    // ノルム
    ll norm() const { return x * x + y * y; }
};
// 入出力
istream& operator>>(istream& is, GaussianInt& g) { is >> g.x >> g.y; return is; }
ostream& operator<<(ostream& os, const GaussianInt& g) { os << "(" << g.x << ", " << g.y << ")"; return os; }

// ガウス整数における最大公約数（ユークリッド互除法）
GaussianInt gcd(GaussianInt a, GaussianInt b)
{
    while (b.norm() != 0)
    {
        GaussianInt r = a % b;
        a = b; b = r;
    }
    return a;
}

// x^2 + y^2 = p を満たす {x, y} を返す
pair<ll, ll> find_gaussian_prime(ll p)
{
    if (p == 2) return {1, 1};
    if (p % 4 != 1) return {-1, -1};

    auto mod_pow = [](ll base, ll exp, ll mod)
    {
        ll res = 1; base %= mod;
        while (exp)
        {
            if (exp % 2 == 1) res = (__int128)res * base % mod;
            base = (__int128)base * base % mod; 
            exp /= 2;
        }
        return res;
    };
    ll z = 0;
    for (ll a = 2; ; ++a)
    {
        if (mod_pow(a, (p - 1) / 2, p) == p - 1)
        {
            z = mod_pow(a, (p - 1) / 4, p);
            break;
        }
    }
    GaussianInt g = gcd(GaussianInt(p, 0), GaussianInt(z, 1));
    return {abs(g.x), abs(g.y)};
}
/* testcase
*/


void solve()
{
    ll p, q, n; cin >> p >> q >> n;
    if (p == 0 && q == 0)
    {
        int ans = 0;
        rep(i, n)
        {
            ll a, b; cin >> a >> b;
            if (a == 0 && b == 0) ans++;
        }
        cout << ans << endl;
        return;
    }

    GaussianInt z1(p, q), z2(q, p);
    GaussianInt g = gcd(z1, z2); 
    int ans = 0;
    rep(i, n)
    {
        ll a, b; cin >> a >> b;
        GaussianInt z(a, b);
        if (z % g == GaussianInt(0, 0)) ans++;
    }
    cout << ans << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while(t--)
    {
        solve();
    }
    return 0;
}