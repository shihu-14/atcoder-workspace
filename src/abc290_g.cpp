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
void solve(){
    ll d, k, x; cin >> d >> k >> x;
    ll sum = 0, L = 0, R = 0;
    {
        ll tmp = 1;
        rep(i, d+1){
            sum += tmp;
            if (i < d) L += tmp;
            tmp *= k;
        }
        R = sum;
    }
    auto f = [&](ll v){
        ll res = sum, tmp_sum = 1, tmp_k = k, u = L;
        while(v > 0){
            if (v%k) res -= tmp_sum*(k-v%k);
            if (v == u) res--;
            v = (v-1)/k;
            u = (u-1)/k;
            tmp_sum += tmp_k;
            tmp_k *= k;
        }
        return res;
    }; 
    ll ac = L, wa = R;
    while(wa-ac>1){
        ll wj = (wa+ac)/2;
        if (f(wj) <= x) ac = wj;
        else wa = wj;
    }
    ll res = f(ac);
    ll leaf = ac-L+1;
    ll in = x-leaf;
    // printf("%d, %d, leaf=%d, in=%d\n", ac, res, leaf, in);
    ll ans = leaf+in*(k+1);
    // printf("ans-p = %lld\n", ans);
    ans -= 2*(x-1);
    // printf("ans = %lld\n", ans);
    ll v0 = L, v1 = ac;
    while(v0 != v1){
        v0 = (v0-1)/k;
        v1 = (v1-1)/k;
    }
    while(v0 > 0 && res < x){
        v0 = (v0-1)/k;
        v1 = (v1-1)/k;
        res++;
    }
    if (v0 == 0) ans--; 
    cout << ans << '\n';
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    while(t--){
        solve();
    }
    cout.flush();
    return 0;
}