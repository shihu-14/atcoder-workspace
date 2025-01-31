
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
using pdd = pair<double, double>;
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

bool equal(double a, double b){return abs(a-b)<1e-8;}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; ll k; cin >> n >> m >> k;
    vector<pdd> p(n), q(m);
    rep(i, n) cin >> p[i].fi >> p[i].se;
    rep(i, m) cin >> q[i].fi >> q[i].se;
    vector<double> tmp(m);

    auto f = [&](double x) -> bool{
        ll res = 0;
        rep(i, n){
            auto [a, b] = p[i];
            auto s = a-(a+b)*x;
            int it = lower_bound(rng(tmp), s) - tmp.begin();
            res += it;
        }
        return res <= k-1;
    };

    double wa = 0, ac = 1;
    int t = 60;
    while(t--){
        double wj = (ac+wa)/2.0;
        rep(i, m){
            auto [c, d] = q[i];
            tmp[i] = -c+(c+d)*wj;
        }
        sort(rng(tmp));
        if (f(wj)) ac = wj;
        else wa = wj;
    }
    ac *= 100;
    printf("%.016f\n", ac);
    return 0;
}