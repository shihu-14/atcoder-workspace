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
#define popcount_ll __builtin_popcountll
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

// n以下の素因数分解/約数列挙
struct Eratosthenes{
    int N;
    vector<int> min_pf, prime; // min_pf[i]: iの最小の素因数, prime: 素数リスト
    // コンストラクタにおいて、ふるいを回す。-> min_pfを埋め、N以下の素数をprimeに列挙。O(Nlog(logN))
    Eratosthenes(int n) : N(n), min_pf(n+1, -1) {
        for(int i = 2; i <= n; i++){
            if(min_pf[i] != -1) continue;
            prime.emplace_back(i);
            for(int j = i; j <= n; j+=i){
                if (min_pf[j] == -1) min_pf[j] = i;
            }
        }
    }
    // N以下の自然数xの素因数分解を返す関数。p^a -> {p, a}, O(logx))
    vector<pair<int, int>> factorize1(int x){
        assert(x <= N);
        vector<pair<int,int>> res;
        while (x > 1) {
            int p = min_pf[x];
            int exp = 0;
            while (min_pf[x] == p) x/=p, exp++;
            res.emplace_back(p, exp);
        }
        return res;
    }  
    // 任意の整数xの素因数分解を返す関数。p^a -> {p, a}, O(sqrt(x))
    vector<pair<ll, int>> factorize2(ll x){
        vector<pair<ll, int>> res;
        for (ll i = 2; i*i <= x; i++) {
            if (x%i) continue;
            int exp = 0;
            while (x%i == 0) x/=i, exp++;
            res.emplace_back(i, exp);
        }
        if (x != 1) res.emplace_back(x, 1);
        return res;
    }
    bool isPrime(int x){return min_pf[x] == x;}
    // N以下のxの約数を列挙する。O(xの約数の個数)
    vector<int> divisor1(int x) {
        assert(x <= N);
        vector<int> res({1});
        auto pf = factorize1(x); // O(logx)
        for (auto p : pf) {
            int s = (int)res.size();
            for (int i = 0; i < s; ++i) {
                int v = 1;
                for (int j = 0; j < p.second; ++j) {
                    v *= p.first;
                    res.push_back(res[i] * v);
                }
            }
        }
        sort(res.begin(), res.end());
        return res;
    }
    // 任意の整数xの約数を列挙する。O(sqrt(x))
    vector<ll> divisor2(ll x) {
        vector<ll> lower, upper;
        for (ll i = 1; i*i <= x; i++) {
            if (x%i) continue;
            lower.emplace_back(i);
            if (i*i != x) upper.emplace_back(x/i);
        }
        lower.insert(lower.end(), upper.rbegin(), upper.rend());
        return lower;
    }
};

// a^n
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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m; cin >> n >> m;
    vector<ll> a(n);
    rep(i, n) cin >> a[i];

    Eratosthenes es(0);
    auto div = es.divisor2(m);
    auto pf = es.factorize2(m);
    int k = pf.size();
    vector<ll> b;
    rep(i, n) if (m%a[i] == 0) b.emplace_back(a[i]);
    int bm = b.size();
    vector<ll> fps(k);
    rep(i, k) fps[i] = power(pf[i].fi, pf[i].se);
    vector<int> bits(bm);
    rep(i, bm){
        rep(j, k){
            if (b[i]%fps[j] == 0) continue;
            bits[i] |= 1<<j;
        }
    }

    mint ans = 0;
    vector<int> c(1<<k);
    rep(i, bm) c[bits[i]]++; 
    rep(i, 1<<k){
        int cnt = 0;
        rep(j, 1<<k) if ((i&j) == i) cnt += c[j];
        mint res = power((mint)2, cnt);
        ans += res*(popcount(i)&1 ? -1 : 1);
    }
    if (k == 0) ans--; 
    cout << ans.val() << endl;
    return 0;
}