// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using mint = modint;
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
using T4 = tuple<ll, int, int, int>;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, K; cin >> n >> K;
    vector<ll> a(n), b(n), c(n);
    rep(i, n) cin >> a[i];
    rep(i, n) cin >> b[i];
    rep(i, n) cin >> c[i];
    sort(rrng(a));
    sort(rrng(b));
    sort(rrng(c));
    auto f = [&](int i, int j, int k) -> ll{
        return a[i]*b[j]+b[j]*c[k]+c[k]*a[i];
    };
    priority_queue<T4> pq;
    pq.emplace(f(0, 0, 0), 0, 0, 0);
    map<T3, bool> used;
    used[{0, 0, 0}] = true;
    int cnt = 0;
    while(!pq.empty()){
        auto [v, i, j, k] = pq.top(); pq.pop();
        cnt++;
        if (cnt == K){
            cout << v << endl;
            return 0;
        }
        if (i+1 < n && !used[{i+1, j, k}]){
            used[{i+1, j, k}] = true;
            pq.emplace(f(i+1, j, k), i+1, j, k);
        }
        if (j+1 < n && !used[{i, j+1, k}]){
            used[{i, j+1, k}] = true;
            pq.emplace(f(i, j+1, k), i, j+1, k);
        }
        if (k+1 < n && !used[{i, j, k+1}]){
            used[{i, j, k+1}] = true;
            pq.emplace(f(i, j, k+1), i, j, k+1);
        }
    }
    return 0;
}