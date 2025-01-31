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

int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

int main(){
    int h, w; cin >> h >> w;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    vector<vector<P>> to(26);
    vector<bool> used(26);
    int sx, sy, gx, gy;
    rep(i, h)rep(j, w){
        if (g[i][j] == 'S') sx = i, sy = j;
        if (g[i][j] == 'G') gx = i, gy = j;
        if (g[i][j] == '#' || g[i][j] == 'S' || g[i][j] == 'G' || g[i][j] == '.') continue;
        to[g[i][j] - 'a'].emplace_back(i, j);
    }

    vector<vector<int>> dist(h, vector<int>(w, inf));
    dist[sx][sy] = 0;
    priority_queue<T, vector<T>, greater<T>> pq;
    pq.emplace(0, sx, sy);
    while(!pq.empty()){
        auto [d, x, y] = pq.top(); pq.pop();
        if (d > dist[x][y]) continue;
        rep(t, 4){
            int nx = x + dx[t];
            int ny = y + dy[t];
            if (nx < 0 || nx >= h || ny < 0 || ny >= w || g[nx][ny] == '#') continue;
            if (dist[nx][ny] > dist[x][y]+1){
                dist[nx][ny] = dist[x][y] + 1;
                pq.emplace(dist[nx][ny], nx, ny);
            }
        }
        if ('a' <= g[x][y] && g[x][y] <= 'z'){
            if (used[g[x][y] - 'a']) continue;
            used[g[x][y] - 'a'] = true;
            for(auto [nx, ny] : to[g[x][y] - 'a']){
                if (dist[nx][ny] > dist[x][y] + 1){
                    dist[nx][ny] = dist[x][y] + 1;
                    pq.emplace(dist[nx][ny], nx, ny);
                }
            }
        }
    }
    cout << (dist[gx][gy] == inf ? -1 : dist[gx][gy]) << endl;
    return 0;
}