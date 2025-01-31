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
    int n, k; ll x; cin >> n >> k >> x;
    vector<ll> date;
    multiset<ll> mst;
    ll sum_t = 0;
    rep(i, n){
        ll t; cin >> t;
        sum_t += t;
        mst.insert(t);
        rep(j, n+1) date.emplace_back(t+j*x);
    }
    date.emplace_back(-LINF);
    UNIQUE(date);
    int m = date.size();
    fenwick_tree<int> fw(m);
    rep(i, m){
        int c = mst.count(date[i]);
        if (c) fw.add(i, c);
    }

    vector dp(m, vector(n+1, LINF)); dp[0][0] = 0;
    rep(i, m-1)rep(j, n+1){
        if (fw.sum(0, i+1) < j) break;
        {
            int nj = j + fw.sum(i+1, i+2);
            chmin(dp[i+1][nj], dp[i][j]);
        }
        {
            int it = upper_bound(rng(date), date[i]+x)-date.begin();
            int ad = fw.sum(i+1, it);
            int nj = j+ad;
            int out = min((int)j, k);
            nj -= out;
            chmin(dp[it-1][nj], dp[i][j]+out*date[i]);
        }
    }

    ll ans = dp[m-1][0]-sum_t;
    cout << ans << endl;
    return 0;
}