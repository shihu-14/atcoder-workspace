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
  yuki321
*/