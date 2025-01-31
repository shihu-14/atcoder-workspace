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
int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};
int main(){
    int h, w; cin >> h >> w;
    int sx, sy, gx, gy; cin >> sx >> sy >> gx >> gy;
    sx--; sy--; gx--; gy--;
    vector<string> g(h);
    rep(i, h) cin >> g[i];
    vector<vector<int>> dist(h, vector<int>(w, inf));
    dist[sx][sy] = 0;
    deque<P> pq;
    pq.emplace_front(sx, sy);

    while(!pq.empty()){
        auto [x, y] = pq.front(); pq.pop_front();
        rep(i, 4){
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || nx >= h || ny < 0 || ny >= w || g[nx][ny] == '#') continue;
            if (dist[nx][ny] > dist[x][y]){
                dist[nx][ny] = dist[x][y];
                pq.emplace_front(nx, ny);
            }
        }
        for(int i = -2; i <= 2; i++){            
            for (int j = -2; j <= 2; j++){
                if (i == 0 && j == 0) continue;
                int nx = x + i;
                int ny = y + j;
                if (nx < 0 || nx >= h || ny < 0 || ny >= w || g[nx][ny] == '#') continue;
                if (dist[nx][ny] > dist[x][y] + 1){
                    dist[nx][ny] = dist[x][y] + 1;
                    pq.emplace_back(nx, ny);
                }
            }
        }
    }

    cout << (dist[gx][gy] == inf ? -1 : dist[gx][gy]) << endl;
    return 0;
}