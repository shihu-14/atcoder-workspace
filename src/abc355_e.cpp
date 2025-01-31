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
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, L, R; cin >> n >> L >> R; R++;
    int n2 = 1<<n;
    vector<vector<int>> g(n2+1);
    for (int i = 0; i <= n; i++){
        for (int j = 0; j < n2; j += (1<<i)){
            g[j].emplace_back(j+(1<<i));
            g[j+(1<<i)].emplace_back(j);
        }
    }

    vector<int> par(n2+1, -1);
    vector<int> dist(n2+1, INF); dist[L] = 0;
    priority_queue<pii, vector<pii>, greater<pii>> pq; pq.emplace(0, L);
    while(!pq.empty()){
        auto [c, u] = pq.top(); pq.pop();
        if (dist[u] < c) continue;
        for (auto v: g[u]){
            if (dist[v] > dist[u]+1){
                dist[v] = dist[u]+1;
                par[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }

    map<int, int> mp;
    for (int i = 0; i <= n; i++) mp[1<<i] = i;
    ll ans = 0;
    int now = R;
    while(now != L){
        int to = par[now];
        int op = 1;
        if (now < to) op = -1;
        cout << "? " << mp[abs(now-to)] << " " << (now < to ? now/abs(now-to) : to/abs(now-to)) << endl;
        int res; cin >> res;
        ans += res*op;
        now = to;
    }
    cout << '!' << " " << (ans%100+100)%100 << endl;
    return 0;
}