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
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
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

// https://codeforces.com/blog/entry/62393 から引用
struct custom_hash {
    // splitmix64 (64ビット乱数生成)
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    // ハッシュ本体
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
} xor_rng64;

struct Random {
    unordered_map<uint64_t, uint64_t> memo;
    std::mt19937_64 engine;
    Random() : engine(std::chrono::steady_clock::now().time_since_epoch().count()) {}
    uint64_t operator()(uint64_t x) {
        if (memo.count(x)) return memo[x];
        std::uniform_int_distribution<uint64_t> dist(0, (1ULL<<63)-1);
        return memo[x] = dist(engine);
    }
} rng64;

/**
 * SetHash 構造体
 * - 集合に対してハッシュを構築
 * - 区間[l, r)のハッシュ値を O(1) で取得できる（排他的論理和(^)ベース）
 */
struct SetHash {
    int n;
    vector<uint64_t> hashSum;
    map<uint64_t, bool> mp;
    SetHash(const vector<uint64_t> &a): n(a.size()), hashSum(n+1) {
        for (int i = 0; i < n; i++){
            uint64_t h = xor_rng64(a[i]);
            if (mp.count(h)) h = 0;
            mp[h] = true;
            hashSum[i+1] = hashSum[i]^h;
        }
    }
    // 区間[l, r)のハッシュ値を返す
    uint64_t getHash(int l, int r) const {
        if(l < 0 || r > n || l > r) return 0;
        return hashSum[r]^hashSum[l];
    }
};

/**
 * MultiSetHash 構造体
 * - 多重集合に対してハッシュを構築
 * - 区間[l, r)のハッシュ値を O(1) で取得できる（加算(+)ベース）
 */
struct MultiSetHash {
    static const uint64_t P = (1LL<<61)-1; // prime
    int n; // array size
    vector<uint64_t> hashSum;
    MultiSetHash(const vector<uint64_t>& a): n(a.size()), hashSum(n+1) {
        for (int i = 0; i < n; i++) {
            uint64_t h = rng64(a[i]);
            hashSum[i+1] = (hashSum[i]+h)%P;
        }
    }
    // 区間[l, r)のハッシュ値を返す
    uint64_t getHash(int l, int r) const {
        if(l < 0 || r > n || l > r) return 0;
        return (hashSum[r]-hashSum[l]+P)%P;
    }
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<uint64_t> a(n), b(n);
    rep(i, n) cin >> a[i];
    rep(i, n) cin >> b[i];
    MultiSetHash ma(a), mb(b);
    rep(qi, q){
        int l1, r1, l2, r2; cin >> l1 >> r1 >> l2 >> r2; l1--; l2--;
        if (ma.getHash(l1, r1) == mb.getHash(l2, r2)) cout << "Yes" << endl;
        else cout << "No" << endl;
    }
    return 0;
}