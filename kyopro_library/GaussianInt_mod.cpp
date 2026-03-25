// Gaussian Integer のライブラリ (Modあり)
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

/* testcase
　abc444_g
*/