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
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}
double op(double a, double b){return min(a, b);}
double e(){return LINF;}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    using pdd = pair<double, double>;
    int n, k; cin >> n >> k;
    vector<pdd> p;
    rep(i, n+1){
        int x, y; cin >> x >> y;
        p.emplace_back(x, y);
    }
    p.emplace_back(p[0]);
    vector<double> d(n+2);
    double sum = 0;
    rep2(i, 1, n+2){
        auto dist = [&](pdd p1, pdd p2) -> double{
            return sqrt((p1.fi-p2.fi)*(p1.fi-p2.fi) + (p1.se-p2.se)*(p1.se-p2.se));
        };
        d[i] = dist(p[i-1], p[0])+dist(p[i], p[0])-dist(p[i-1], p[i]);
        sum += dist(p[i-1], p[i]);
    }
    segtree<double, op, e> dp(n+2); dp.set(0, 0);
    rep2(i, 1, n+2){
        int l = max(0LL, i-k);
        dp.set(i, dp.prod(l, i)+d[i]);
    }
    double ans = dp.get(n+1);
    ans += sum;
    printf("%.016f\n", ans);
    return 0;
}