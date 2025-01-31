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
    int n, q; cin >> n >> q;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    vector<pii> p;
    map<pii, vector<int>> mp;
    rep(i, q){
        int l, r; cin >> l >> r; l--; r--;
        p.emplace_back(r, l);
        mp[{l, r}].emplace_back(i);
    }
    sort(rng(p));
    int D = (double)n/(double)sqrt((double)q);
    if (D == 0) D = 1;
    vector<vector<pii>> g(n/D+1);
    int t = 0;
    rep(i, q){
        auto [r, l] = p[i];
        if ((t+1)*D <= r){
            t++;
            g[t].emplace_back(l, r);
            continue;
        }
        g[t].emplace_back(l, r);
    }
    vector<ll> ans(q);
    rep(i, n/D+1){
        int m = g[i].size();
        if (m == 0) continue;
        sort(rng(g[i]));
        UNIQUE(g[i]);
        m = g[i].size();
        vector<ll> c(200010);
        rep2(j, g[i][0].fi, g[i][0].se+1){
            c[a[j]]++;
        }
        ll res = 0;
        rep(j, 200001){
            res += c[j]*(c[j]-1)/2*(c[j]-2)/3;
        }
        for (auto j: mp[{g[i][0].fi, g[i][0].se}]) ans[j] = res;
        int prel = g[i][0].fi, prer = g[i][0].se;
        rep2(j, 1, m){
            auto [l, r] = g[i][j];
            rep2(k, prel, l){
                res -= c[a[k]]*(c[a[k]]-1)/2*(c[a[k]]-2)/3;
                c[a[k]]--;
                res += c[a[k]]*(c[a[k]]-1)/2*(c[a[k]]-2)/3;
            }
            if (prer <= r){
                rep2(k, prer+1, r+1){
                    res -= c[a[k]]*(c[a[k]]-1)/2*(c[a[k]]-2)/3;
                    c[a[k]]++;
                    res += c[a[k]]*(c[a[k]]-1)/2*(c[a[k]]-2)/3;
                }
            }
            else{
                for(int k = prer; k > r; k--){
                    res -= c[a[k]]*(c[a[k]]-1)/2*(c[a[k]]-2)/3;
                    c[a[k]]--;
                    res += c[a[k]]*(c[a[k]]-1)/2*(c[a[k]]-2)/3;
                }
            }
            for (auto k: mp[{l, r}]) ans[k] = res;
            prel = l, prer = r;
        }
        
    }
    rep(i, q) cout << ans[i] << endl;
    return 0;
}