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
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
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
    int T; cin >> T;
    int d = 6;
    rep(_, T){
        ll n; cin >> n;
        ll ans = 0;

        vector<int> deg(d);
        auto f = [&](ll x, vector<int> &di) -> int{
            ll res = 0, base = 1;
            rep(i, d){
                if (di[i]) res += base;
                if (base <= n/x) base *= x;
                else base = ll(1e18)+1;
            }
            if (res < n) return 1;
            else if (res == n) return 0;
            else return -1;
        };

        rep(i, 1<<d){
            deg.assign(6, 0);
            rep(j, d){
                if(i>>j&1) deg[j] = 1;
            }
            ll ac = 2, wa = n+1;
            while(wa-ac > 1){
                ll wj = (ac+wa)/2;
                if (f(wj, deg) >= 0) ac = wj;
                else wa = wj;
            }
            if (f(ac, deg) == 0){
                ans++;
            }
        }

        for(ll b = 2; b <= 1000; b++){
            ll m = n;
            bool flag = true;
            int cnt = 0;
            while(m){
                ll r = m%b;
                if (r != 0 && r != 1){
                    flag = false;
                    break;
                }
                cnt++;
                m /= b;
            }
            if (flag && cnt > d) ans++;
        }
        cout << ans << "\n";
    }
    cout.flush();

    
    return 0;
}