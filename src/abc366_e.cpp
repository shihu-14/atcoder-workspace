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

const int D = 2000000;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; ll d; cin >> n >> d;
    vector<ll> x(n), y(n);
    vector<int> in_x(2*D+1), in_y(2*D+1);
    rep(i, n){
        cin >> x[i] >> y[i];
        in_x[x[i]+D]++;
        in_y[y[i]+D]++;
    }
    sort(rng(x));
    sort(rng(y));

    vector<ll> sx, sy;
    ll sum_x = 0, sum_y = 0;
    int kx = 0, ky = 0;
    {
        rep(i, n) sum_x += x[i]+D;
        rep(i, n) sum_y += y[i]+D;
        sx.emplace_back(sum_x);
        sy.emplace_back(sum_y);
        kx += in_x[0];
        ky += in_y[0];
    }
    for (int i = -D+1; i <= D; i++){
        sum_x += kx;
        sum_x -= n-kx;
        sx.emplace_back(sum_x);
        kx += in_x[i+D];
    }
    for (int i = -D+1; i <= D; i++){
        sum_y += ky;
        sum_y -= n-ky;
        sy.emplace_back(sum_y);
        ky += in_y[i+D];
    }

    sort(rng(sy));
    
    ll ans = 0;
    rep(i, sx.size()){
        int it = upper_bound(rng(sy), d-sx[i]) - sy.begin();
        if (it < sy.size() && sx[i]+sy[it] == d) it++;
        ans += it;
    }
    cout << ans << endl;
    return 0;
}