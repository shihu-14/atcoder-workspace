/*
-最適化手法
・山登り法

-初期配置
・盤面全体をt_s_dir*t_s_dirの正方形ブロックに分解し,
各ブロックに対してx,y座標の和が偶数or奇数のマスに木を置く.
ただし, S->Gの最短経路上には木を置かない.
・ゴール周りは, 一本道のジグザグの道を探していき, 見つかり次第終了し, その道の両脇に木を置く.

-近傍操作
・木をn本追加
    ・ある空きマスについて, 上下左右に空きますが多い順に優先して木を置く
・木をn本削除
    ・ある木について, 上下左右の空きマスのGからのdistの差が大きい順に優先して木を削る
    ・以下のようなブロックがあった時に, 左上の不要な木を削る.
        TT
        T.
・木をn本移動
    ・ある木を上下左右斜め方向にランダムに動かす.

-評価の要素
・曲がり角の多さ(多いほど良い)
・到達可能なマスの数(多いほど良い)
・S->Gの最短距離(長いほど良い)
・盤面全体がアクセスしやすい(多いほど良い)
・盤面全体の見通しの悪さ(悪いほど良い)

-その他
・vis_scoreの計算は差分のみを評価している.
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
using pdd = pair<double, double>;
using T3 = tuple<ll, ll, ll>;
template<class T>
using G = vector<vector<T>>;
#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)
#define rep2(i, a, b) for (ll i = (ll)a; i < (ll)(b); ++i)
#define rrep2(i, a, b) for (ll i = (ll)a-1; i >= (ll)(b); --i)
#define rep3(i, a, b, c) for (ll i = (ll)a; i < (ll)(b); i+=c)
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

#ifdef LOCAL_JUDGE
constexpr bool IS_LOCAL_JUDGE = true;
#else
constexpr bool IS_LOCAL_JUDGE = false;
#endif

// ---- グローバル変数----------
extern int n, gx, gy;
extern vector<vector<char>> g;

// ====== LOCAL JUDGE (fast) ======
namespace Sim {
    // 内部状態
    static int N, TI, TJ;            // 花(目標)の座標（パディング）
    static int pi, pj;               // 現在位置（パディング）
    static bool finished = false;

    static vector<vector<char>> GG;  // 実盤面 '.' / 'T'（外周含む）
    static vector<vector<char>> seen;// 確認済み（外周 true）
    static vector<pair<int,int>> Q;  // 目的地候補（0-index, 内部で+1）
    static size_t qpos = 0;

    static bool has_dest = false;    // 目的地の有無
    static int  di = -1, dj = -1;    // 目的地（パディング）

    // 距離場キャッシュ（dest→全点の逆向きBFS）
    static vector<vector<int>> dist;
    static bool dist_valid = false;  // true なら dist は現状パス可能性に対して有効
    static const int INF = 1e9;

    // 「新しい木を視認してパス可能性が変わった」フラグ
    static bool blockers_changed = false;

    // ★総移動回数
    static long long steps = 0;
    inline void        sim_reset_steps(){ steps = 0; }
    inline long long   sim_get_steps(){ return steps; }

    // 暫定地図での通行可否：未確認は通行可、確認済みかつ木のみ遮断
    inline bool passable(int x,int y){
        return !(seen[x][y] && GG[x][y]=='T');
    }

    inline void invalidate_dist(){ dist_valid = false; }

    // 目的地までの距離場（逆向きBFS）を必要時のみ構築
    inline void ensure_dist(){
        if (!has_dest){ dist_valid=false; return; }
        if (dist_valid) return;
        dist.assign(N+2, vector<int>(N+2, INF));
        queue<pair<int,int>> q;
        dist[di][dj] = 0;
        q.emplace(di,dj);
        static const int DX[4]={-1,0,1,0}, DY[4]={0,1,0,-1};
        while(!q.empty()){
            auto [x,y] = q.front(); q.pop();
            for(int k=0;k<4;k++){
                int nx=x+DX[k], ny=y+DY[k];
                if (!passable(nx,ny)) continue;
                if (dist[nx][ny] != INF) continue;
                dist[nx][ny] = dist[x][y]+1;
                q.emplace(nx,ny);
            }
        }
        dist_valid = true;
    }

    // 現在地からの可達成分（暫定地図）を 1 回だけ計算（目的地未定→Q走査時）
    inline void reachable_from_here(vector<vector<char>>& vis){
        vis.assign(N+2, vector<char>(N+2,0));
        queue<pair<int,int>> q;
        q.emplace(pi,pj);
        vis[pi][pj] = 1;
        static const int DX[4]={-1,0,1,0}, DY[4]={0,1,0,-1};
        while(!q.empty()){
            auto [x,y]=q.front(); q.pop();
            for(int k=0;k<4;k++){
                int nx=x+DX[k], ny=y+DY[k];
                if (!passable(nx,ny)) continue;
                if (vis[nx][ny]) continue;
                vis[nx][ny] = 1;
                q.emplace(nx,ny);
            }
        }
    }

    // 現在地から4方向、最初の木まで（その木を含む）を確認済みに
    // ここで「新しい木を見たか」を検出して blockers_changed を立てる
    inline void reveal_here(vector<pair<int,int>>& newly){
        bool introduced_blocker = false;
        auto push = [&](int ix,int iy){
            if (1<=ix && ix<=N && 1<=iy && iy<=N){
                if (!seen[ix][iy]){
                    seen[ix][iy]=true;
                    newly.emplace_back(ix-1,iy-1);
                    if (GG[ix][iy]=='T') introduced_blocker = true;
                }
            }else{
                // 外周は内部のみtrue
                if (!seen[ix][iy]) seen[ix][iy]=true;
            }
        };
        static const int DX[4]={-1,0,1,0}, DY[4]={0,1,0,-1};
        push(pi,pj);
        for(int k=0;k<4;k++){
            int x=pi, y=pj;
            while(true){
                x+=DX[k]; y+=DY[k];
                push(x,y);
                if (GG[x][y]=='T') break;
            }
        }
        if (introduced_blocker){
            blockers_changed = true;
            invalidate_dist();
        }
    }

    // 目的地更新（③〜⑤）: 距離場は必要時のみ評価
    inline void update_dest(){
        // ③ 花が見えたら花へ確定
        if (seen[TI][TJ]){
            if (!(has_dest && di==TI && dj==TJ)){
                has_dest=true; di=TI; dj=TJ;
                invalidate_dist();
            }
            return;
        }

        // ④ 現目的地が見えていて花でない → 未定化
        if (has_dest){
            if (seen[di][dj] && !(di==TI && dj==TJ)){
                has_dest=false; invalidate_dist();
            }
        }

        // ④' 新しい木でパスが潰れた可能性 → 到達不能なら未定化
        if (has_dest && blockers_changed){
            ensure_dist();
            if (dist[pi][pj] == INF){
                has_dest=false; invalidate_dist();
            }
        }

        // ⑤ 未定なら Q から：未確認 かつ 暫定地図で到達可能
        if (!has_dest){
            // 1回だけ現在地からの可達成分を計算
            vector<vector<char>> vis;
            reachable_from_here(vis);
            for(; qpos<Q.size(); ++qpos){
                int qi=Q[qpos].first+1, qj=Q[qpos].second+1;
                if (qi<1||qi>N||qj<1||qj>N) continue;
                if (seen[qi][qj]) continue;           // 未確認のみ
                if (!vis[qi][qj]) continue;           // 暫定地図で到達可
                has_dest=true; di=qi; dj=qj; ++qpos;
                invalidate_dist();
                break;
            }
        }
    }

    // 1ターン進める： (現在位置0-index, 新たに確認済みになったマス群0-index) を返す
    pair<pair<int,int>, vector<pair<int,int>>> sim_step(){
        vector<pair<int,int>> newly;
        bool moved = false;
        if (finished || (pi==TI && pj==TJ)){ finished=true; return {{pi-1,pj-1}, newly}; }

        // 視認 → 目的地更新
        blockers_changed = false;
        reveal_here(newly);
        update_dest();

        // 距離場を用いて1歩移動
        if (has_dest){
            ensure_dist();
            int cur = dist[pi][pj];
            if (cur == INF){
                // 目的地が実は到達不能 → 目的地を破棄して次を探す（同ターン内で1回だけ再試行）
                has_dest=false; invalidate_dist();
                update_dest();
                if (has_dest){
                    ensure_dist();
                    cur = dist[pi][pj];
                }
            }
            if (cur < INF){
                static const int DX[4]={-1,0,1,0}, DY[4]={0,1,0,-1};
                const int ord[4]={0,2,3,1}; // 上,下,左,右
                for(int t=0;t<4;t++){
                    int k=ord[t];
                    int ni=pi+DX[k], nj=pj+DY[k];
                    if (!passable(ni,nj)) continue;
                    if (dist[ni][nj] < cur){ pi=ni; pj=nj; moved=true; break; }
                }
            }
        }

        if (moved) steps++;
        if (pi==TI && pj==TJ) finished=true;
        return {{pi-1,pj-1}, newly};
    }

    // 盤面に木を反映（パディング座標で渡す）
    void sim_apply_trees(const vector<pair<int,int>>& v){
        for (auto [i,j] : v) GG[i][j] = 'T';
        // 実盤面が変わったので、次回視認で見えるまでは passable に影響しない。
        // （見えた瞬間に blockers_changed で距離場を無効化）
    }

    // 初期化
    void sim_init(const vector<pair<int,int>>& Q0){
        // ★ extern はここには書かない。グローバルを明示的に参照する。
        N  = ::n;
        TI = ::gx;
        TJ = ::gy;

        GG.assign(N+2, vector<char>(N+2,'T'));
        seen.assign(N+2, vector<char>(N+2,false));
        for(int i=0;i<=N+1;i++){ seen[i][0]=seen[i][N+1]=true; }
        for(int j=0;j<=N+1;j++){ seen[0][j]=seen[N+1][j]=true; }
        for(int i=1;i<=N;i++)for(int j=1;j<=N;j++)
            GG[i][j] = (::g[i][j]=='T' ? 'T' : '.'); // 'S','G' は通路扱い

        pi=1; pj=N/2+1;
        finished=false; has_dest=false; qpos=0; di=dj=-1;
        Q=Q0;

        dist.assign(N+2, vector<int>(N+2, INF));
        dist_valid = false;
        blockers_changed = false;
        steps = 0;
    }

    // 到達判定
    bool sim_finished(){ return finished; }
} // namespace Sim
// ====== END LOCAL JUDGE ======




// global variables
int n, sx, sy, gx, gy;
vector<vector<char>> g;
vector<vector<ll>> dist;
vector<set<int>> rows, cols; // 各行・各列の木の位置
double global_vis_score;
ll global_space_count;
double tmp_diff_vis_score, tmp_new_space_count, tmp_pre_space_count;

// parameter
double a_limit = 1900;
 // 時間帯ごとの操作の割合%(add, del1. del2, move)
vector<vector<int>> a_r = {{30, 20, 20, 30}, {30, 20, 20, 30}, 
    {20, 30, 20, 30}, {25, 25, 10, 40}};
vector<double> a_l = {0.15, 0.4, 0.70, 1.0}; // 時間帯の境界(%)
vector<ll> a_n = {18, 10, 8, 10}; // 各時間帯で一度に操作する回数

double t_curve = 23000; // 盤面の入り組み具合を評価する
double t_spread = 12; // 到達可能なますの数を評価
double t_sg = 2000; // スタートからゴールまでの距離を評価する
double t_accessible = 200; // 盤面全体が行き来しやすいことを評価する
double t_vis = 200; // 見通しの悪さを評価する

ll t_s_dir = 10; // 初期配置の間隔
ll t_g_edir = 6; // ゴール周りには木を置かない
ll t_g_dir = 8; // ゴール周りの堅固さを評価する際の範囲
ll t_g_straight = 3; // ゴール周りの堅固さを評価する際の直線距離
// double T0 = 5e-12, Tend = 5e-12; // SAの温度パラメータ

// debug
int debug_times = 0;
int debug_update = 0;
int debug_elapsed = 0;
int debug_first_times = 0;
int debug_middle_times = 0;
int debug_last_times = 0;
int debug_first_accept = 0;
int debug_middle_accept = 0;
int debug_last_accept = 0;
double debug_score = 0;

// グローバル変数を初期化する
void initialize_global_variables(){
    { // dist tableを作る
        dist.assign(n+2, vector<ll>(n+2, INF));
        queue<pii> q;
        q.emplace(gx, gy);
        dist[gx][gy] = 0;
        while(!q.empty())
        {
            auto [x,y] = q.front(); q.pop();
            rep(d, 4)
            {
                int nx = x + dx[d], ny = y + dy[d];
                if (g[nx][ny] == 'T') continue;
                if (dist[nx][ny] != INF) continue;
                dist[nx][ny] = dist[x][y] + 1;
                q.emplace(nx,ny);
            }
        }
    }
    // row, col を埋める
    rows.assign(n+2, set<int>());
    cols.assign(n+2, set<int>());
    global_vis_score = 0;
    tmp_diff_vis_score = 0;
    global_space_count = 0;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (g[i][j] == 'T')
            {
                rows[i].emplace(j);
                cols[j].emplace(i);
            }
        }
    }
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (dist[i][j] == INF) continue;
        global_space_count++;
        int r = 0, c = 0;
        if (rows[i].size())
        {
            auto it = rows[i].lower_bound(j);
            if (it != rows[i].begin())
            {
                r += j - *prev(it) - 1;
            }
            if (it != rows[i].end())
            {
                r += *it - 1 - j;
            }
        }
        if (cols[j].size())
        {
            auto it = cols[j].lower_bound(i);
            if (it != cols[j].begin())
            {
                c += i - *prev(it) - 1;
            }
            if (it != cols[j].end())
            {
                c += *it - 1 - i;
            }
        }
        global_vis_score += r + c + 1;
    }
    global_vis_score /= global_space_count;
}

// 初期配置
vector<pii> initial_alignment(){
    dist.assign(n+2, vector<ll>(n+2, INF));
    vector<vector<bool>> onpath(n+2, vector<bool>(n+2, false));
    { // スタートからゴールまでの最短経路を求める
        vector<vector<pii>> par(n+2, vector<pii>(n+2, {-1,-1}));
        queue<pii> q;
        q.emplace(sx, sy);
        dist[sx][sy] = 0;
        while(!q.empty())
        {
            auto [x,y] = q.front(); q.pop();
            if (x==gx && y==gy) break;
            rep(d, 4)
            {
                int nx = x + dx[d], ny = y + dy[d];
                if (g[nx][ny] == 'T') continue;
                if (dist[nx][ny] != INF) continue;
                dist[nx][ny] = dist[x][y] + 1;
                par[nx][ny] = {x,y};
                q.emplace(nx,ny);
            }
        }

        if (dist[gx][gy] != INF)
        {
            int x=gx, y=gy;
            while(!(x==sx && y==sy))
            {
                onpath[x][y]=true;
                auto p = par[x][y];
                if (p.first==-1) break;
                tie(x,y) = p;
            }
            onpath[sx][sy]=true;
        }
    }

    vector<pii> res;
    for (int i = 1; i <= n; i += t_s_dir)
    {
        for (int j = 1; j <= n; j += t_s_dir)
        {
            for (int di = 0; di < t_s_dir; di++)
            {
                int ni = i + di;
                if (ni > n) break;
                for (int dj = 0; dj < t_s_dir; dj++)
                {
                    int nj = j + dj;
                    if (nj > n) break;
                    if (g[ni][nj] != '.') continue;
                    if (abs(ni - gx) + abs(nj - gy) <= t_g_edir) continue;
                    if ((ni+nj)%2 != (i+j)%2) continue;
                    if (onpath[ni][nj]) continue;
                    g[ni][nj] = 'T';
                    res.emplace_back(ni, nj);
                }
            }
        }
    }
    return res;
}

// ゴール周りの配置
vector<pii> goal_alignment(){
    vector<pii> res, best_path; // 移動経路
    T3 best_score = {-INF, -INF, -INF}; // {曲がり角の数, -直線移動の数} (大きいほど良い)
    auto calc_dir = [](pii e)
    {       
        if (e.fi == 0) return e.se == 1 ? 1: 3;
        else return e.fi == 1 ? 2: 0;
    };
    auto is_ok = [](pii e1, pii e2)
    {
        return e1.fi*e2.fi + e1.se*e2.se == 0;
    };
    auto reachable = [&](vector<pii> &path) -> int
    {
        auto board = g;
        {
            set<pii> st;
            for (auto [x, y]: path) st.emplace(x, y);
            rep(i, path.size()-1)
            {
                auto [x, y] = path[i];
                if (board[x][y] != '.' && board[x][y] != 'G') assert(0);
                rep(d, 4)
                {
                    int nx = x + dx[d];
                    int ny = y + dy[d];
                    if (st.count({nx, ny})) continue;
                    if (board[nx][ny] == '.') board[nx][ny] = 'T';
                }
            }
        }

        queue<pii> q; 
        q.emplace(gx, gy);
        vector<vector<int>> dist(n+2, vector<int>(n+2, INF));
        dist[gx][gy] = 0;
        while (q.size())
        {
            auto [x, y] = q.front(); q.pop();
            if (x == sx && y == sy) return dist[x][y];
            rep(d, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (board[nx][ny] == 'T') continue;
                if (dist[nx][ny] != INF) continue;
                dist[nx][ny] = dist[x][y] + 1;
                q.emplace(nx, ny);
            }
        }
        return -INF;
    };

    // main
    bool found = false;
    for (int d2: {2, 1, 3, 0})
    {
        if (found) break;
        for (int d1: {0, 2})
        {
            // start simulation
            vector<pii> path, tmp_stk;
            T3 score = {0, 0, 0};
            auto dfs = [&](auto f, int x, int y, int p1x, int p1y, 
                int d1, int d2, bool ok=true, int curve=1, int straight=0) -> bool
            {
                if (t_g_straight < straight) return false;
                if (t_g_dir <= curve) 
                {
                    ll sg = reachable(tmp_stk);
                    if (sg == -INF) return false;
                    if (chmax(score, {curve, -straight, sg}))
                    {
                        path = tmp_stk;
                    }
                    return true;
                }

                vector<int> d_list;
                if (ok) d_list = {d2, d1};
                else d_list = {(d1+1)%4, (d1+3)%4, d1};

                for (auto d: d_list)
                {
                    int nx = x + dx[d];
                    int ny = y + dy[d];
                    if (g[nx][ny] == '.') 
                    {
                        {
                            bool used = false;
                            for (int tmp_d = 0; tmp_d < 4; tmp_d++)
                            {
                                int tx = nx + dx[tmp_d];
                                int ty = ny + dy[tmp_d];
                                if (tx == x && ty == y) continue;
                                auto it = find(rng(tmp_stk), pii{tx, ty});
                                if (it != tmp_stk.end())
                                {
                                    used = true;
                                    break;
                                }
                            }
                            if (used) continue;
                        }
                        tmp_stk.emplace_back(nx, ny);
                        int nd1 = calc_dir({nx-x, ny-y});
                        int nd2 = calc_dir({x-p1x, y-p1y});
                        bool nok = is_ok({dx[nd1], dy[nd1]}, {dx[nd2], dy[nd2]});
                        if (f(f, nx, ny, x, y, nd1, nd2, nok, curve+nok, straight+!nok)) return true;
                        tmp_stk.pop_back();
                    }
                }
                return false;
            };

            int x = gx, y = gy;
            tmp_stk.emplace_back(x, y);
            x += dx[d2]; y += dy[d2];
            tmp_stk.emplace_back(x, y);
            if (g[x][y] == 'T') continue;
            if (d2%2 == 0) d1++;
            x += dx[d1]; y += dy[d1];
            tmp_stk.emplace_back(x, y);
            if (g[x][y] == 'T') continue;
            if (dfs(dfs, x, y, x-dx[d1], y-dy[d1], d1, d2))
            {
                best_score = score;
                best_path = path;
                found = true;
                break;
            }
        }
    }
    { // apply res to grid
        set<pii> st;
        for (auto [x, y]: best_path) st.emplace(x, y);
        rep(i, (int)best_path.size()-1)
        {
            auto [x, y] = best_path[i];
            if (g[x][y] != '.' && g[x][y] != 'G') assert(0);
            if (g[x][y] != 'G') g[x][y] = 'A';
            rep(d, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (st.count({nx, ny})) continue;
                if (g[nx][ny] == '.') 
                {
                    g[nx][ny] = 'T';
                    res.emplace_back(nx, ny);
                }
            }
        }
    }
    return res;
}

// 見通しの悪さを評価する関数(差分を評価)
double vis_score(int type, vector<pii> &add_trees, vector<pii> &del_trees)
{
    double delta_score = 0, delta_count = tmp_new_space_count - tmp_pre_space_count;
    auto calc_delta = [&](bool is_add=true) -> double
    {
        double res = 0, minus = (is_add ? -1.0: 1.0);
        vector<pii> trees = (is_add ? add_trees: del_trees);
        for (auto [x, y]: trees)
        {
            if (rows[x].size())
            {
                int left = 0, right = 0;
                auto it = rows[x].lower_bound(y);
                if (it != rows[x].begin())
                {
                    left += abs(y - *prev(it));
                }
                if (it != rows[x].end())
                {
                    right += abs(*it - y);
                }
                res += ((left-1)*right + (right-1)*left)*minus; 
            }
            if (cols[y].size())
            {
                int up = 0, down = 0;
                auto it = cols[y].lower_bound(x);
                if (it != cols[y].begin())
                {
                    up += abs(x - *prev(it));
                }
                if (it != cols[y].end())
                {
                    down += abs(*it - x);
                }
                res += ((up-1)*down + (down-1)*up)*minus;
            }
        }
        return res;
    };
    if (type == 0)
    {
        delta_score += calc_delta(true);
    }
    else if (type == 1 ||  type == 2)
    {
        delta_score += calc_delta(false);
    }
    else if (type == 3)
    {
        delta_score += calc_delta(true);
        delta_score += calc_delta(false);
    }
    else 
    {
        return 0;
    }
    double res = (tmp_pre_space_count*delta_score-(global_vis_score*tmp_pre_space_count)*delta_count)/(tmp_new_space_count*tmp_pre_space_count);
    return res;
}

// 盤面全体の入り組み具合を評価する関数
// {曲がり角の数/距離の総和, 到達可能なマスの数, SからGまでの距離, 通気性の評価, Gまで到達可能か}
tuple<double, double, double, double, bool> curve_score()
{
    queue<pii> q;
    q.emplace(gx, gy);
    dist.assign(n+2, vector<ll>(n+2, INF));
    dist[gx][gy] = 0;
    vector<vector<pii>> from(n+2, vector<pii>(n+2, {-1, -1}));
    vector<vector<int>> turns(n+2, vector<int>(n+2, 0));
    while(!q.empty())
    {
        auto [x, y] = q.front(); q.pop();
        rep(i, 4)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (g[nx][ny] == 'T') continue;
            int nd = dist[x][y] + 1;
            int nt = turns[x][y];
            if (from[x][y] != pii{-1, -1} && (nx - x != x - from[x][y].fi || ny - y != y - from[x][y].se))
            {
                nt++;
            }
            if (nd < dist[nx][ny] || (nd == dist[nx][ny] && nt < turns[nx][ny]))
            {
                dist[nx][ny] = nd;
                turns[nx][ny] = nt;
                from[nx][ny] = {x, y};
                q.emplace(nx, ny);
            }
        }
    }
    double res = 0, dists = 0, cnt = 0, diff = 0;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (dist[i][j] != INF) 
        {
            res += turns[i][j];
            dists += dist[i][j];
            cnt++;
        }
        if (g[i][j] == 'T') // calc diff
        {
            rep(d, 2)
            {
                int ni1 = i+dx[d], nj1 = j+dy[d];
                int ni2 = i+dx[d+2], nj2 = j+dy[d+2];
                if (dist[ni1][nj1] != INF && dist[ni2][nj2] != INF)
                {
                    diff += abs(dist[ni1][nj1] - dist[ni2][nj2]);
                }
            }
            int ni1 = i+dx[1], nj1 = j+dy[1];
            int ni2 = i+dx[2], nj2 = j+dy[2];
            int ni3 = i+1, nj3 = j+1;
            if (dist[ni1][nj1] != INF && dist[ni2][nj2] != INF && g[ni3][nj3] == 'T')
            {
                diff += abs(dist[ni1][nj1] - dist[ni2][nj2]);
            }
        }
    }
    if (dists) res /= dists;
    if (cnt) diff /= cnt;
    return {res, cnt, dist[sx][sy], diff, dist[sx][sy] != INF && dists != 0 && cnt != 0};
}

// 盤面を評価する関数
double calculate(int type, vector<pii> &add_trees, vector<pii> &del_trees)
{
    double res = 0;
    { // 盤面の入り組み具合を評価(**先にdistを計算しておくこと)
      // 到達可能なマスの数を評価  
        auto [d_curve, d_spread, d_sg, d_accessible, reachable] = curve_score();
        if (!reachable) return -LINF;
        res += d_curve * t_curve;
        res += d_spread * t_spread;
        res += d_sg * t_sg;
        res -= d_accessible * t_accessible;

        tmp_pre_space_count = global_space_count;
        tmp_new_space_count = d_spread;
        tmp_diff_vis_score = vis_score(type, add_trees, del_trees);
        res -= tmp_diff_vis_score * t_vis;
    }
    return res;
}

// ランダムに近傍操作を繰り返して、評価関数の値を最適化する
vector<pii> alignment()
{
    auto start = chrono::steady_clock::now();
    auto res_g = goal_alignment();
    vector<pii> res = initial_alignment(), cand_add;
    initialize_global_variables();
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (g[i][j] == '.') cand_add.emplace_back(i, j);
    }
    printvv(g);
    // tmp for revert
    vector<pii> tmp_add, tmp_del;

    // for output
    double best_score = calculate(-1, tmp_add, tmp_del);
    // rand
    random_device rd;
    mt19937 gen(rd());
    std::uniform_int_distribution<> op_dist(1, 100);

    // main loop
    while(1)
    {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start).count();
        debug_elapsed = elapsed;
        if (elapsed > a_limit) break;
        double ratio = min(1.0, (double)elapsed/a_limit);   
        tmp_add.clear();
        tmp_del.clear();

        // operation
        int op_kind = -1, op_times = 0;
        vector<int> op_rand(a_r.size());
        rep(i, a_l.size())
        {
            if (ratio <= a_l[i])
            {
                std::uniform_int_distribution<> t_dist(1, a_n[i]);
                op_times = t_dist(gen);
                for (int j = 0; j < a_r.size(); j++)
                {
                    op_rand[j] = a_r[i][j];
                }
                break;
            }
        }

        {
            int rand_op = op_dist(gen), sum = 0;
            rep(i, op_rand.size())
            {
                sum += op_rand[i];
                if (rand_op <= sum)
                {
                    op_kind = i;
                    break;
                }
            }
        }
        // ----------------近傍操作-----------------
        // add
        if (op_kind == 0 && cand_add.size())
        {
            auto calc_spacing = [&](int x, int y) -> ll
            {
                ll res = 0;
                if (rows[x].size())
                {
                    auto it = rows[x].lower_bound(y);
                    if (it != rows[x].begin())
                    {
                        res += abs(y - *prev(it) - 1);
                    }
                    if (it != rows[x].end())
                    {
                        res += abs(*it - 1 - y);
                    }
                }
                if (cols[y].size())
                {
                    auto it = cols[y].lower_bound(x);
                    if (it != cols[y].begin())              
                    {
                        res += abs(x - *prev(it) - 1);
                    }
                    if (it != cols[y].end())
                    {
                        res += abs(*it - 1 - x);          
                    }
                }
                return res;
            };
            sort(rng(cand_add), [&](pii a, pii b)
            {
                return calc_spacing(a.fi, a.se) < calc_spacing(b.fi, b.se);
            });
            rrep2(i, cand_add.size(), 0)
            {
                if (tmp_add.size() >= op_times) break;
                auto [x, y] = cand_add[i];
                g[x][y] = 'T';
                res.emplace_back(x, y);
                tmp_add.emplace_back(x, y);
            }
            for (auto [x, y]: tmp_add)
            {
                cand_add.erase(find(rng(cand_add), pii{x, y}));
                rows[x].emplace(y);
                cols[y].emplace(x);
            }
        }   
        // del
        else if (op_kind == 1 && res.size())
        {
            auto calc_diff = [&](int x, int y) -> ll
            {
                ll res = 0;
                rep(d, 2)
                {
                    int ni1 = x+dx[d], nj1 = y+dy[d];
                    int ni2 = x+dx[d+2], nj2 = y+dy[d+2];
                    if (dist[ni1][nj1] != INF && dist[ni2][nj2] != INF)
                    {
                        res += abs(dist[ni1][nj1] - dist[ni2][nj2]);
                    }
                }
                int ni1 = x+dx[1], nj1 = y+dy[1];
                int ni2 = x+dx[2], nj2 = y+dy[2];
                int ni3 = x+1, nj3 = y+1;
                if (dist[ni1][nj1] != INF && dist[ni2][nj2] != INF && g[ni3][nj3] == 'T')
                {
                    res += abs(dist[ni1][nj1] - dist[ni2][nj2]);
                }
                return res;
            };
            sort(rng(res), [&](pii a, pii b)
            {
                return calc_diff(a.fi, a.se) < calc_diff(b.fi, b.se);
            });
            rrep2(i, res.size(), 0)
            {
                if (tmp_del.size() >= op_times) break;
                auto [x, y] = res[i];
                g[x][y] = '.';
                tmp_del.emplace_back(x, y);
                cand_add.emplace_back(x, y);
            }
            for (auto [x, y]: tmp_del)
            {
                res.erase(find(rng(res), pii{x, y}));
                rows[x].erase(y);
                cols[y].erase(x);
            }
        }
        else if (op_kind == 2 && res.size())
        {
            shuffle(rng(res), gen);
            rrep2(i, res.size(), 0)
            {
                if (tmp_del.size() >= op_times) break;
                auto [x, y] = res[i];
                bool ok = false;
                rep(d, 4)
                {
                    int nx1 = x + dx[d], ny1 = y + dy[d];
                    int nx2 = x + dx[(d+1)%4], ny2 = y + dy[(d+1)%4];
                    if (g[nx1][ny1] == 'T' && g[nx2][ny2] == 'T')
                    {
                        ok = true;
                        break;
                    }
                }
                if (ok)
                {
                    g[x][y] = '.';
                    tmp_del.emplace_back(x, y);
                    cand_add.emplace_back(x, y);
                }
            }
            for (auto [x, y]: tmp_del)
            {
                res.erase(find(rng(res), pii{x, y}));
                rows[x].erase(y);
                cols[y].erase(x);
            }
        }
        // move
        else if (op_kind == 3 && res.size())
        {
            shuffle(rng(res), gen);
            {
                set<pii> st;
                rep(i, res.size())
                {
                    if (tmp_del.size() >= op_times) break;
                    auto [x, y] = res[i];
                    vector<int> d_list = {0, 1, 2, 3, 4, 5, 6, 7};
                    shuffle(rng(d_list), gen);
                    for (auto d: d_list)
                    {
                        int nx = x + dxx[d];
                        int ny = y + dyy[d];
                        if (g[nx][ny] == '.' && !st.count({nx, ny}))
                        {
                            tmp_del.emplace_back(x, y);
                            tmp_add.emplace_back(nx, ny);
                            st.emplace(nx, ny);
                            break;
                        }
                    }
                }
            }
            // del
            for (auto [x, y]: tmp_del)
            {
                g[x][y] = '.';
                res.erase(find(rng(res), pii{x, y}));
                rows[x].erase(y);
                cols[y].erase(x);
            }
            for (auto [nx, ny]: tmp_add)
            {
                g[nx][ny] = 'T';
                cand_add.erase(find(rng(cand_add), pii{nx, ny}));
                rows[nx].emplace(ny);
                cols[ny].emplace(nx);
            }
            // add
            for (auto [x, y]: tmp_del)
            {
                cand_add.emplace_back(x, y);
            }
            for (auto [nx, ny]: tmp_add)
            {
                res.emplace_back(nx, ny);
            }
        }
        else
        {
            continue;
        }

        // ----------------------- SA -----------------------
        // evaluate
        auto score = calculate(op_kind, tmp_add, tmp_del);
         
        debug_times++;
        if (ratio < 0.2) debug_first_times++;
        else if (ratio < 0.5) debug_middle_times++;
        else debug_last_times++;

        // double T = T0 * pow(Tend / T0, ratio); 
        double delta = score - best_score;
        // static uniform_real_distribution<double> ur01(0.0, 1.0);
        // bool accept = (delta > 0) || (exp(delta / T) > ur01(gen));
        bool accept = (delta > 0);
        if (accept) // update
        {
            global_vis_score += tmp_diff_vis_score;
            global_space_count = tmp_new_space_count;
            best_score = score;
            debug_update++;
            if (delta < 0)
            {
                if (ratio < 0.5) debug_first_accept++;
                else if (ratio < 0.75) debug_middle_accept++;
                else debug_last_accept++;
            }
            continue;
        }

        // ------------------------------ revert ------------------------------
        if (op_kind == 0)
        {
            for (auto [x, y]: tmp_add)
            {
                g[x][y] = '.';
                res.pop_back();
                cand_add.emplace_back(x, y);
                rows[x].erase(y);
                cols[y].erase(x);
            }
        }
        else if (op_kind == 1 || op_kind == 2)
        {
            for (auto [x, y]: tmp_del)
            {
                g[x][y] = 'T';
                res.emplace_back(x, y);
                cand_add.pop_back();
                rows[x].emplace(y);
                cols[y].emplace(x);
            }
        }
        else if (op_kind == 3)
        {
            // del
            for (auto [x, y]: tmp_del)
            {
                g[x][y] = 'T';
                cand_add.pop_back();
            }
            for (auto [nx, ny]: tmp_add)
            {
                g[nx][ny] = '.';
                res.pop_back();
            }
            // add
            for (auto [x, y]: tmp_del)
            {
                res.emplace_back(x, y);
            }
            for (auto [nx, ny]: tmp_add)
            {
                cand_add.emplace_back(nx, ny);
            }
        }
    }
    for (auto [x, y]: res_g) res.emplace_back(x, y);
    debug_score = best_score;
    return res;
}

void solve()
{
    cin >> n >> gx >> gy; gx++; gy++;
    sx = 1; sy = n/2+1;
    g.resize(n+2, vector<char>(n+2, 'T'));
    rep(i, n)rep(j, n)
    {
        cin >> g[i+1][j+1];
    }
    g[sx][sy] = 'S'; // start
    g[gx][gy] = 'G'; // goal

    int x = -1, y = -1;
    // first
    {
        if (!IS_LOCAL_JUDGE)
        { // for judge
            cin >> x >> y; x++; y++;
            int m; cin >> m;
            rep(i, m)
            {
                int a, b; cin >> a >> b; a++; b++;
            }
        }
        auto res = alignment();
        // for judge
        if (!IS_LOCAL_JUDGE)
        {
            
            cout << res.size() << " ";
            for (auto [a, b] : res) cout << a-1 << " " << b-1 << " ";
            cout << endl;
        }
        // for local test
        if (IS_LOCAL_JUDGE)
        {
            vector<pii> Q;
            rep(i, n*n-1)
            {
                int qi, qj; cin >> qi >> qj;
                Q.emplace_back(qi, qj);
            }
            Sim::sim_init(Q);
            { // for one shot output
                // cout << res.size() << " ";
                // for (auto [a, b] : res) cout << a-1 << " " << b-1 << " ";
                // cout << endl;
            }
            Sim::sim_apply_trees(res); 
            Sim::sim_step();
            if (Sim::sim_finished()) return;
        }
    }
    // main loop
    while(1)
    {
        if (!IS_LOCAL_JUDGE)
        { // for judge (if execute in local, need to comment out)
            cin >> x >> y; x++; y++;
            int m; cin >> m;
            rep(i, m)
            {
                int a, b; cin >> a >> b; a++; b++;
            }
            if (x == gx && y == gy) return;
            cout << 0 << endl;
        }
        else
        { // for local test
            if (Sim::sim_finished()) return;
            // for one shot output
            {
                // cout << 0 << endl; 
            }
            Sim::sim_step();   
        }
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
    if (IS_LOCAL_JUDGE)
    {
        cout << Sim::sim_get_steps() << endl; // ★
        rep(i, n+2)
        {
            rep(j, n+2) cerr << g[i][j];
            cerr << endl;
        }
        // rep(i, n+2)
        // {
        //     rep(j, n+2) cout << (dist[i][j] == INF ? -1: dist[i][j])<< " ";
        //     cout << endl;
        // }
        cerr << "debug_times: " << debug_times << endl;
        cerr << "debug_update: " << debug_update << endl;
        cerr << "debug_elapsed: " << debug_elapsed << endl;
        cerr << "debug_score: " << debug_score << endl;
        cerr << "debug_first_accept_rate: " << debug_first_accept * 100.0 / debug_first_times << "%" << endl;
        cerr << "debug_middle_accept_rate: " << debug_middle_accept * 100.0 / debug_middle_times << "%" << endl;
        cerr << "debug_last_accept_rate: " << debug_last_accept * 100.0 / debug_last_times << "%" << endl;   
        cerr << "system_score: " << Sim::sim_get_steps() << endl;
        // printvv(debug_best_g);
    }
    return 0;
}