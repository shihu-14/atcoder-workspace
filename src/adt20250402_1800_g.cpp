// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
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
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}
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


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n; cin >> n;
    Eratosthenes er(n);
    vector<ll> mp(n+1);
    rep2(i, 1, n+1){
        auto pl = er.factorize1(i);
        ll tmp = 1;
        for (auto [p, a]: pl){
            if (a&1) tmp *= p;
        }
        mp[tmp]++;
    }
    ll ans = 0;
    rep(i, n+1) ans += mp[i]*mp[i];
    cout << ans << endl;
    return 0;
}