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
using T = tuple<ll, int, int, int>;
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
#define SUM(v) accumulate(rng(v),0)
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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k; cin >> n >> k;
    vector<ll> a(n), b(n), c(n);
    rep(i, n) cin >> a[i];
    rep(i, n) cin >> b[i];
    rep(i, n) cin >> c[i];
    sort(rrng(a));
    sort(rrng(b));
    sort(rrng(c));
    priority_queue<T> pq; pq.emplace(a[0]*b[0]+b[0]*c[0]+c[0]*a[0], 0, 0, 0);
    map<tuple<int, int, int>, bool> used; used[{0, 0, 0}] = true;
    while(!pq.empty() && k){
        auto [v, x, y, z] = pq.top(); pq.pop();
        k--;
        if (k == 0){
            cout << v << endl;
            break;
        }
        if (x+1 < n && !used[{x+1, y, z}]) used[{x+1, y, z}] = true, pq.emplace(a[x+1]*b[y]+b[y]*c[z]+c[z]*a[x+1], x+1, y, z);
        if (y+1 < n && !used[{x, y+1, z}]) used[{x, y+1, z}] = true, pq.emplace(a[x]*b[y+1]+b[y+1]*c[z]+c[z]*a[x], x, y+1, z);
        if (z+1 < n && !used[{x, y, z+1}]) used[{x, y, z+1}] = true, pq.emplace(a[x]*b[y]+b[y]*c[z+1]+c[z+1]*a[x], x, y, z+1);
    }
    return 0;
}
