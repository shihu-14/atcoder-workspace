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
    int n, m, q; cin >> n >> m >> q;
    vector<ll> a(n), b(m);
    vector<pll> c;
    rep(i, n) cin >> a[i];
    rep(i, m) cin >> b[i];
    rep(i, n){
        c.emplace_back(a[i], 1);
    }
    rep(i, m){
        c.emplace_back(b[i], 0);
    }
    sort(rng(c));
    rep(i, q){
        ll x; cin >> x;
        int it1 = lower_bound(rng(c), make_pair(x, -1LL)) - c.begin();
        ll ans = 0;
        if (it1 == 0){
            auto [y, f] = c[it1];
            ans += y-x;
            if (f){
                int it2 = lower_bound(rng(b), y) - b.begin();
                ans += b[it2]-y;
            }
            else{
                int it2 = lower_bound(rng(a), y) - a.begin();
                ans += a[it2]-y;
            }
        }
        else if (it1 == n+m){
            auto [y, f] = c[it1-1];
            ans += x-y;
            if (f){
                int it2 = lower_bound(rng(b), y) - b.begin();
                ans += y-b[it2-1];
            }
            else{
                int it2 = lower_bound(rng(a), y) - a.begin();
                ans += y-a[it2-1];
            }
        }
        else{
            ans = LINF;
            auto [yr, fr] = c[it1];
            auto [yl, fl] = c[it1-1];
            ll res = 0;
            if (fr){
                int it2 = lower_bound(rng(b), yr) - b.begin();
                if (it2 == m){
                    it2--;
                    res += yr-x;
                    res += yr-b[it2];
                    chmin(ans, res);
                }
                else if (it2 == 0){
                    res += yr-x;
                    res += b[it2]-yr;
                    chmin(ans, res);
                }
                else{
                    res += yr-x;
                    res += yr-b[it2-1];
                    chmin(ans, res);
                    res -= yr-b[it2-1];
                    res += b[it2]-yr;
                    chmin(ans, res);
                }
            }
            else{
                int it2 = lower_bound(rng(a), yr) - a.begin();
                if (it2 == n){
                    it2--;
                    res += yr-x;
                    res += yr-a[it2];
                    chmin(ans, res);
                }
                else if (it2 == 0){
                    res += yr-x;
                    res += a[it2]-yr;
                    chmin(ans, res);
                }
                else{
                    res += yr-x;
                    res += yr-a[it2-1];
                    chmin(ans, res);
                    res -= yr-a[it2-1];
                    res += a[it2]-yr;
                    chmin(ans, res);
                }
            }
            res = 0;
            if (fl){
                int it2 = lower_bound(rng(b), yl) - b.begin();
                if (it2 == m){
                    it2--;
                    res += x-yl;
                    res += yl-b[it2];
                    chmin(ans, res);
                }
                else if (it2 == 0){
                    res += x-yl;
                    res += b[it2]-yl;
                    chmin(ans, res);
                }
                else{
                    res += x-yl;
                    res += yl-b[it2-1];
                    chmin(ans, res);
                    res -= yl-b[it2-1];
                    res += b[it2]-yl;
                    chmin(ans, res);
                }
            }
            else{
                int it2 = lower_bound(rng(a), yl) - a.begin();
                if (it2 == n){
                    it2--;
                    res += x-yl;
                    res += yl-a[it2];
                    chmin(ans, res);
                }
                else if (it2 == 0){
                    res += x-yl;
                    res += a[it2]-yl;
                    chmin(ans, res);
                }   
                else{
                    res += x-yl;
                    res += yl-a[it2-1];
                    chmin(ans, res);
                    res -= yl-a[it2-1];
                    res += a[it2]-yl;
                    chmin(ans, res);
                }
            }
        }

        cout << ans << '\n';
    }
    cout.flush();
    return 0;
}