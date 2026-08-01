#include <iostream>
#include <vector>
#include <queue>

using namespace std;
using ll = long long;
using P = pair<ll, int>;
const ll INF = 3001002003004005006ll;
int main()
{
    int n, m; ll K; cin >> n >> m >> K;
    vector<ll> h(n);
    for (int i = 0; i < n; i++)
    {
        cin >> h[i];
    }
    vector<vector<int> > g(n);
    for (int i = 0; i < m; i++)
    {
        int u, v; cin >> u >> v; u--, v--;
        if (abs(h[u]-h[v]) <= K)
        {
            g[u].emplace_back(v);
            g[v].emplace_back(u);
        }
    }
    priority_queue<P, vector<P>, greater<P> > pq;
    vector<ll> dist(n, INF);
    pq.emplace(0, 0);
    dist[0] = 0;

    while(!pq.empty())
    {
        auto [d, v] = pq.top(); pq.pop();
        if (dist[v] < d) continue;
        for (int u: g[v])
        {
            ll w = abs(h[u]-h[v]);
            if (dist[v]+w < dist[u])
            {
                dist[u] = dist[v]+w;
                pq.emplace(dist[u], u);
            }
        }
    }
    cout << (dist[n-1] != INF ? dist[n-1]: -1) << endl;
}