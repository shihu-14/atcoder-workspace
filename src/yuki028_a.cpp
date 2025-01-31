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
const ll mod = 100000009;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q; cin >> q;
    rep(t, q){
        ll seed, n, k, b; cin >> seed >> n >> k >> b;
        vector<ll> x(n+1); x[0] = seed;
        rep(i, n) x[i+1] = 1+(x[i]*x[i]+x[i]*12345)%mod;

        vector<pii> ps;
        rep2(i, 2, 37){
            if (b%i) continue;
            int c = 0;
            while(b%i == 0) b /= i, c++;
            ps.emplace_back(i, c);
        }
        vector<vector<int>> a(ps.size(), vector<int>(n+1));
        rep(i, ps.size()){
            int p = ps[i].fi;
            rep(j, n+1){
                int c = 0;
                while(x[j]%p == 0) x[j] /= p, c++;
                a[i][j] = c;
            }
        }

        int res = INF;
        rep(i, ps.size()){
            sort(rng(a[i]));
            int s = 0;
            rep(j, k){
                s += a[i][j];
            }
            chmin(res, s/ps[i].se);
        }
        cout << res << endl;
    }
    return 0;
}