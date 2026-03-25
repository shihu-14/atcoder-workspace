/*
-最適化手法
・山登り法

-初期配置
・盤面全体をt_s_dir*t_s_dirの正方形ブロックに分解し,
各ブロックに対してx,y座標の和が偶数or奇数のマスに木を置く.
ただし, S->Gの最短経路上には木を置かない.
・ゴール周りは,一本道のジグザグの道を探していき, 見つかり次第終了し, その道の両脇に木を置く.
さらに, その木の周りは空きマスにする.
・釣り針型

-近傍操作
・木をn本追加
    ・ある空きマスについて, 上下左右に空きますが多い順に優先して木を置く
・木をn本削除
    ・ある木について, 上下左右の空きマスのGからのdistの差が大きい順に優先して木を削る
    ・以下のようなブロックがあった時に, 左上の不要な木を削る.
        TT
        T.
    ・空きマスだが, Sから到達できない木を削る
・木をn本移動
    ・ある木を上下左右斜め方向にランダムに動かす.

-評価の要素
・事故らず安定した出力が大前提
    ・木を大量に置きすぎない.
        **盤面全体に対して設置する木の割合を決める.
    ・Sから到達できない空きマスを少なめに.
        **全空きマスうち, Sから到達可能なマスの割合を決める.
    ・盤面全体が行き来しやすい. (1通にならない)
        **閉路の数を数える.
    ・Gを野晒しにしない.
        **初期条件
    (・局所解に陥らない.)
・高得点を狙うには
    ・曲がり角を多くする.
    ・ある空きマスから新たに確認済みになるマスの数を減らす.
        **曲がり角の多さの割合
    ・SからGまでの距離を長くする.
        **S->Gの最短距離(長いほど良い)
    ・Gを程よく堅固にする.
        **初期条件
        **

・盤面全体がアクセスしやすい(多いほど良い)

-その他
・vis_scoreの計算は差分のみを評価している.

-メモ

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
vector<vector<char>> g, tmp_g;
vector<vector<int>> dist, turns, tmp_dist;
vector<vector<pii>> from;
vector<set<int>> rows, cols; // 各行・各列の木の位置
queue<pii> q;
int global_trees = 0;

// parameter
double a_limit = 1900;
 // 時間帯ごとの操作の割合%(add, del1. del2, move)
vector<vector<int>> a_r = {{80, 5, 5, 10}, {50, 10, 10, 30}, 
    {25, 30, 5, 40}, {25, 30, 5, 40}};
vector<double> a_l = {0.15, 0.4, 0.70, 1.0}; // 時間帯の境界(%)
vector<ll> a_n = {60, 30, 30, 10}; // 各時間帯で一度に操作する回数

double t_curve = 100; // 盤面の入り組み具合を評価する
double t_spread = 100; // 到達可能なますの数を評価
double t_sg = 1; // スタートからゴールまでの距離を評価する
double t_loop = 1; // 盤面全体が行き来しやすいことを評価する
double t_tree = 100; // 木の本数を評価する

double t_curve_rate = 0.5;
double t_spread_rate = 1;
double t_tree_density = 0.4; // 盤面全体に対して設置する木の割合

ll t_s_dir = 10; // 初期配置の間隔
ll t_g_edir = 6; // ゴール周りには木を置かない
ll t_g_dir = 5; // ゴール周りの堅固さを評価する際の範囲
ll t_g_straight = 2; // ゴール周りの堅固さを評価する際の直線距離
// double T0 = 5e-12, Tend = 5e-12; // SAの温度パラメータ

// debug
int debug_all_times = 0;
int debug_all_updates = 0;
int debug_elapsed = 0;
double debug_score = 0;

double debug_trees_rate = 0;
double debug_curve_rate = 0;
double debug_spread_rate = 0;
int debug_sg = 0;
int debug_loop = 0;
vector<int> debug_times, debug_updates, debug_op_times;
// int debug_first_accept = 0;
// int debug_middle_accept = 0;
// int debug_last_accept = 0;

// グローバル変数を初期化する
void initialize_global_variables()
{
    g.resize(n+2, vector<char>(n+2, 'T'));
    tmp_g.resize(n+2, vector<char>(n+2, 'T'));
    dist.resize(n+2, vector<int>(n+2, INF));
    tmp_dist.resize(n+2, vector<int>(n+2, INF));
    turns.resize(n+2, vector<int>(n+2, 0));
    from.resize(n+2, vector<pii>(n+2, {-1,-1}));
    rows.resize(n+2, set<int>());
    cols.resize(n+2, set<int>());
    debug_times.resize(a_l.size());
    debug_updates.resize(a_l.size());
    debug_op_times.resize(a_r[0].size());
}

// 初期配置
vector<pii> initial_alignment(){
    auto dijkstra = [&]()
    {
        queue<pii> empty_q; q.swap(empty_q);
        rep(i, n+2) fill(rng(dist[i]), INF);
        rep(i, n+2) fill(rng(from[i]), pii{-1,-1});
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
                from[nx][ny] = {x,y};
                q.emplace(nx,ny);
            }
        }
    };
    dijkstra();
    vector<vector<bool>> onpath(n+2, vector<bool>(n+2, false));
    { // onpathの更新
        int x = sx, y = sy;
        while(!(x == gx && y == gy))
        {
            onpath[x][y] = true;
            auto p = from[x][y];
            if (p.fi == -1 || p.se == -1) break;
            tie(x , y) = p;
        }
        onpath[gx][gy] = true;
    }
    // main loop
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
                    if ((ni+nj)%2 != (i/t_s_dir+j/t_s_dir)%2) continue;
                    if (onpath[ni][nj]) continue;
                    g[ni][nj] = 'T';
                    res.emplace_back(ni, nj);
                    rows[ni].insert(nj);
                    cols[nj].insert(ni);
                }
            }
        }
    }
    dijkstra();
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
        tmp_g = g;
        {
            set<pii> st;
            for (auto [x, y]: path) st.emplace(x, y);
            for (int i = 0; i+1 < path.size(); i++)
            {
                auto [x, y] = path[i];
                if (tmp_g[x][y] != '.' && tmp_g[x][y] != 'G') assert(0);
                rep(d, 4)
                {
                    int nx = x + dx[d];
                    int ny = y + dy[d];
                    if (st.count({nx, ny})) continue;
                    if (tmp_g[nx][ny] == '.') tmp_g[nx][ny] = 'T';
                }
            }
        }

        queue<pii> empty_q; q.swap(empty_q);
        q.emplace(gx, gy);
        rep(i, n+2) fill(rng(dist[i]), INF);
        dist[gx][gy] = 0;
        while (q.size())
        {
            auto [x, y] = q.front(); q.pop();
            if (x == sx && y == sy) return dist[x][y];
            rep(d, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (tmp_g[nx][ny] == 'T') continue;
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
        set<pii> st, st2; // st: 道のマス, st2: 道の周囲のマス
        for (auto [x, y]: best_path) st.emplace(x, y);
        for (int i = 0; i+1 < best_path.size(); i++)
        {
            auto [x, y] = best_path[i];
            if (g[x][y] != '.' && g[x][y] != 'G') assert(0);
            if (g[x][y] != 'G') g[x][y] = 'A';
            rep(d, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (st.count({nx, ny})) continue;
                st2.emplace(nx, ny);
                if (g[nx][ny] == '.') 
                {
                    g[nx][ny] = 'T';
                    res.emplace_back(nx, ny);
                    rows[nx].insert(ny);
                    cols[ny].insert(nx);
                }
            }
        }
        for (auto [x, y]: st2) // 木の周囲は空きマスに
        {
            rep(d, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (nx < 1 || n < nx || ny < 1 || n < ny) continue;
                if (g[nx][ny] == '.')
                {
                    g[nx][ny] = 'B';
                }
            }
        }
    }
    return res;
}

// 盤面全体の入り組み具合を評価する関数
// {曲がり角の数/距離の総和, 到達可能なマスの数, SからGまでの距離, 通気性の評価, Gまで到達可能か}
tuple<double, double, double, double, bool> curve_score()
{
    queue<pii> empty_q;
    q.swap(empty_q);
    rep(i, n+2) fill(rng(tmp_dist[i]), INF);
    rep(i, n+2) fill(rng(turns[i]), 0);
    rep(i, n+2) fill(rng(from[i]), pii{-1, -1});
    q.emplace(gx, gy);
    tmp_dist[gx][gy] = 0;
    turns[gx][gy] = 0;
    while(!q.empty())
    {
        auto [x, y] = q.front(); q.pop();
        rep(i, 4)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (g[nx][ny] == 'T') continue;
            int nd = tmp_dist[x][y] + 1;
            int nt = turns[x][y];
            if (from[x][y] != pii{-1, -1} && (nx - x != x - from[x][y].fi || ny - y != y - from[x][y].se))
            {
                nt++;
            }
            if (nd < tmp_dist[nx][ny] || (nd == tmp_dist[nx][ny] && nt < turns[nx][ny]))
            {
                tmp_dist[nx][ny] = nd;
                turns[nx][ny] = nt;
                from[nx][ny] = {x, y};
                q.emplace(nx, ny);
            }
        }
    }

    double res = 0, dists = 0, f4 = 0, e = 0, v = 0, v2 = 0;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (g[i][j] != 'T') v2++; // 全空きマス
        if (tmp_dist[i][j] != INF) 
        {
            res += turns[i][j];
            dists += tmp_dist[i][j];
            v++;
            bool loop = true;
            rep(d1, 2)rep(d2, 2)
            {
                if (tmp_dist[i+d1][j+d2] == INF) loop = false;
            }
            if (loop) f4++;
            rep(d, 4)
            {
                int ni = i + dx[d];
                int nj = j + dy[d];
                if (tmp_dist[ni][nj] == INF) continue;
                e++;
            }
        }
    }
    if (dists) res /= dists;
    return {res, v/v2, tmp_dist[sx][sy], e/2-v+1-f4, tmp_dist[sx][sy] != INF && dists != 0};
}

// 盤面を評価する関数
double calculate()
{
    double res = 0;
    auto [d_curve, d_spread, d_sg, d_loop, reachable] = curve_score();
    { // 盤面の入り組み具合を評価(**先にdistを計算しておくこと)
      // 到達可能なマスの数を評価  
        if (!reachable) return -LINF;
        res -= abs(d_curve-t_curve_rate)*t_curve;
        res -= abs(d_spread-t_spread_rate)*t_spread;
        res += d_sg * t_sg;
        res += d_loop * t_loop;
        res -= abs((double)global_trees/(n*n)-t_tree_density)*t_tree;
    }
    { // debug
        debug_curve_rate = d_curve;
        debug_spread_rate = d_spread;
        debug_sg = d_sg;
        debug_loop = d_loop;
    }
    return res;
}

// ランダムに近傍操作を繰り返して、評価関数の値を最適化する
vector<pii> alignment()
{
    auto start = chrono::steady_clock::now();
    auto res_g = goal_alignment();
    vector<pii> res = initial_alignment(), cand_add;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (g[i][j] == '.') cand_add.emplace_back(i, j);
        if (g[i][j] == 'T') global_trees++;
    }
    // tmp for revert
    vector<pii> tmp_add, tmp_del;

    // for output
    double best_score = calculate();
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
        vector<int> op_rand(a_r[0].size()); // 各操作が選ばれる確率を決定.
        rep(i, a_l.size()) // 各操作の確率の決定
        {
            if (ratio <= a_l[i])
            {
                std::uniform_int_distribution<> dist_n(1, a_n[i]);
                op_times = dist_n(gen);
                for (int j = 0; j < a_r[0].size(); j++)
                {
                    op_rand[j] = a_r[i][j];
                }
                break;
            }
        }
        { // 各操作の選択
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
        auto erase_one = [&](vector<pii> &v, pii e)
        {
            auto it = find(rng(v), e);
            if (it != v.end()) v.erase(it);
        };
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
                erase_one(cand_add, pii{x, y});
                rows[x].emplace(y);
                cols[y].emplace(x);
            }
            global_trees += tmp_add.size();
        }   
        // del 1
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
                rep(d, 4)
                {
                    int ni1 = x + dx[d], nj1 = y + dy[d];
                    int ni2 = x + dx[(d+1)%4], nj2 = y + dy[(d+1)%4];
                    int ni3 = x + dx[d]+dx[(d+1)%4], nj3 = y + dy[d]+dy[(d+1)%4];
                    if (dist[ni1][nj1] != INF && dist[ni2][nj2] != INF && g[ni3][nj3] == 'T')
                    {
                        res += abs(dist[ni1][nj1] - dist[ni2][nj2]);
                    }
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
                erase_one(res, pii{x, y});
                rows[x].erase(y);
                cols[y].erase(x);
            }
            global_trees -= tmp_del.size();
        }
        // del 2
        else if (op_kind == 2 && res.size())
        {
            auto is_delete = [&](int x, int y) -> bool
            {
                rep(d, 4)
                {
                    int nx1 = x + dx[d], ny1 = y + dy[d];
                    int nx2 = x + dx[(d+1)%4], ny2 = y + dy[(d+1)%4];
                    if (g[nx1][ny1] == 'T' && g[nx2][ny2] == 'T')
                    {
                        return true;
                    }
                    if (g[nx1][ny1] == '.' && dist[nx1][ny1] == INF)
                    {
                        return true;
                    }
                }
                return false;
            };
            shuffle(rng(res), gen);
            rrep2(i, res.size(), 0)
            {
                if (tmp_del.size() >= op_times) break;
                auto [x, y] = res[i];
                if (is_delete(x, y))
                {
                    g[x][y] = '.';
                    tmp_del.emplace_back(x, y);
                    cand_add.emplace_back(x, y);
                }
            }
            for (auto [x, y]: tmp_del)
            {
                erase_one(res, pii{x, y});
                rows[x].erase(y);
                cols[y].erase(x);
            }
            global_trees -= tmp_del.size();
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
                erase_one(res, pii{x, y});
                rows[x].erase(y);
                cols[y].erase(x);
            }
            for (auto [nx, ny]: tmp_add)
            {
                g[nx][ny] = 'T';
                erase_one(cand_add, pii{nx, ny});
            }
            // add
            for (auto [x, y]: tmp_del)
            {
                cand_add.emplace_back(x, y);
            }
            for (auto [nx, ny]: tmp_add)
            {
                res.emplace_back(nx, ny);
                rows[nx].emplace(ny);
                cols[ny].emplace(nx);
            }
        }
        else
        {
            continue;
        }
        // ----------------------- SA -----------------------
        // evaluate
        auto score = calculate();
         
        debug_all_times++;
        rep(i, a_l.size())
        {
            if (ratio <= a_l[i])
            {
                debug_times[i]++;
                break;
            }
        }
        debug_op_times[op_kind]++;
        // double T = T0 * pow(Tend / T0, ratio); 
        double delta = score - best_score;
        // static uniform_real_distribution<double> ur01(0.0, 1.0);
        // bool accept = (delta > 0) || (exp(delta / T) > ur01(gen));
        bool accept = delta > 0;
        if (accept) // update
        {
            dist = tmp_dist;
            best_score = score;
            debug_all_updates++;
            debug_updates[op_kind]++;
            // if (delta < 0)
            // {
            //     if (ratio < 0.5) debug_first_accept++;
            //     else if (ratio < 0.75) debug_middle_accept++;
            //     else debug_last_accept++;
            // }
            continue;
        }

        // ------------------------------ revert ------------------------------
        if (op_kind == 0)
        {
            for (auto [x, y]: tmp_add)
            {
                g[x][y] = '.';
                res.pop_back();
                // erase_one(res, pii{x, y});
                cand_add.emplace_back(x, y);
                rows[x].erase(y);
                cols[y].erase(x);
            }
            global_trees -= tmp_add.size();
        }
        else if (op_kind == 1 || op_kind == 2)
        {
            for (auto [x, y]: tmp_del)
            {
                g[x][y] = 'T';
                cand_add.pop_back();
                // erase_one(cand_add, pii{x, y});
                res.emplace_back(x, y);
                rows[x].emplace(y);
                cols[y].emplace(x);
            }
            global_trees += tmp_del.size();
        }
        else if (op_kind == 3)
        {
            // del
            for (auto [x, y]: tmp_del)
            {
                g[x][y] = 'T';
                cand_add.pop_back();
                // erase_one(cand_add, pii{x, y});
            }
            for (auto [nx, ny]: tmp_add)
            {
                g[nx][ny] = '.';
                res.pop_back();
                // erase_one(res, pii{nx, ny});
                rows[nx].erase(ny);
                cols[ny].erase(nx);
            }
            // add
            for (auto [x, y]: tmp_del)
            {
                res.emplace_back(x, y);
                rows[x].emplace(y);
                cols[y].emplace(x);
            }
            for (auto [nx, ny]: tmp_add)
            {
                cand_add.emplace_back(nx, ny);
            }
        }
    }

    res.insert(res.end(), rng(res_g));
    debug_trees_rate = (double)global_trees/(n*n);
    debug_score = best_score;
    return res;
}

void solve()
{
    cin >> n >> gx >> gy; gx++; gy++;
    sx = 1, sy = n/2+1;
    initialize_global_variables();
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
            cout << -1 << endl;
            return;
        }
        // for local test
        if (IS_LOCAL_JUDGE)
        {
            vector<pii> Q; Q.reserve(n*n-1);
            rep(i, n*n-1)
            {
                int qi, qj; cin >> qi >> qj;
                Q.emplace_back(qi, qj);
            }
            Sim::sim_init(Q);
            { // for one shot output
                cout << res.size() << " ";
                for (auto [a, b] : res) cout << a-1 << " " << b-1 << " ";
                cout << endl;
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
                cout << 0 << endl; 
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
        // cout << Sim::sim_get_steps() << endl; // ★
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
        cerr << "------------main information------------" << endl;
        cerr << "debug_all_times: " << debug_all_times << endl;
        cerr << "debug_all_updates: " << debug_all_updates << endl;
        cerr << "debug_elapsed: " << debug_elapsed << endl;
        cerr << "debug_score: " << debug_score << endl;
        cerr << "------------detailed information------------" << endl;
        rep(i, debug_times.size())
        {
            cerr << "debug_times[" << i << "]: " << debug_times[i] << endl;
        }
        rep(i, debug_updates.size())
        {
            cerr << "debug_updates_rate[" << i << "]: " << debug_updates[i]*100.0/debug_times[i] << "%" << endl;
        }
        rep(i, debug_op_times.size())
        {
            cerr << "debug_op_times[op = " << i << "]: " << debug_op_times[i] << endl;
        }
        cerr << "debug_trees_rate: " << debug_trees_rate << endl;
        cerr << "debug_curve_rate: " << debug_curve_rate << endl;
        cerr << "debug_spread_rate: " << debug_spread_rate << endl;
        cerr << "debug_sg: " << debug_sg << endl;
        cerr << "debug_loop: " << debug_loop << endl;
        cerr << "system_score: " << Sim::sim_get_steps() << endl;
    }
    return 0;
}