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

bool f(ll x){return x < 0;}
ll op(ll a, ll b){return max(a, b);}
ll e(){return -LINF;}
ll mapping(ll f, ll x){return f+x;}
ll composition(ll f, ll g){return f+g;}
ll id(){return 0;}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; ll b; cin >> n >> b >> q;
    vector<int> a(n), s(n);
    rep(i,n) cin >> a[i];
    lazy_segtree<ll, op, e, ll, mapping, composition, id> seg(n);
    rep(i, n) s[i] = a[i] - b;
    ll tmp = 0;
    rep(i, n){
        tmp += s[i];
        seg.set(i, tmp);
    }
    rep(i, q){
        int c, x; cin >> c >> x; c--;
        ll pre = a[c]; a[c] = x;
        seg.apply(c, n, x-pre);
        int it = seg.max_right<f>(0);
        if (it == n) it--;
        double res = ((double)seg.get(it)+b*(it+1))/(double)(it+1);
        printf("%.016f\n", res);
    }
    return 0;
}