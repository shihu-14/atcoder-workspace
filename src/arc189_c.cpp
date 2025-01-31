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

struct LIS{
    int n, lis_n;
    LIS(vector<int> &a): n(a.size()){
        vector<int> dp(n, INF);
        // printv(a);
        rep(i, n){
            int it = lower_bound(rng(dp), a[i]) - dp.begin();
            dp[it] = a[i];
        }
        // printv(dp);
        lis_n = lower_bound(rng(dp), INF) - dp.begin();
    }
    int lis_size(){return lis_n; }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, x; cin >> n >> x; x--;
    vector<int> a(n), b(n);
    rep(i, n) cin >> a[i];
    rep(i, n) cin >> b[i];
    vector<int> p(n), q(n);
    rep(i, n) cin >> p[i], p[i]--; 
    rep(i, n) cin >> q[i], q[i]--;
    vector<int> inc_a, inc_b;
    vector<bool> used_a(n), used_b(n);
    {
        int now = x;
        used_a[now] = true;
        while(1){
            int to = p[now];
            if (used_a[to]) break;
            used_a[to] = true;
            if (inc_a.size() || a[to]) inc_a.push_back(to);
            now = to;
        }
    
        now = x;
        used_b[now] = true;
        while(1){
            int to = q[now];
            if (used_b[to]) break;
            used_b[to] = true;
            if (inc_b.size() || b[to]) inc_b.push_back(to);
            now = to;
        }
    }
    
    rep(i, n){
        if (used_a[i]) continue;
        int now = i;
        used_a[now] = true;
        if (a[now] == 1){
            cout << -1 << endl;
            return 0;
        }
        while(1){
            int to = p[now];
            if (used_a[to]) break;
            if (a[to] == 1){
                cout << -1 << endl;
                return 0;
            }
            used_a[to] = true;
            now = to;
        }
    }
    

    rep(i, n){
        if (used_b[i]) continue;
        int now = i;
        used_b[now] = true;
        if (b[now] == 1){
            cout << -1 << endl;
            return 0;
        }
        while(1){
            int to = q[now];
            if (used_b[to]) break;
            if (b[to] == 1){
                cout << -1 << endl;
                return 0;
            }
            used_b[to] = true;
            now = to;
        }
    }

    // printv(inc_a);
    // printv(inc_b);
    
    vector<int> id_a(n, -1);
    rep(i, inc_a.size()) id_a[inc_a[i]] = i;
    vector<int> tmp_b;
    rep(i, inc_b.size()){
        if (id_a[inc_b[i]] != -1){
            tmp_b.emplace_back(id_a[inc_b[i]]);
        }
    }
    LIS lis(tmp_b);
    cout << inc_a.size()+inc_b.size()-lis.lis_size() << endl;

    return 0;
}