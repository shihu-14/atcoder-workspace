#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
const int inf = 1001001001;
int main(){
    int n, m; cin >> n >> m;
    scc_graph tmp_g(n);
    vector<vector<P>> g(2*n), grev(n);
    rep(i, m){
        int a, b, c; cin >> a >> b >> c; a--, b--;
        tmp_g.add_edge(a, b);
        g[a].emplace_back(b, c);
        grev[b].emplace_back(a, c);
    }
    auto scc = tmp_g.scc();
    vector<int> ans(n, -1);
    for (auto vec: scc){
        for (auto v: vec){
            vector<vector<P>> g2 = g;
            for(auto [u, c]: grev[v]){
                g2[u].emplace_back(n+v, c);
            }
            vector<int> dist(n*2, inf); dist[v] = 0;
            priority_queue<P, vector<P>, greater<P>> pq; pq.emplace(0, v);
            while(!pq.empty()){
                auto [nc, now] = pq.top(); pq.pop();
                if (nc > dist[now]) continue;
                for(auto [to, c]: g2[now]){
                    if (dist[to] > dist[now]+c){
                        dist[to] = dist[now]+c;
                        pq.emplace(dist[to], to);
                    }
                }
            }
            if (dist[n+v] == inf) ans[v] = -1;
            else ans[v] = dist[n+v];
        }
    }
    rep(i, n) cout << ans[i] << endl;
    return 0;
}