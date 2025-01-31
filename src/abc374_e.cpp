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
    int n; ll x; cin >> n >> x;
    vector<ll> a(n), p(n), b(n), q(n);
    rep(i, n) cin >> a[i] >> p[i] >> b[i] >> q[i];
    auto f = [&](ll w) -> bool{
        ll res = 0;
        rep(i, n){
            ll min_cost = LINF;
            if (a[i]*q[i] > b[i]*p[i]){
                rep(j, a[i]){
                    ll tmp = 0;
                    int k = (w-j*b[i]+a[i]-1)/a[i];
                    tmp += j*q[i]+k*p[i];
                    chmin(min_cost, tmp);
                }
            }
            else{
                rep(j, b[i]){
                    ll tmp = 0;
                    int k = (w-j*a[i]+b[i]-1)/b[i];
                    tmp += j*p[i]+k*q[i];
                    chmin(min_cost, tmp);
                }
            }
            res += min_cost;
        }
        return res <= x;
    };
    ll ac = 0, wa = INF;
    while(wa-ac > 1){
        ll wj = (ac+wa)/2;
        if (f(wj)) ac = wj;
        else wa = wj;
    }
    cout << ac << endl;
    return 0;
}