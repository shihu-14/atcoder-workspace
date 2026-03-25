
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

// Gaussian Integer (ガウス整数) のライブラリ
struct GaussianInt
{
    ll x, y;
    static ll MOD;

    GaussianInt(ll x = 0, ll y = 0)
    {
        this->x = (x % MOD + MOD) % MOD;
        this->y = (y % MOD + MOD) % MOD;
    }

    // 基本演算
    GaussianInt operator+(const GaussianInt& o) const { return GaussianInt(x + o.x, y + o.y); }
    GaussianInt operator-(const GaussianInt& o) const { return GaussianInt(x - o.x, y - o.y); }
    GaussianInt operator*(const GaussianInt& o) const
    { 
        ll nx = ((__int128)x * o.x - (__int128)y * o.y) % MOD;
        ll ny = ((__int128)x * o.y + (__int128)y * o.x) % MOD;
        return GaussianInt(nx, ny);
    }
    
    // 代入演算子
    GaussianInt& operator+=(const GaussianInt& o) { *this = *this + o; return *this; }
    GaussianInt& operator-=(const GaussianInt& o) { *this = *this - o; return *this; }
    GaussianInt& operator*=(const GaussianInt& o) { *this = *this * o; return *this; }

    // 等価比較
    bool operator==(const GaussianInt& o) const { return x == o.x && y == o.y; }
    bool operator!=(const GaussianInt& o) const { return !(*this == o); }

    // 共役
    GaussianInt conj() const { return GaussianInt(x, -y); }

    // ノルム (x^2 + y^2)
    ll norm() const { return ((__int128)x * x + (__int128)y * y) % MOD; }
};
// 入出力
istream& operator>>(istream& is, GaussianInt& g)
{
    is >> g.x >> g.y;
    return is;
}
ostream& operator<<(ostream& os, const GaussianInt& g)
{
    os << "(" << g.x << ", " << g.y << ")";
    return os;
}
ll GaussianInt::MOD = 998244353;

// base^exp O(log(exp))
GaussianInt pow_c(GaussianInt base, ll exp)
{
    GaussianInt res(1, 0);
    while (exp > 0)
    {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

// x^2 + y^2 = p を満たす非負整数 {x, y} を返す. O(log p)
pair<ll, ll> find_gaussian_prime(ll p)
{
    if (p == 2) return {1, 1};
    if (p % 4 != 1) return {-1, -1};

    auto mod_pow = [](ll base, ll exp, ll mod)
    {
        ll result = 1 % mod;
        ll cur = base % mod;
        while (exp > 0)
        {
            if (exp & 1) result = (__int128)result * cur % mod;
            cur = (__int128)cur * cur % mod;
            exp >>= 1;
        }
        return result;
    };
    // z^2 = -1 (mod p) を探す
    ll z = 0;
    for (ll a = 2; ; ++a)
    {
        if (mod_pow(a, (p - 1) / 2, p) == p - 1)
        {
            z = mod_pow(a, (p - 1) / 4, p);
            break;
        }
    }
    // ユークリッドの互除法
    ll r0 = p;
    ll r1 = z;
    while ((__int128)r1 * r1 > p)
    {
        ll nr = r0 % r1;
        r0 = r1;
        r1 = nr;
    }
    
    ll a = r1;
    ll b = round(sqrt(p - a * a));
    return {a, b};
}


void solve()
{
    ll a, b, c, m;
    cin >> a >> b >> c >> m;
    vector<vector<mint>> dp(c, vector<mint>(c, 0));
    dp[1%c][0] += 1;        
    dp[0][1%c] += 1;        
    dp[c-1][0] += 1;  
    dp[0][c-1] += 1;

    GaussianInt::MOD = c;
    rep(i, m)
    {
        ll p, e; cin >> p >> e;
        vector<vector<mint>> res(c, vector<mint>(c, 0));
        if (p%4 == 3 && e%2 == 1)
        {
            cout << 0 << endl;
            return;
        }

        if (p == 2)
        {
            GaussianInt z(1, 1);
            z = pow_c(z, e);
            res[z.x][z.y] += 1;
        }
        else if (p%4 == 3)
        {
            e /= 2;
            GaussianInt z(p%c, 0);
            z = pow_c(z, e);
            res[z.x][z.y] += 1;
        }
        else
        {   
            using D = tuple<vector<vector<mint>>, GaussianInt, GaussianInt>;
            auto f = [&](auto f, ll n, GaussianInt phi, GaussianInt phibar) -> D
            {
                if (n == 1)
                {
                    vector<vector<mint>> res(c, vector<mint>(c, 0));
                    res[1%c][0] += 1;
                    return {res, phi, phibar};
                }

                ll n2 = n/2;
                auto [base, z, zbar] = f(f, n2, phi, phibar);

                vector<vector<mint>> res(c, vector<mint>(c, 0));
                rep(i, c)rep(j, c)
                {
                    GaussianInt u(i, j);
                    GaussianInt v = zbar*u;
                    res[v.x][v.y] += base[i][j];
                }
                rep(i, c)rep(j, c)
                {
                    GaussianInt u(i, j);
                    GaussianInt v = z*u;
                    res[v.x][v.y] += base[i][j];
                }
                z *= z;
                zbar *= zbar;
                if (n%2 == 1)
                {
                    vector<vector<mint>> nres(c, vector<mint>(c, 0));
                    rep(i, c)rep(j, c)
                    {
                        GaussianInt u(i, j);
                        GaussianInt v = phibar*u;
                        nres[v.x][v.y] += res[i][j];
                    }
                    nres[z.x][z.y] += 1;
                    res = nres;
                    z *= phi;
                    zbar *= phibar;
                    return {res, z, zbar};
                }
                return {res, z, zbar};

            };
            auto [x, y] = find_gaussian_prime(p);
            GaussianInt z(x, y);
            auto [base, _, _] = f(f, e+1, z, z.conj());
            res = base;
        }

        vector<vector<mint>> ndp(c, vector<mint>(c, 0));
        rep(i, c)rep(j, c)
        {
            if (dp[i][j] == 0) continue;
            rep(ni, c)rep(nj, c)
            {
                if (res[ni][nj] == 0) continue;
                GaussianInt u(i, j);
                GaussianInt v(ni, nj);
                GaussianInt w = u * v;
                ndp[w.x][w.y] += dp[i][j] * res[ni][nj];
            }
        }
        dp = ndp;
    }
    cout << dp[(-a%c+c)%c][(-b%c+c)%c].val() << endl;
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