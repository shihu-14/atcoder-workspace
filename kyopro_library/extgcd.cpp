ll gcd(ll a, ll b){ return (b ? gcd(b, a%b) : a); }
ll lcm(ll a, ll b){ return a/gcd(a, b)*b; }

// ax+by=gとなるg=gcd(a, b), x, yを求める拡張gcd
tuple<ll, ll, ll> extgcd(ll a, ll b) {
    if (b == 0) return {a, 1, 0};
    ll g, x, y;
    tie(g, x, y) = extgcd(b, a%b);
    return {g, y, x-a/b*y};
}

