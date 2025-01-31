#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
const ll inf = 1e18;
int main(){
    int n, m, x, y; cin >> n >> m >> x >> y; x--, y--;
    vector<vector<T>> g(n);
    rep(i, m){
        int a, b, t, k; cin >> a >> b >> t >> k;
        a--; b--;
        g[a].emplace_back(b, t, k);
        g[b].emplace_back(a, t, k);
    }

    vector<ll> dist(n, inf);
    dist[x] = 0;
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.emplace(0, x);

    while(!pq.empty()){
        auto [nt, now] = pq.top(); pq.pop();
        if (dist[now] < nt) continue;
        for (auto [to, t, k] : g[now]){
            if (dist[now]%k == 0 && dist[to] > dist[now]+t){
                dist[to] = dist[now]+t;
                pq.emplace(dist[to], to);
            }
            else if (dist[now]%k != 0){
                ll res = (dist[now]/k+1)*k - dist[now];
                if (dist[to] > dist[now]+res+t){
                    dist[to] = dist[now]+res+t;
                    pq.emplace(dist[to], to);
                }
            }
        }
    }
    cout << (dist[y] == inf ? -1 : dist[y]) << endl;
    return 0;
}