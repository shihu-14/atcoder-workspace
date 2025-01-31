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
using T = tuple<int, ll, ll, ll, ll>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
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
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<T>> g(n);
    rep(i, m){
        ll l, d, k, c; int a, b; cin >> l >> d >> k >> c >> a >> b; a--, b--;
        g[b].emplace_back(a, l+c, d, k, c);
    }
    vector<ll> dist(n, -LINF); dist[n-1] = LINF;
    priority_queue<pll> pq; pq.emplace(LINF, n-1);
    while(!pq.empty()){
        auto [d, u] = pq.top(); pq.pop();
        if(dist[u] > d) continue;
        for(auto [v, l, d, k, c]: g[u]){
            if (l > dist[u]) continue;
            int ac = 0, wa = k;
            while(wa-ac > 1){
                int wj = (ac+wa)/2;
                if (l+wj*d <= dist[u]) ac = wj;
                else wa = wj;
            }
            if(dist[v] < l+ac*d-c){
                dist[v] = l+ac*d-c;
                pq.emplace(dist[v], v);
            }
        }
    }
    rep(i, n-1){
        if (dist[i] == -LINF) cout << "Unreachable" << endl;
        else cout << dist[i] << endl;
    }
    return 0;
}   