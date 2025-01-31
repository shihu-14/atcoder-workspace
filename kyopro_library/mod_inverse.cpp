ll gcd(ll a, ll b){
    return (b ? gcd(b, a%b) : a);
}

// nを素因数分解する。
vector<pair<ll, int>> prime_factor(ll n){
    vector<pair<ll, int>> res;
    for(ll i = 2; i*i <= n && n != 1; ++i){
        if (n%i == 0){
            ll cnt = 0;
            while(n%i == 0){
                n /= i;
                cnt++;
            }
            res.emplace_back(i, cnt);
        }
    }
    if (n != 1) res.emplace_back(n, 1);
    return res;
}

// a^x(mod n)の値を求める。
ll power(ll a, ll x, ll mod){
    ll res = 1;
    while(x > 0){
        if (x&1) res = (res*a)%mod;
        a = (a*a)%mod;
        x >>= 1;
    }
    return res;
}

// ax≡1(mod n)となるようなxを求める。
ll modinv(ll a, ll mod){
    assert(gcd(a, mod) == 1);
    auto pf = prime_factor(mod);
    ll phi = mod;
    // φ(n)を求める。
    for(auto [p, cnt]: pf){
        phi *= p-1;
        phi /= p;
    }
    phi--;
    // オイラーの定理a^φ(n) ≡ 1(mod n)から逆元を求める。-> a^-1 = a^(φ(n)-1)(mod n)
    return power(a, phi, mod);
}
