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
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
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

struct combination{
    vector<mint> fact, factinv;
    combination(int n): fact(n+1), factinv(n+1){
        fact[0] = 1;
        for(int i=1; i<=n; i++) fact[i] = fact[i-1]*i;
        factinv[n] = fact[n].inv();
        for(int i=n; i>0; i--) factinv[i-1] = factinv[i]*i;
    }
    mint operator()(int n, int k){
        if(n < 0 || k < 0 || k > n) return 0;
        return fact[n]*factinv[k]*factinv[n-k];
    }

    mint power(mint a, ll b){
        if (b < 0) return 0;
        mint res = 1;
        for(; b; b>>=1, a*=a) if(b&1) res *= a;
        return res;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    vector<int> c(m+1);
    rep(i, n) c[a[i]]++;
    combination C(n);
    vector<mint> p(m+1); p[0] = 1;
    // i個以下の数がk個未満である確率を求める。
    rep2(i, 1, m+1){
        int r = 0;
        rep2(j, 1, i+1) r += c[j]; // i個以下の数がr個存在する。
        if (r >= k) continue; // k個未満なのでrがk以上なら確率は0。
        mint res = 0; 
        rep(j, k){ // ちょうどj個(0<=j<k)存在する確率の和が求める確率。
            if (r > j || c[0] < j-r) continue;
            res += C(c[0], j-r)*C.power(i, j-r)*C.power(m-i, c[0]-j+r);
        }
        res /= C.power(m, c[0]);
        p[i] = res;
    }
    mint ans = 0;
    rep2(i, 1, m+1){
        ans += (p[i-1]-p[i])*i;
    }
    cout << ans.val() << endl;
    return 0;
}