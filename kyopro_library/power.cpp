// a^x
template <typename T>
T power(T a, ll n){
    T res = 1;
    for (;n; n>>=1, a=a*a) if(n&1) res*=a;
    return res;
}
// floor(a^(1/n))
ll power2(ll a, ll n){
    assert(n >= 1);
    auto f = [&](ll w) -> bool{
        ll res = 1;
        rep(i, n){
            if (res > a/w) return false;
            res *= w;
        }
        return res <= a;
    };
    ll ac = 0, wa = a+1;
    while(wa-ac>1){
        ll wj = (ac+wa)/2;
        if (f(wj)) ac = wj;
        else wa = wj;
    }
    return ac;
}