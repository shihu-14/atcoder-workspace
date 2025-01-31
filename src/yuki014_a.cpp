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
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
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

ll gcd(ll a, ll b){ return (b ? gcd(b, a%b) : a); }
ll lcm(ll a, ll b){ return a/gcd(a, b)*b; }

// ax+by=gとなるg=gcd(a, b), x, yを求める拡張gcd
tuple<ll, ll, ll> extgcd(ll a, ll b) {
    if (b == 0) return {a, 1, 0};
    ll g, x, y;
    tie(g, x, y) = extgcd(b, a%b);
    return {g, y, x-a/b*y};
}

// n以下の素因数分解/約数列挙
struct Eratosthenes{
    vector<int> min_pf, prime; // min_pf[i]: iの最小の素因数, prime: 素数リスト
    // コンストラクタにおいて、ふるいを回す。
    Eratosthenes(int n) : min_pf(n+1, -1) {
        for(int i = 2; i <= n; i++){
            if(min_pf[i] != -1) continue;
            prime.emplace_back(i);
            for(int j = i; j <= n; j+=i){
                if (min_pf[j] == -1) min_pf[j] = i;
            }
        }
    }
    // nの素因数分解を返す関数。
    vector<pair<int, int>> factorize(int n){
        vector<pair<int,int>> res;
        while (n > 1) {
            int p = min_pf[n];
            int exp = 0;
            while (min_pf[n] == p) {
                n /= p;
                ++exp;
            }
            res.emplace_back(p, exp);
        }
        return res;
    }  

    // nの約数を列挙する関数。
    vector<int> divisor(int n) {
        vector<int> res({1});
        auto pf = factorize(n);
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
};



int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    Eratosthenes e(10010);
    vector<set<pii>> div(10010);
    vector<vector<int>> divs;
    rep(i, n){
        divs.emplace_back(e.divisor(a[i]));
        for(auto d: divs.back()){
            div[d].emplace(a[i], i);
        }
    }
    cout << a[0] << " ";
    int now = a[0], id = 0;
    rep(i, n-1){
        for(auto d: divs[id]){
            div[d].erase({now, id});
        }
        int min_lcm = INF, res = INF, rid = -1;
        for(auto d: divs[id]){
            if (div[d].empty()) continue;
            auto [min_d, nid] = *div[d].begin();
            if (min_lcm > lcm(min_d, now)){
                min_lcm = lcm(min_d, now);
                res = min_d;
                rid = nid;
            } 
            else if (min_lcm == lcm(min_d, now) && res > min_d){
                res = min_d;
                rid = nid;
            }
        }
        now = res;
        id = rid;
        cout << now << " ";
    }
    cout << endl;
    return 0;
}