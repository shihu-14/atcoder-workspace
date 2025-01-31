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
#define rep(i, n) for (ll i = 0; i < n; ++i)
#define rep2(i, a, b) for (ll i = a; i < b; ++i)
#define rrep2(i, a, b) for (ll i = a; i > b; --i)
#define rep3(i, a, b, c) for (ll i = a; i < b; i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;
const ull mod = 0x1fffffffffffffff, base = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch()).count() % mod;
struct RollingHash {
    vector<ull> hashed, power;
    
    static constexpr ull mask(ll a){ return (1ULL << a) - 1; }
    
    inline ull mul(ull a, ull b) const {
        //*
        __uint128_t ans = __uint128_t(a) * b;
        /*/
         // without __uint128_t
         ull a31 = a >> 31, b31 = b >> 31;
         a &= mask(31);
         b &= mask(31);
         ull x = a * b31 + b * a31;
         ull ans = (a31 * b31 << 1) + (x >> 30) + ((x & mask(30)) << 31) + a * b;
         //*/
        ans = (ans >> 61) + (ans & mod);
        if(ans >= mod) ans -= mod;
        return ans;
    }
    
    
    RollingHash(const string &s) {
        ll n = s.size();
        hashed.assign(n + 1, 0);
        power.assign(n + 1, 0);
        power[0] = 1;
        for(ll i = 0; i < n; i++) {
            power[i + 1] = mul(power[i], base);
            hashed[i + 1] = mul(hashed[i], base) + s[i];
            if(hashed[i + 1] >= mod) hashed[i + 1] -= mod;
        }
    }
    
    ull get(ll l, ll r) const {
        ull ret = hashed[r] + mod - mul(hashed[l], power[r - l]);
        if(ret >= mod) ret -= mod;
        return ret;
    }
    
    ull connect(ull h1, ull h2, ll h2len) const {
        ull ret = mul(h1, power[h2len]) + h2;
        if(ret >= mod) ret -= mod;
        return ret;
    }
    
    void connect(const string &s){
        ll n = hashed.size() - 1, m = s.size();
        hashed.resize(n + m + 1);
        power.resize(n + m + 1);
        for(ll i = n; i < n + m; i++) {
            power[i + 1] = mul(power[i], base);
            hashed[i + 1] = mul(hashed[i], base) + s[i - n];
            if(hashed[i + 1] >= mod) hashed[i + 1] -= mod;
        }
    }
    
    ll LCP(const RollingHash &b, ll l1, ll r1, ll l2, ll r2) {
        ll len = min(r1 - l1, r2 - l2);
        ll low = -1, high = len + 1;
        while(high - low > 1) {
            ll mid = (low + high) / 2;
            if(get(l1, l1 + mid) == b.get(l2, l2 + mid)) low = mid;
            else high = mid;
        }
        return low;
    }
};



/*

mod 2^61 - 1 の RollingHash です  結構強いです
名前の衝突がある場合は namespace で囲うと良いです
hashed[i] : s[0, i) のハッシュ結果  hashed[i + 1] = s[i] + s[i-1] * power[1] + s[i-2] * power[2] + ...
power[i] : base ** i
get(l, r) : s[l, r) のハッシュ結果  s[r-1] + s[r-2] * power[1] + ... + s[l] * power[r - l + 1]
connect(h1, h2, h2len) : 2つのハッシュ結果からそれらを繋げたときのハッシュ結果を得ます
connect(s) : コンストラクタで渡した文字列に s を繋げたことにします  過去必要になったやつ
LCP(...) : Longest Common Prefix の長さ

*/

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    string s; cin >> s;
    RollingHash rh(s);
    int ac = 0, wa = n/2+1;
    auto f = [&](int m) -> bool{
        rep(i, n){
            rep2(j, i+m, n){
                if (j+m > n) break;
                if (rh.get(i, i+m) == rh.get(j, j+m)) return true;
            }
        }
        return false;
    };
    while(wa-ac > 1){
        int wj = (ac+wa)/2;
        if (f(wj)) ac = wj;
        else wa = wj;
    }
    cout << ac << endl;
    return 0;
}