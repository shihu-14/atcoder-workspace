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
#define SIZE 26

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<ll> a(n);
    rep(i, n) cin >> a[i];
    vector<ll> c(SIZE), s(SIZE);
    vector<vector<ll>> g(SIZE);
    rep(i, n){
        int j = 0; ll two = 2;
        while(a[i]%two==0){
            j++;
            two *= 2;
        }
        c[j]++;
        two /= 2;
        g[j].emplace_back(a[i]/two);
        s[j] += a[i]/two;
    }

    ll ans = 0;
    vector<ll> two(SIZE, 1);
    rep(i, SIZE-1) two[i+1] = two[i]*2;

    rep(i, SIZE){
        int m = g[i].size();
        if (m == 0) continue;
        vector<ll> ksum(SIZE);
        rep(j, SIZE){
            vector<ll> mpc(two[j]), mps(two[j]);
            rep(k, m){
                ll r = ((-g[i][k])%two[j]+two[j])%two[j];
                mpc[r]++;
                mps[r] += g[i][k];
                ksum[j] += mpc[g[i][k]%two[j]]*g[i][k] + mps[g[i][k]%two[j]];
            }
        }
        rep(j, SIZE-1) ans += (ksum[j]-ksum[j+1])/two[j];
    }
   
    rep(i, SIZE)rep2(j, i+1, SIZE){
        if (c[i] == 0 || c[j] == 0) continue;
        ans += s[i]*c[j]+two[j-i]*s[j]*c[i];
    }
    cout << ans << endl;
    return 0;

}