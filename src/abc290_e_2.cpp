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
    int n; cin >> n;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    rep(i, n) a[i]--;
    vector<vector<int>> g(n);
    rep(i, n){
        g[a[i]].emplace_back(i);
    }
    ll ans = 0;
    map<ll, ll> mp;
    rep(l, n){
        ll r = n-l-1;
        if (l <= r){
            int it1 = lower_bound(rng(g[a[l]]), l) - g[a[l]].begin();
            int it2 = upper_bound(rng(g[a[l]]), r) - g[a[l]].begin();
            ll k = it2-it1;
            ans += ((r-l+1)-k)*(l+1);
            if (r != n-1){
                mp[a[r+1]] += n-r-1;
                ans += (2*(n-r-1)-(n-2-r))*(n-r-1)/2 - mp[a[l]];
            }
        }
        else{
            if (n%2 == 0 && l == n/2){
                mp[a[l]] += n-l;
            }
            ans += (2*(n-l)-(n-l-1))*(n-l)/2 - mp[a[l]];
            mp[a[l]] -= n-l;
        }
    }
    cout << ans << endl;
    return 0;
}