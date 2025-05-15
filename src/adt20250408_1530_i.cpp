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
ll dp[3010][3010][2];
using T4 = tuple<ll, int, int, int>;
template<typename T=int>
struct CC {
    bool initialized;
    vector<T> xs;
    CC(): initialized(false) {}
    void add(T x) { xs.push_back(x);}
    void init() {
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(),xs.end()),xs.end());
        initialized = true;
    }
    int operator()(T x) {
        if (!initialized) init();
        return upper_bound(xs.begin(), xs.end(), x) - xs.begin() - 1;
    }
    T operator[](int i) {
        if (!initialized) init();
        return xs[i];
    }
    int size() {
        if (!initialized) init();
        return xs.size();
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, gx; cin >> n >> gx;
    vector<int> Y(n), Z(n);
    CC<T3> cc;
    cc.add({0, 0, 0});
    cc.add({gx, 0, 0});
    rep(i, n){
        cin >> Y[i];
        cc.add({Y[i], i, 1});
    }
    rep(i, n){
        cin >> Z[i];
        cc.add({Z[i], i, 0});
    }
    int m = cc.size();
    rep(i, m)rep(j, m)rep(k, 2) dp[i][j][k] = LINF;
    int init_id = cc({0, 0, 0});
    dp[init_id][init_id][0] = 0, dp[init_id][init_id][1] = 0;
    priority_queue<T4, vector<T4>, greater<T4>> pq;
    pq.emplace(0, init_id, init_id, 0);
    pq.emplace(0, init_id, init_id, 1);
    while(!pq.empty()){
        auto [d, l, r, f] = pq.top(); pq.pop();
        if (dp[l][r][f] < d) continue;
        auto xl = get<0>(cc[l]);
        auto xr = get<0>(cc[r]);
        int x = (f ? xr : xl);
        if (l-1 >= 0){
            auto [nx, id, iswall] = cc[l-1];    
            if (iswall){
                int p_key = Z[id];
                if (xl <= p_key && p_key <= xr){
                    if (dp[l-1][r][0] > dp[l][r][f]+abs(x-nx)){
                        dp[l-1][r][0] = dp[l][r][f]+abs(x-nx);
                        pq.emplace(dp[l-1][r][0], l-1, r, 0);
                    }
                }
            }
            else{
                if (dp[l-1][r][0] > dp[l][r][f]+abs(x-nx)){
                    dp[l-1][r][0] = dp[l][r][f]+abs(x-nx);
                    pq.emplace(dp[l-1][r][0], l-1, r, 0);
                }
            }
        }
        if (r+1 < m){
            auto [nx, id, iswall] = cc[r+1];
            if (iswall){
                int p_key = Z[id];
                if (xl <= p_key && p_key <= xr){
                    if (dp[l][r+1][1] > dp[l][r][f]+abs(x-nx)){
                        dp[l][r+1][1] = dp[l][r][f]+abs(x-nx);
                        pq.emplace(dp[l][r+1][1], l, r+1, 1);
                    }
                }
            }
            else{
                if (dp[l][r+1][1] > dp[l][r][f]+abs(x-nx)){
                    dp[l][r+1][1] = dp[l][r][f]+abs(x-nx);
                    pq.emplace(dp[l][r+1][1], l, r+1, 1);
                }
            }
        }
    }

    ll ans = LINF;
    int goal_id = cc({gx, 0, 0});
    rep(i, goal_id+1)rep(j, 2) chmin(ans, dp[i][cc({gx, 0, 0})][j]);
    rep2(i, goal_id+1, m)rep(j, 2) chmin(ans, dp[cc({gx, 0, 0})][i][j]);
    cout << (ans == LINF ? -1 : ans) << endl;
    return 0;
}