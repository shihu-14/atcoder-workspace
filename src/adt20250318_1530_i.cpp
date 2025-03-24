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
using T3 = tuple<ll, ll, ll>;
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

ll SUM2(ll a, ll d, ll n){
    return n*(2*a+(n-1)*d)/2;
}
void solve(){
    int n, m; cin >> n >> m;
    vector<ll> x(n), y(n);
    rep(i,n) cin >> x[i] >> y[i];
    vector<T3> b;
    b.emplace_back(0, 0, 0);
    ll sum = 0;
    rep(i, n){
        sum += x[i]*y[i];
        b.emplace_back(sum, x[i], y[i]);
        // cout << sum << " " << x[i] << " " << y[i] << endl;
    }
    ll ans = get<1>(b[1]);
    sum = 0;
    rep2(i, 1, n+1){
        auto [s1, d1, n1] = b[i-1];
        auto [s2, d2, n2] = b[i];
        if (s1 > 0 && s2 < 0){
            ll n3 = s1/(-d2);
            ll s3 = SUM2(s1+d2, d2, n3);
            // printf("!!s1=%lld, d2=%lld, n2=%lld, SUM2(s1, d2, n3)=%lld\n", s1, (-d2), n3, s3);
            chmax(ans, sum+s3);
            sum += SUM2(s1+d2, d2, n2);
            // printf("s1=%lld, d2=%lld, n2=%lld, SUM2(s1, d2, n2)=%lld\n", s1, d2, n2, SUM2(s1+d2, d2, n2));
            chmax(ans, sum);
            // cout << i << "-> " << sum << endl;
            continue;
        }
        sum += SUM2(s1+d2, d2, n2);
        // printf("s1=%lld, d2=%lld, n2=%lld, SUM2(s1, d2, n2)=%lld\n", s1, d2, n2, SUM2(s1+d2, d2, n2));
        // cout << i << "-> " << sum << endl;
        chmax(ans, sum);
    }
    cout << ans << "\n";    
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    while (t--){
        solve();
    }
    cout.flush();
    return 0;
}
