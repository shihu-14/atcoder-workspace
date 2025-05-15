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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<vector<int>> a(n, vector<int>(n));
    rep(i, n) rep(j, n) cin >> a[i][j];
    int n2 = 2*n-2;
    int ln = n2/2, rn = n2-ln;
    vector<vector<int>> L(n*n), R(n*n);
    {
        auto dfs = [&](auto f, int x, int y, int s, int res)-> void{
            res ^= a[x][y];
            if (s == ln){
                L[x*n+y].emplace_back(res);
                return;
            }
            if (x+1 < n) f(f, x+1, y, s+1, res);
            if (y+1 < n) f(f, x, y+1, s+1, res);
            return;
        };
        dfs(dfs, 0, 0, 0, 0);
    }
    {
        rep(i, n)rep(j, n){
            if (i+j != ln) continue;
            auto dfs = [&](auto f, int x, int y, int s, int res)-> void{
                if (s == rn){
                    R[i*n+j].emplace_back(res);
                    return;
                }
                if (x+1 < n) f(f, x+1, y, s+1, res^a[x+1][y]);
                if (y+1 < n) f(f, x, y+1, s+1, res^a[x][y+1]);
                return;
            };
            dfs(dfs, i, j, 0, 0);
        }
    }
    ll ans = 0;
    rep(i, n*n){
        if (L[i].empty() || R[i].empty()) continue;
        map<int, int> mp;
        for (auto x: R[i]) mp[x]++;
        for (auto x: L[i]){
            ans += mp[x];
        }
    }
    cout << ans << endl;
    return 0;
}