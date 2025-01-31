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
using T = tuple<ll, ll, int>;
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
    G g(n);
    rep(i, n){
        string s; cin >> s;
        rep(j, n){
            if (s[j] == 'Y') g[i].push_back(j);
        }
    }
    int q; cin >> q;
    vector<vector<pii>> qs(n);
    rep(i, q){
        int u, v; cin >> u >> v; u--; v--;
        qs[u].push_back({v, i});
    }
    vector<pll> ans(q);
    rep(i, n){
        vector<pll> dist(n, {LINF, -LINF});
        dist[i] = {0, a[i]};
        priority_queue<T, vector<T>, greater<T>> pq;
        pq.emplace(0, a[i], i);
        while(!pq.empty()){
            auto [nc, nv, now] = pq.top(); pq.pop();
            if(dist[now].fi < nc || (dist[now].fi == nc && dist[now].se > nv)) continue;
            for(auto to: g[now]){
                if (dist[to].fi == dist[now].fi+1 && dist[to].se < dist[now].se+a[to]){
                    dist[to].se = dist[now].se+a[to];
                    pq.emplace(dist[to].fi, dist[to].se, to);
                }
                else if (dist[to].fi > dist[now].fi+1){
                    dist[to] = {dist[now].fi+1, dist[now].se+a[to]};
                    pq.emplace(dist[to].fi, dist[to].se, to);
                }
            }
        }
        for (auto [j, id]: qs[i]){
            ans[id] = dist[j];
        }
    }
    rep(i, q){
        if (ans[i].fi == LINF) cout << "Impossible" << endl;
        else cout << ans[i].fi << " " << ans[i].se << endl;
    }
    return 0;
}