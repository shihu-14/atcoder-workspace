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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
    vector<int> p(n);
    rep(i, n) cin >> p[i], p[i]--;
    mint fact_k = 1;
    rep2(i, 1, k+1) fact_k *= i;
    mint ans = 0, inv1 = 0, inv2 = 0, inv3 = 0;
    fenwick_tree<int> fw(n), fw2(n);
    rep(i, n){
        inv1 += fw.sum(p[i]+1, n);
        fw.add(p[i], 1);
    }
    ans += inv1*(n-k+1)*fact_k;
    mint tmp_inv = 0;
    rep(i, k){
        int inv = fw2.sum(p[i]+1, n);
        inv2 += inv;
        tmp_inv += inv;
        fw2.add(p[i], 1);
    }
    rep2(i, 1, n-k+1){
        tmp_inv -= fw2.sum(0, p[i-1]);
        fw2.add(p[i-1], -1);
        tmp_inv += fw2.sum(p[i+k-1]+1, n);
        inv2 += tmp_inv;
        fw2.add(p[i+k-1], 1);
    }
    ans -= inv2*fact_k;
    inv3 += (mint)k*(k-1)/2*fact_k/2*(n-k+1);
    ans += inv3;
    ans /= (n-k+1)*fact_k;
    cout << ans.val() << endl;
    return 0;
}