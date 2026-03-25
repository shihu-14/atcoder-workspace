/*
ランダム木に設置して、評価関数の値を最適化する。
-近傍操作
・木を1本追加
・木を1本削除
-評価関数
・スタートから到達できるマスの数(多いほど良い)
・スタートからゴールまでの距離(到達不可能は場合は-∞)
・木の本数(多いほど良い)
・曲角の個数(多いほど良い)
・ゴールの周りの堅固さ(多いほど良い)
*/
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
template<class T>
using G = vector<vector<T>>;
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
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
using T4 = tuple<int, int, int, int>;

int n, gx, gy;
vector<vector<char>> g;

// parameter
ll a0 = 80; // 近傍操作で木を追加する確率(%)

ll t0 = 5000; // 到達可能なマスの数
ll t1 = 5000; // スタートからゴールまでの距離1増加あたりの評価関数の増加量
ll t2 = 1000; // 木1本あたりの評価関数の増加量
ll t3 = 3200; // 角がどれくらいあるか
ll t4 = 3000; // ゴールの周りの半径1周の堅固さ
ll t5 = 2000; // ゴールの周りの半径5周の堅固さ
ll t5_dir = 10;

// debug
int debug_times = 0;
int debug_update = 0;
int debug_elapsed = 0;
ll debug_score = 0;
vector<vector<char>> debug_best_g;


// スタートからゴールまで到達可能か調べる関数
pll valid_score(int sx=1, int sy=n/2+1)
{
    queue<pii> q;
    q.emplace(sx, sy);
    vector<vector<int>> dist(n+2, vector<int>(n+2, INF));
    dist[sx][sy] = 0;
    auto f = [&]() -> ll
    {
        int res = 0;
        rep2(i, 1, n+1)rep2(j, 1, n+1)
        {
            if (dist[i][j] != INF) res++;
        }
        return res;
    };
    while(!q.empty())
    {
        auto [x, y] = q.front(); q.pop();
        rep(i, 4)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(g[nx][ny] == 'T') continue;
            if(dist[nx][ny] != INF) continue;
            dist[nx][ny] = dist[x][y] + 1;
            q.emplace(nx, ny);
        }
    }
    return {(dist[gx][gy] == INF ? -INF : dist[gx][gy]), f()};
}

// 盤面を評価する関数
ll calculate()
{
    ll res = 0;
    { // startからgoalまでの距離と到達可能なマスの数
        auto [s1, s0] = valid_score();
        res += s0 * t0;
        res += s1 * t1;
    }
    { // 木の本数
        ll tree_count = 0;
        rep2(i, 1, n+1)rep2(j, 1, n+1)
        {
            if (g[i][j] == 'T') tree_count++;
        }
        res += tree_count * t2;
    }
    { // 曲角の個数
        ll corner_count = 0;
        rep2(i, 1, n+1)rep2(j, 1, n+1)
        {
            if (g[i][j] != '.') continue;
            rep(d, 4)
            {
                int ni1 = i + dx[d];
                int nj1 = j + dy[d];
                int ni2 = i + dx[(d+1)%4];
                int nj2 = j + dy[(d+1)%4];
                if (g[ni1][nj1] == 'T' && g[ni2][nj2] == 'T') corner_count++;
            }
        }
        res += corner_count * t3;
    }
    { // ゴールの周りの半径1周の堅固さ
        int goal_cnt = 0;
        rep(d, 4)
        {
            int ni = gx + dx[d];
            int nj = gy + dy[d];
            if (g[ni][nj] == 'T') goal_cnt++;
        }
        res += goal_cnt * t4;
    }
    { // ゴールの周りの半径5周の堅固さ
        int goal_cnt = 0;
        for (int i = -t5_dir; i <= t5_dir; i++)
        {
            for (int j = -t5_dir; j <= t5_dir; j++)
            {
                int ni = gx + i;
                int nj = gy + j;
                if (ni < 0 || ni >= n+2 || nj < 0 || nj >= n+2) continue;
                if (g[ni][nj] == 'T') goal_cnt++;
            }
        }
        res += goal_cnt * t5;
    }
    return res;
}

// ランダムに近傍操作を繰り返して、評価関数の値を最適化する
vector<pii> alignment()
{
    auto start = chrono::steady_clock::now();
    vector<pii> res, cand_add, cand_del;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (g[i][j] == '.') cand_add.emplace_back(i, j);
    }
    random_device rd;
    mt19937 gen(rd());
    ll max_score = calculate();
    std::uniform_int_distribution<> op_dist(1, 100);
    while(1)
    {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start).count();
        debug_elapsed = elapsed;
        if (elapsed > 1900) break;
        // tmp for revert
        int tmp_x = -1, tmp_y = -1;
        // operation
        int op = op_dist(gen) < a0 ? 0 : 1; // 0: add, 1: del
        // add
        if (op == 0 && cand_add.size())
        {
            std::uniform_int_distribution<> dist(0, (int)cand_add.size()-1);
            int id = dist(gen);
            auto [x, y] = cand_add[id];
            g[x][y] = 'T';
            tmp_x = x; tmp_y = y;
            cand_add.erase(cand_add.begin()+id);
            cand_del.emplace_back(x, y);
            res.emplace_back(x, y);
        }
        // del
        else if (op == 1 && cand_del.size())
        {
            std::uniform_int_distribution<> dist(0, (int)cand_del.size()-1);
            int id = dist(gen);
            auto [x, y] = cand_del[id];
            g[x][y] = '.';
            tmp_x = x; tmp_y = y;
            cand_del.erase(cand_del.begin()+id);
            cand_add.emplace_back(x, y);
            res.erase(find(rng(res), pii(x, y)));
        }
        else{
            continue;
        }
        debug_times++;
        // evaluate
        auto score = calculate();
        if (score > max_score)
        {
            max_score = score;
            debug_update++;
            continue;
        }
        // revert
        if (op == 0)
        {
            g[tmp_x][tmp_y] = '.';
            cand_del.pop_back();
            cand_add.emplace_back(tmp_x, tmp_y);
            res.pop_back();
        }
        else if (op == 1)
        {
            g[tmp_x][tmp_y] = 'T';
            cand_add.pop_back();
            cand_del.emplace_back(tmp_x, tmp_y);
            res.emplace_back(tmp_x, tmp_y);
        }
    }
    debug_score = max_score;
    debug_best_g = g;
    return res;
}

void solve()
{
    cin >> n >> gx >> gy; gx++; gy++;
    g.resize(n+2, vector<char>(n+2, 'T'));
    rep(i, n)rep(j, n)
    {
        cin >> g[i+1][j+1];
    }
    g[1][n/2+1] = 'S'; // start
    g[gx][gy] = 'G'; // goal
    int x = -1, y = -1;
    // first
    {
        cin >> x >> y; x++; y++;
        { // for judge
            int m; cin >> m;
            rep(i, m)
            {
                int a, b; cin >> a >> b; a++; b++;
            }
        }
        auto res = alignment();
        cout << res.size() << " ";
        for (auto [a, b] : res) cout << a-1 << " " << b-1 << " ";
        cout << endl;
    }
    // loop
    while(1)
    {
        cin >> x >> y;
        x++; y++;
        { // for judge (if execute in local, need to comment out)
            int m; cin >> m;
            rep(i, m)
            {
                int a, b; cin >> a >> b; a++; b++;
            }
        }
        if (x == gx && y == gy)
        {
            return;
        }
        cout << 0 << endl;
    } 
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while(t--)
    {
        solve();
    }
    // for debug
    {
        // cerr << "debug_times: " << debug_times << endl;
        // cerr << "debug_update: " << debug_update << endl;
        // cerr << "debug_elapsed: " << debug_elapsed << endl;
        // cerr << "debug_score: " << debug_score << endl;
        // rep(i, n+2)
        // {
        //     rep(j, n+2) cerr << debug_best_g[i][j];
        //     cerr << endl;
        // }
        // printvv(debug_best_g);
    }
    return 0;
}