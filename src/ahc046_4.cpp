// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <climits>
#include <cfloat>
#include <cassert>
#include <ctime>
#include <cctype>
#include <cwctype>
#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <utility>
#include <bitset>
#include <vector>
#include <deque>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <tuple>
#include <array>
#include <new>
#include <memory>
#include <limits>
#include <random>
#include <exception>
#include <stdexcept>
#include <regex>
#include <complex>
#include <chrono>
#include <future>
#include <thread>
#include <mutex>
#include <atomic>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using mint = modint;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
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
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const char dc[] = {'U', 'R', 'D', 'L'};
const char op[] = {'M', 'S', 'A'};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
int n, m;
vector<pii> goal;
vector<vector<vector<pii>>> make_slide_list(vector<vector<int>> &block_list){
    vector<vector<vector<pii>>> res(n+2, vector<vector<pii>>(n+2, vector<pii>(4)));
    vector<vector<int>> rows(n+2, {0, n+1}), cols(n+2, {0, n+1});
    rep2(x, 1, n+1)rep2(y, 1, n+1){
        if (block_list[x][y] == 0) continue;
        rows[x].emplace_back(y);
        cols[y].emplace_back(x);
    }
    rep(i, n+2) sort(rng(rows[i]));
    rep(i, n+2) sort(rng(cols[i]));
    rep2(i, 1, n+1)rep2(j, 1, n+1){
        if (block_list[i][j]) continue;
        {
            int it = upper_bound(rng(rows[i]), j) - rows[i].begin();
            res[i][j][1] = {i, rows[i][it]-1};
            res[i][j][3] = {i, rows[i][it-1]+1};
        }
        {
            int it = upper_bound(rng(cols[j]), i) - cols[j].begin();
            res[i][j][0] = {cols[j][it-1]+1, j};
            res[i][j][2] = {cols[j][it]-1, j};
        }
    }
    return res;
}

vector<pii> calc_score(int sx, int sy, vector<vector<int>> &block_list, vector<vector<vector<pii>>> &slide_list){
    vector<pii> res;
    int tmp_sx = sx, tmp_sy = sy;
    for (auto [gx, gy]: goal){
        vector<vector<pii>> par(n+2, vector<pii>(n+2, {-1, -1}));
        priority_queue<T3, vector<T3>, greater<T3>> pq;
        pq.emplace(0, tmp_sx, tmp_sy);
        vector<vector<int>> dist(n+2, vector<int>(n+2, INF));
        dist[tmp_sx][tmp_sy] = 0;
        while(!pq.empty()){
            auto [d, x, y] = pq.top(); pq.pop();
            if (dist[x][y] < d) continue;
            if (x == gx && y == gy) break;
            rep(t, 4){
                auto [nx, ny] = slide_list[x][y][t];
                if (block_list[nx][ny]) continue;
                if (dist[nx][ny] > dist[x][y]+1){
                    dist[nx][ny] = dist[x][y]+1;
                    par[nx][ny] = {x, y};
                    pq.emplace(dist[nx][ny], nx, ny);
                }
            }
            rep(t, 4){
                int nx = x+dx[t], ny = y+dy[t];
                if (block_list[nx][ny]) continue;
                if (dist[nx][ny] > dist[x][y]+1){
                    dist[nx][ny] = dist[x][y]+1;
                    par[nx][ny] = {x, y};
                    pq.emplace(dist[nx][ny], nx, ny);
                }
            }
        }
        if (dist[gx][gy] == INF){
            return {};
        }
        vector<pii> ways;
        {
            int x = gx, y = gy;
            while(x != tmp_sx || y != tmp_sy){
                ways.emplace_back(x, y);
                auto [px, py] = par[x][y];
                x = px, y = py;
            }
        }
        ways.emplace_back(tmp_sx, tmp_sy);
        if (ways.size() < 2){
            tmp_sx = gx, tmp_sy = gy;
            continue;
        }
        reverse(rng(ways));
        rep(i, ways.size()-1){
            auto [x1, y1] = ways[i];
            auto [x2, y2] = ways[i+1];
            int d = (abs(x1-x2) + abs(y1-y2) > 1);
            int vx = x2-x1, vy = y2-y1;
            assert(vx == 0 || vy == 0);
            if (vx == 0){
                if (vy > 0) res.emplace_back(d, 1);
                else res.emplace_back(d, 3);
            }
            else{
                if (vx > 0) res.emplace_back(d, 2);
                else res.emplace_back(d, 0);
            }
        }
        tmp_sx = gx, tmp_sy = gy;
    }
    return res;
}

pii block_setting(int sx, int sy, vector<pii> &block_list, vector<vector<int>> &blocks){
    vector<vector<bool>> used(n+2, vector<bool>(n+2, false));
    vector<pii> res;
    int tmp_sx = sx, tmp_sy = sy;
    for (auto [gx, gy]: block_list){
        vector<vector<pii>> par(n+2, vector<pii>(n+2, {-1, -1}));
        priority_queue<T3, vector<T3>, greater<T3>> pq;
        pq.emplace(0, tmp_sx, tmp_sy);
        vector<vector<int>> dist(n+2, vector<int>(n+2, INF));
        dist[tmp_sx][tmp_sy] = 0;
        while(!pq.empty()){
            auto [d, x, y] = pq.top(); pq.pop();
            if (dist[x][y] < d) continue;
            if (x == gx && y == gy) break;
            rep(t, 4){
                int nx = x+dx[t], ny = y+dy[t];
                if (blocks[nx][ny]) continue;
                if (dist[nx][ny] > dist[x][y]+1){
                    dist[nx][ny] = dist[x][y]+1;
                    par[nx][ny] = {x, y};
                    pq.emplace(dist[nx][ny], nx, ny);
                }
            }
        }
        if (dist[gx][gy] == INF){
            return {};
        }
        vector<pii> ways;
        {
            int x = gx, y = gy;
            while(x != tmp_sx || y != tmp_sy){
                ways.emplace_back(x, y);
                auto [px, py] = par[x][y];
                x = px, y = py;
            }
        }
        ways.emplace_back(tmp_sx, tmp_sy);
        if (ways.size() < 2){
            tmp_sx = gx, tmp_sy = gy;
            continue;
        }
        reverse(rng(ways));
        rep(i, ways.size()-1){
            auto [x1, y1] = ways[i];
            auto [x2, y2] = ways[i+1];
            int d = (abs(x1-x2) + abs(y1-y2) > 1);
            int vx = x2-x1, vy = y2-y1;
            assert(vx == 0 || vy == 0);
            if (i == ways.size()-2){
                if (vx == 0){
                    if (vy > 0) res.emplace_back(2, 1);
                    else res.emplace_back(2, 3);
                }
                else{
                    if (vx > 0) res.emplace_back(2, 2);
                    else res.emplace_back(2, 0);
                }
                used[gx][gy] = true;
                tmp_sx = x1, tmp_sy = x2;
                break;
            }
            if (vx == 0){
                if (vy > 0) res.emplace_back(d, 1);
                else res.emplace_back(d, 3);
            }
            else{
                if (vx > 0) res.emplace_back(d, 2);
                else res.emplace_back(d, 0);
            }
        }
    }
    return {tmp_sx, tmp_sy};
}

int main(){
    auto start = std::chrono::high_resolution_clock::now();
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int sx, sy;
    cin >> n >> m;
    cin >> sx >> sy; sx++, sy++;
    vector<pii> non_goal;
    {
        map<pii, bool> used; used[{sx, sy}] = true;
        rep(i, m-1){
            int x, y; cin >> x >> y; x++, y++;
            goal.emplace_back(x, y);
            used[{x, y}] = true;
        }
        rep2(i, 1, n+1)rep2(j, 1, n+1){
            if (used[{i, j}]) continue;
            non_goal.emplace_back(i, j);
        }
    }
    vector<vector<int>> blocks(n+2, vector<int>(n+2, 0));
    {
        rep(i, n+2) blocks[i][0] = blocks[i][n+1] = 1;
        rep(i, n+2) blocks[0][i] = blocks[n+1][i] = 1;
    }
    random_device rd; mt19937 gen(rd()); 
    uniform_int_distribution<int> dist(0, non_goal.size()-1);
    vector<pii> block_list;
    int times = 0, update = 0;
    int final_score = INF;
    vector<pii> final_ans;
    int alpha = 13, beta = 9, gamma = 7;
    while(1){
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (elapsed.count() > 1850) break;
        times++;
        int kind, tmp_x, tmp_y, tmp_nx, tmp_ny, tmp_id, tmp_id2;
        if (times < 10 || times % alpha == 0){ // add
            while(1){
                auto [x, y] = non_goal[dist(gen)];
                if (blocks[x][y]) continue;
                kind = 0;
                tmp_x = x, tmp_y = y;
                blocks[x][y] = 1;
                block_list.emplace_back(x, y);
                break;
            }
        }
        else if (times % beta == 1){ // delete
            if (block_list.size() == 0) continue;
            uniform_int_distribution<int> dist2(0, block_list.size()-1);
            while(1){
                int id = dist2(gen);
                auto [x, y] = block_list[id];
                if (blocks[x][y] == 0) continue;
                kind = 1;
                tmp_id = id;
                tmp_x = x, tmp_y = y;
                blocks[x][y] = 0;
                block_list.erase(block_list.begin() + id);
                break;
            }
        }
        else if (times % gamma == 0){ // move
            if (block_list.size() == 0) continue;
            uniform_int_distribution<int> dist2(0, block_list.size()-1);
            while(1){
                int id = dist2(gen);
                auto [x, y] = block_list[id];
                auto [nx, ny] = non_goal[dist(gen)];
                if (blocks[nx][ny]) continue;
                kind = 2;
                tmp_x = x, tmp_y = y;
                tmp_nx = nx, tmp_ny = ny;
                tmp_id = id;
                blocks[nx][ny] = 1;
                blocks[x][y] = 0;
                block_list[id] = {nx, ny};
                break;
            }
        }
        else {
            if (block_list.size() < 2) continue;
            uniform_int_distribution<int> dist2(0, block_list.size()-1);
            while(1){
                int id = dist2(gen), id2 = dist2(gen);
                if (id == id2) continue;
                kind = 3;
                tmp_id = id, tmp_id2 = id2;
                swap(block_list[id], block_list[id2]);
                break;
            }
        }
        vector<pii> tmp_ans;
        auto [tmp_sx, tmp_sy] = block_setting(sx, sy, block_list, blocks);
        auto slide_list = make_slide_list(blocks);
        auto res = calc_score(tmp_sx, tmp_sy, blocks, slide_list);
        tmp_ans.insert(tmp_ans.end(), res.begin(), res.end());
        int tmp_score = tmp_ans.size();
        if (res.size() != 0 && tmp_score < final_score){
            final_score = tmp_score;
            final_ans = tmp_ans;
            update++;
            continue;
        }
        { // recover
            if (kind == 0){
                blocks[tmp_x][tmp_y] = 0;
                block_list.pop_back();
            }
            else if (kind == 1){
                blocks[tmp_x][tmp_y] = 1;
                block_list.insert(block_list.begin() + tmp_id, {tmp_x, tmp_y});
            }
            else if (kind == 2){
                blocks[tmp_x][tmp_y] = 1;
                blocks[tmp_nx][tmp_ny] = 0;
                block_list[tmp_id] = {tmp_x, tmp_y};
            }
            else if (kind == 3){
                swap(block_list[tmp_id], block_list[tmp_id2]);
            }
        }
    }
    { // debug 
        cerr << "final score: " << final_score << endl;
        cerr << "times: " << times << endl;
        cerr << "update: " << update << endl;
    }
    { // output
        int cnt = 0;
        for (auto [o, dir]: final_ans){
            cnt++;
            cout << op[o] << " " << dc[dir] << endl;
            if (cnt == 2*n*m) break;
        }
    }
    return 0;
}