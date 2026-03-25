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
vector<vector<int>> dist, turns, tmp_dist;
vector<vector<pii>> from;
vector<set<int>> rows, cols; // 各行・各列の木の位置
queue<pii> q;
int global_trees = 0;

// parameter
double a_limit = 1900;

// 時間帯ごとの操作の割合%(add1, add2, add3, del1. del2, del3, move)
/*
    add1:  見通しが良いマスに木を置く
    add2:  ランダム
    add3:  一本道を塞ぐ
    del1:  通気性を良くする
    del2:  不要な木を削る
    del3:  ランダム 
    move:  ランダムに8方向に動かす
*/
vector<vector<int>> a_r = {{20, 60, 0, 0, 0, 0, 20}, {30, 10, 0, 0, 30, 0, 30}, 
    {10, 0, 20, 0, 15, 15, 40}, {0, 0, 50, 0, 10, 10, 30}};
vector<double> a_l = {0.15, 0.4, 0.70, 1.0}; // 時間帯の境界(%)
vector<ll> a_n = {15, 15, 10, 8}; // 各時間帯で一度に操作する回数

double t_curve = 100; // 盤面の入り組み具合を評価する
double t_spread = 130; // 到達可能なますの数を評価
double t_corner = 100;
double t_sg = 1; // スタートからゴールまでの距離を評価する
double t_loop = 0; // 盤面全体が行き来しやすいことを評価する
double t_tree = 130; // 木の本数を評価する
double t_vis = 30;

double t_curve_rate = 0.45;
double t_spread_rate = 1;
double t_corner_rate = 0.15;
double t_tree_density = 0.48; // 盤面全体に対して設置する木の割合
double t_vis_rate = 4.8; // 全空きマスうち, Sから到達可能なマスの割合

ll t_s_dir = 20; // 初期配置の間隔
ll t_s_edir = 10;
ll t_g_edir = 0; // ゴール周りには木を置かない
ll t_g_dir = 1; // ゴール周りの堅固さを評価する際の範囲
ll t_g_straight = 2; // ゴール周りの堅固さを評価する際の直線距離
ll t_g_sgdir = 7;
ll t_gs_dir = 4; // ゴール周りの堅固さを評価する際の範囲
ll t_gs_straight = 2; // ゴール周りの堅固さを評価する際の直線距離
ll t_tol = 3; 
// double T0 = 5e-12, Tend = 5e-12; // SAの温度パラメータ

// debug
int debug_all_times = 0;
int debug_update = 0;
int debug_elapsed = 0;
double debug_score = 0;

double debug_trees_rate = 0;
double debug_curve_rate = 0;
double debug_spread_rate = 0;
double debug_corner_rate = 0;
int debug_sg = 0;
int debug_loop = 0;
double debug_vis = 0;
vector<int> debug_times, debug_op_times;

double debug_tmp_curve_rate = 0;
double debug_tmp_spread_rate = 0;
double debug_tmp_corner_rate = 0;
int debug_tmp_sg = 0;
int debug_tmp_loop = 0;
double debug_tmp_vis = 0;

// グローバル変数を初期化する
void initialize_global_variables()
{
    g.resize(n+2, vector<char>(n+2, 'T'));
    dist.resize(n+2, vector<int>(n+2, INF));
    tmp_dist.resize(n+2, vector<int>(n+2, INF));
    turns.resize(n+2, vector<int>(n+2, 0));
    from.resize(n+2, vector<pii>(n+2, {-1,-1}));
    rows.resize(n+2, set<int>());
    cols.resize(n+2, set<int>());
    debug_times.resize(a_l.size());
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
    // main 
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
                    if (abs(ni-gx)+abs(nj-gy) <= t_s_edir) continue;
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
    
    auto calc_dir = [](pii e)
    {       
        if (e.fi == 0) return e.se == 1 ? 1: 3;
        else return e.fi == 1 ? 2: 0;
    };
    auto is_ok = [](pii e1, pii e2)
    {
        return e1.fi*e2.fi + e1.se*e2.se == 0;
    };
    // Gからの空きマスの列(path)をgrid Gに反映させる. return {その周りに置いた木の座標}
    auto apply_path = [&](vector<pii> &path, bool fill_edge=true) -> vector<pii>
    {
        set<pii> used;
        vector<pii> res;
        rep(i, path.size()) used.emplace(path[i]);
        for (int i = 0; i+1 < path.size(); i++)
        {
            auto [x, y] = path[i];
            if (g[x][y] != '.' && g[x][y] != 'G')
            {
                assert(0);
            }
            rep(d, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (used.count({nx, ny})) continue;
                if (g[nx][ny] == '.') 
                {
                    g[nx][ny] = 'T';
                    res.emplace_back(nx, ny);
                }
            }
        }
        // 末端を処理するか.
        if (path.size() >= 2 && fill_edge)
        {
            auto [x1, y1] = path.back();
            auto [x2, y2] = path[path.size()-2];
            int d1 = calc_dir({x1-x2, y1-y2});
            rep(d, 4)
            {
                if (d == d1 || d == (d1+2)%4) continue;
                int nx = x1 + dx[d], ny = y1 + dy[d];
                if (used.count({nx, ny})) continue;
                if (g[nx][ny] == '.') 
                {
                    g[nx][ny] = 'T';
                    res.emplace_back(nx, ny);   
                }
            }
        }
        return res;
    };  
    // apply_pathで置いた木を元に戻す
    auto revert_path = [&](vector<pii> &trees) -> void
    {
        for (auto [x, y]: trees) g[x][y] = '.';
    };
    // apply_path(path)後のgridにおいて, S->Gに到達可能か. 可能ならその距離を返す. 不可能なら-INFを返す.
    // ただし, path上のマスを全て通ることが条件. そうでなければ-INFを返す.
    auto reachable = [&](vector<pii> &path, bool fill_edge=true) -> int
    {
        auto trees = apply_path(path, fill_edge);
        // printvv(g);
        queue<pii> empty_q; q.swap(empty_q);
        rep(i, n+2) fill(rng(dist[i]), INF);
        rep(i, n+2) fill(rng(from[i]), pii{-1,-1});
        q.emplace(sx, sy);
        dist[sx][sy] = 0;
        while (q.size())
        {
            auto [x, y] = q.front(); q.pop();
            if (x == gx && y == gy) 
            { // 到達可能. ただし, path上のマスを全て通ったか確認する.
                set<pii> tmp_path;
                for (auto [x, y]: path) tmp_path.emplace(x, y);
                while (!(x == sx && y == sy))
                {
                    tmp_path.erase({x, y});
                    if (tmp_path.size() == 0) break;
                    auto p = from[x][y];
                    if (p.fi == -1 || p.se == -1) break;
                    tie(x, y) = p;
                }
                tmp_path.erase({sx, sy});
                revert_path(trees);
                return tmp_path.size() == 0 ? dist[gx][gy] : -INF;
            }
            rep(d, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (g[nx][ny] == 'T') continue;
                if (dist[nx][ny] != INF) continue;
                dist[nx][ny] = dist[x][y] + 1;
                from[nx][ny] = {x, y};
                q.emplace(nx, ny);
            }
        }
        revert_path(trees);
        return -INF;
    };

    // main process
    vector<pii> res, best_path, sub_best_path; // 移動経路
    set<pii> best_target_trees, sub_best_target_trees; // ゴールまでの経路において, 視認しておくべき木.
    pll best_score = {-INF, -INF}, sub_best_score = {-INF, -INF}; // (pathの短さ, S-Gの距離の長さ} (大きいほど良い)
    auto search = [&](int tol) -> bool
    {
        bool found = false;
        for (int d2: {2, 1, 3, 0})
        {
            for (int d1: {0, 2})
            {
                vector<pii> tmp_stk;
                pll score = {-INF, -INF}, sub_score = {-INF, -INF}; // (pathの短さ, S-Gの距離の長さ} (大きいほど良い)
                // return {迂回する道, その道を辿る際に視認すべき木}
                 // ゴールへ迂回する道を探す. return {その道, その道を辿る際に視認すべき木, 視認できなかった木の数}
                auto return_alignment = [&]() -> tuple<vector<pii>, set<pii>, int>
                {
                    auto trees = apply_path(tmp_stk); // pathの周りに木を置いている.
                    vector<pii> q_trees;
                    { // 木を連結成分を作る (それを囲うように空きマスを探査するため)
                        set<pii> used_tree; // visitedを管理
                        for (auto [x, y]: tmp_stk) // pathの周りの木をq_treesに入れ初期化
                        {
                            rep(d, 8)
                            {
                                int nx = x + dxx[d], ny = y + dyy[d];
                                if (nx < 1 || n < nx || ny < 1 || n < ny) continue;
                                if (g[nx][ny] == 'T' && !used_tree.count({nx, ny}))
                                {
                                    q_trees.emplace_back(nx, ny);
                                    used_tree.emplace(nx, ny);
                                }
                            }
                        }
                        while(1) // 連結成分を広げる
                        {
                            vector<pii> tmp_q;
                            for (auto [x, y]: q_trees)
                            {
                                rep(d, 8)
                                {
                                    int nx = x + dxx[d];
                                    int ny = y + dyy[d];
                                    if (nx < 1 || n < nx || ny < 1 || n < ny) continue;
                                    if (g[nx][ny] == 'T' && !used_tree.count({nx, ny}))
                                    {
                                        tmp_q.emplace_back(nx, ny);
                                        used_tree.emplace(nx, ny);
                                    }
                                }
                            }
                            if (tmp_q.size() == 0) break;
                            q_trees.insert(q_trees.end(), rng(tmp_q));
                        }
                    }

                    // main process 
                    // returnする変数たち.
                    vector<pii> res; // 迂回する道(path)
                    set<pii> target_trees; // ゴールまでの経路において, 視認しておくべき木の最小の集合.
                    int not_visible_cnt = INF; // その木の内側のマスのうち, 視認できなかった木の数(小さいほど良い)
                    { // 迂回する道を探す
                        // 木の連結成分を囲うような空きマスを探す(road)
                        set<pii> road;
                        for (auto [x, y]: q_trees) // 木の周囲の空きマス(path以外)をroadに入れる
                        {
                            rep(d, 8)
                            {
                                int nx = x + dxx[d];
                                int ny = y + dyy[d];
                                if (road.count({nx, ny})) continue;
                                if (g[nx][ny] == '.' 
                                    && find(rng(tmp_stk), pii{nx, ny}) == tmp_stk.end())
                                {
                                    road.emplace(nx, ny);
                                }
                            }
                        }
                        // tmp_target_treesの決定
                        set<pii> tmp_target_trees; // loopの確認するための木の集合(視認した木を全て入れる)
                        // 外周は木であることは分かっている.
                        rep(i, n+2)
                        {
                            tmp_target_trees.emplace(i, 0);
                            tmp_target_trees.emplace(i, n+1);
                            tmp_target_trees.emplace(0, i);
                            tmp_target_trees.emplace(n+1, i);
                        }
                        // pathの最初以外の部分は, 視認できることが分かっている.
                        rep2(i, tol+1, tmp_stk.size())
                        {
                            auto [x, y] = tmp_stk[i];
                            rep(d, 4)
                            {
                                int nx = x, ny = y;
                                while(1)
                                {
                                    nx += dx[d];
                                    ny += dy[d];
                                    if (g[nx][ny] == 'T')
                                    {
                                        tmp_target_trees.emplace(nx, ny);
                                        break;
                                    }
                                }
                            }
                        }
                        // Sから視認できる木もいれる.
                        if (abs(sx-gx)+abs(sy-gy) <= t_g_sgdir)
                        {
                            rep(d, 4)
                            {
                                int nx = sx, ny = sy;
                                while(1)
                                {
                                    nx += dx[d];
                                    ny += dy[d];
                                    if (g[nx][ny] == 'T')
                                    {
                                        tmp_target_trees.emplace(nx, ny);
                                        break;
                                    }
                                }
                            }
                        }
                        auto [ex, ey] = tmp_stk.back(); // 迂回道が始まる点.
                        int sx = -1, sy = -1, gx = -1, gy = -1; 
                        { // sx, sy, gx, gyの決定
                            rep(d, 4)
                            {
                                int nx = ex + dx[d];
                                int ny = ey + dy[d];
                                if (g[nx][ny] != 'T') continue;
                                if (sx == -1)
                                { 
                                    sx = nx;
                                    sy = ny; 
                                }
                                else 
                                { 
                                    gx = nx; 
                                    gy = ny; 
                                }
                            }
                        }
                        // sx, syからgx, gyへの最短路を探す. return {あればその道の最短の木の集合}
                        auto loop_check = [&]() -> set<pii>
                        {
                            priority_queue<T3, vector<T3>, greater<T3>> pq;
                            map<pii, int> mp_dist;
                            map<pii, pii> mp_from;
                            pq.emplace(0, sx, sy);
                            mp_dist[{sx, sy}] = 0;
                            mp_from[{sx, sy}] = {-1, -1};
                            while(!pq.empty())
                            {
                                auto [c, x, y] = pq.top(); pq.pop();
                                if (mp_dist.count({x, y}) && mp_dist[{x, y}] < c) continue;
                                if (x == gx && y == gy)
                                {
                                    set<pii> res;
                                    while(!(x == sx && y == sy))
                                    {
                                        res.emplace(x, y);
                                        tie(x, y) = mp_from[{x, y}];
                                    }
                                    res.emplace(sx, sy);
                                    return res;
                                }
                                rep(d, 8)
                                {
                                    int nx = x + dxx[d];
                                    int ny = y + dyy[d];
                                    if (tmp_target_trees.count({nx, ny}))
                                    {
                                        if (mp_dist.count({nx, ny}) == 0 
                                            || mp_dist[{nx, ny}] > mp_dist[{x, y}] + (d&1 ? 1 : 2))
                                        {
                                            mp_dist[{nx, ny}] = mp_dist[{x, y}] + (d&1 ? 1 : 2);
                                            mp_from[{nx, ny}] = {x, y};
                                            pq.emplace(mp_dist[{nx, ny}], nx, ny);
                                        }
                                    }
                                }
                            }
                            return {};
                        };

                        vector<pii> stk_road; // 迂回路の管理
                        set<pii> used_road; // 迂回路のvisitedの管理
                        map<pii, int> around_tree; // そのマスから視認できる木を管理するためのmap
                        auto dfs = [&](auto f, int x, int y) -> void
                        {
                            auto loop_path = loop_check();
                            if (loop_path.size()) 
                            {
                                if (res.empty() || stk_road.size() < res.size())
                                {
                                    
                                    res = stk_road;
                                    target_trees = loop_path;
                                }
                                return;
                            }
                            // 4方向に探索
                            rep(d, 4)
                            {
                                int nx = x + dx[d];
                                int ny = y + dy[d];
                                // すでに通ったマスでなく, 道かどうかの確認.
                                if (used_road.count({nx, ny})) continue;
                                if (road.count({nx, ny}))
                                {
                                    // add
                                    vector<pii> tmp_add;
                                    rep(nd, 4)
                                    {
                                        int tx = nx, ty = ny;
                                        while (1)
                                        {
                                            tx += dx[nd]; ty += dy[nd];
                                            if (g[tx][ty] == 'T') 
                                            { 
                                                around_tree[{tx, ty}]++; 
                                                tmp_add.emplace_back(tx, ty);
                                                if (around_tree[{tx, ty}] == 1)
                                                {
                                                    tmp_target_trees.emplace(tx, ty);
                                                }
                                                break; 
                                            }
                                        }
                                    }
                                    stk_road.emplace_back(nx, ny);
                                    used_road.emplace(nx, ny);
                                    f(f, nx, ny);
                                    // revert
                                    for (auto [tx, ty]: tmp_add)
                                    {
                                        around_tree[{tx, ty}]--;
                                        if (around_tree[{tx, ty}] == 0)
                                        {
                                            tmp_target_trees.erase({tx, ty});
                                        }
                                    }
                                    stk_road.pop_back();
                                    used_road.erase({nx, ny});
                                }
                            }
                        };
                        for (auto [x, y]: tmp_target_trees)
                        {
                            around_tree[{x, y}]++;
                        }
                        used_road.emplace(ex, ey);
                        if (1)
                        { // TEST
                            // for (auto [tx, ty]: road)
                            // {
                            //     g[tx][ty] = 'R';
                            // }
                            // for (auto [tx, ty]: tmp_target_trees)
                            // {
                            //     g[tx][ty] = 'V';
                            // }
                            // printvv(g);
                            // for (auto [tx, ty]: tmp_target_trees)
                            // {
                            //     g[tx][ty] = 'T';
                            // }
                            // for (auto [tx, ty]: road)
                            // {
                            //     g[tx][ty] = '.';
                            // }
                        }
                        dfs(dfs, ex, ey);
                        used_road.erase({ex, ey});
                    }
                    revert_path(trees);
                    return {res, target_trees, not_visible_cnt};
                };

                vector<pii> path;
                set<pii> target_trees;
                auto dfs = [&](auto f, int x, int y, int p1x, int p1y, 
                    int d1, int d2, bool ok=true, int curve=1, int straight=0) -> bool
                {
                    if (t_g_straight < straight) return false;
                    if (t_g_dir <= curve) 
                    {
                        auto [path_add, tmp_target_trees, not_visible_cnt] = return_alignment();
                        if (path_add.size() == 0) return false;
                        auto path_all = tmp_stk;
                        path_all.insert(path_all.end(), rng(path_add));
                        ll sg = reachable(path_all, false);
                        if (sg == -INF) return false;
                        score = {-(int)path_all.size(), sg};
                        path = path_all;
                        target_trees = tmp_target_trees;
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
                            { // 次の空マスの周囲にすでに通ったマスがないかの確認.
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
                {
                    tmp_stk.clear();
                    int x = gx, y = gy;
                    tmp_stk.emplace_back(x, y);
                    {
                        bool ok = true;
                        rep(i, tol)
                        {
                            x += dx[d2]; y += dy[d2];
                            tmp_stk.emplace_back(x, y);
                            if (g[x][y] == 'T')
                            {
                                ok = false;
                                break;
                            }
                        }
                        if (!ok) continue;
                    }
                    if (d2%2 == 0) d1++;
                    x += dx[d1]; y += dy[d1];
                    tmp_stk.emplace_back(x, y);
                    if (g[x][y] == 'T') continue;
                    if (dfs(dfs, x, y, x-dx[d1], y-dy[d1], d1, d2))
                    {
                        if (best_score < score)
                        {
                            best_score = score;
                            best_path = path;
                            best_target_trees = target_trees;
                            found = true;
                            { // TEST
                                // auto trees = apply_path(best_path);
                                // for (auto [tx, ty]: target_trees)
                                // {   
                                //     g[tx][ty] = 'V';
                                // }
                                // printvv(g);
                                // for (auto [tx, ty]: target_trees)
                                // {   
                                //     g[tx][ty] = 'T';
                                // }
                                // revert_path(trees);
                            }
                        }
                    }
                }
            }
        }
        return found;
    };
    for (int i = 1; i <= t_tol; i++)
    {
        if (search(i)) break;
    }
    // 迂回する道まで見つからなかった場合, 単純にゴール周りを囲む道を探す.
    bool around_blank = false;
    // 上記でみつからなかった場合, 釣り鐘型ではなく単純にGからのジグザグ道を探す.
    if (best_path.size() == 0)
    {
        bool found = false;
        for (int d2: {2, 1, 3, 0})
        {
            for (int d1: {0, 2})
            {
                // start simulation
                vector<pii> tmp_stk, path;
                pll score = {-INF, -INF};
                auto dfs = [&](auto f, int x, int y, int p1x, int p1y, 
                    int d1, int d2, bool ok=true, int curve=1, int straight=0) -> bool
                {
                    if (t_gs_straight < straight) return false;
                    if (t_gs_dir <= curve) 
                    {
                        ll sg = reachable(tmp_stk, false);
                        if (sg == -INF) return false;
                        score = {sg, -straight};
                        path = tmp_stk;
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
                            { // 次の空マスの周囲にすでに通ったマスがないかの確認.
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
                {
                    tmp_stk.clear();
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
                        if (best_score < score)
                        {
                            best_score = score;
                            best_path = path;
                            found = true;
                        }
                    }
                }
            }
        }
        around_blank = found;
    }
    { // apply best_path to grid g
        // best_pathの周囲に木を置く.
        set<pii> used_tree; // best_pathを辿る際に視認した木の集合
        for (auto [x, y]: best_path)
        {
            if (g[x][y] != 'G') g[x][y] = 'R';
            else g[x][y] = (around_blank ? 'K' : 'G');
        }
        for (int i = 0; i+1 < best_path.size(); i++)
        {
            auto [x, y] = best_path[i];
            rep(d, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (g[nx][ny] == 'T') used_tree.emplace(nx, ny);
                if (g[nx][ny] == '.') 
                {
                    bool flag = true;
                    int tx = nx, ty = ny;
                    while(1)
                    {
                        tx += dx[d]; ty += dy[d];
                        if (g[tx][ty] == 'T')
                        { // **視認できる木がある場合は遮らないようにする.
                            if (best_target_trees.count({tx, ty}) != 0)
                            {
                                flag = false;
                            }
                            break;
                        }
                    }
                    if (flag)
                    {
                        g[nx][ny] = 'T';
                        res.emplace_back(nx, ny);
                        used_tree.emplace(nx, ny);
                        rows[nx].insert(ny);
                        cols[ny].insert(nx);
                    }
                }
            }
        }
        if (around_blank) 
        { // 単純なジグザグ道の場合, 木の周囲を空きマスにする.
            for (auto [x, y]: used_tree) // 木の周囲は空きマスに
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
        else // 釣り鐘型の道の場合, 一本道に入るマスから視認できる空きマスを全てBにする.
        { 
            vector<pii> tmp_target_blank;
            if (best_path.size()) tmp_target_blank.emplace_back(best_path.back());
            if (abs(sx-gx)+abs(sy-gy) <= t_g_sgdir)
            {
                tmp_target_blank.emplace_back(sx, sy);
            }
            for (auto [x, y]: tmp_target_blank)
            {
                rep(d, 4)
                {
                    int nx = x, ny = y;
                    vector<pii> tmp_blank;
                    while(1)
                    {
                        nx += dx[d];
                        ny += dy[d];
                        if (g[nx][ny] != '.')
                        {
                            if (best_target_trees.count({nx, ny}))
                            {
                                for (auto [bx, by]: tmp_blank)
                                {
                                    if (g[bx][by] == '.')
                                    {
                                        g[bx][by] = 'B';
                                    }
                                }
                            }
                            break;
                        }
                        tmp_blank.emplace_back(nx, ny);
                    }
                }
            }
        }
    }
    return res;
}

double vis_score()
{

    double res = 0, cnt = 0;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (tmp_dist[i][j] == INF) continue;
        cnt += 1.0;
        int r = 0, c = 0;
        if (rows[i].size())
        {
            auto it = rows[i].lower_bound(j);
            if (it != rows[i].begin())
            {
                r += abs(j - *prev(it) - 1);
            }
            if (it != rows[i].end())
            {
                r += abs(*it - 1 - j);
            }
        }
        if (cols[j].size())
        {
            auto it = cols[j].lower_bound(i);
            if (it != cols[j].begin())
            {
                c += abs(i - *prev(it) - 1);
            }
            if (it != cols[j].end())
            {
                c += abs(*it - 1 - i);
            }
        }
        res += r + c + 1;
    }
    res /= cnt;
    return res;
}

// 盤面全体の入り組み具合を評価する関数
// {曲がり角の数/距離の総和, 到達可能なマスの数の割合, 行き止まりの割合, SからGまでの距離, ループの数, Gまで到達可能か}
tuple<double, double, double, double, double, bool> board_score()
{
    queue<pii> empty_q; q.swap(empty_q);
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

    double turns_sum = 0, dists = 0, f4 = 0, e = 0, v = 0, v_all = 0, v_corner = 0;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (g[i][j] != 'T') v_all++; // 全空きマス
        if (tmp_dist[i][j] != INF) 
        {
            turns_sum += turns[i][j];
            dists += tmp_dist[i][j];
            v++;
            bool loop = true;
            rep(d1, 2)rep(d2, 2)
            {
                if (tmp_dist[i+d1][j+d2] == INF) loop = false;
            }
            if (loop) f4++;
            int around_tree_cnt = 0;
            rep(d, 4)
            {
                int ni = i + dx[d];
                int nj = j + dy[d];
                if (g[ni][nj] == 'T') around_tree_cnt++;
                if (tmp_dist[ni][nj] != INF) e++;
            }
            if (around_tree_cnt == 3) 
            {
                v_corner++;
            }
        }
    }
    bool enable = tmp_dist[sx][sy] != INF && dists != 0 && v_all != 0 && v != 0;
    if (!enable) return {-INF, -INF, -INF, -INF, 0, false};
    else return {turns_sum/dists, v/v_all, v_corner/v, tmp_dist[sx][sy], e/2-v+1-f4, true};
}

// 盤面を評価する関数
double calculate()
{
    double res = 0;
    auto [d_curve, d_spread, d_corner, d_sg, d_loop, reachable] = board_score();
    double d_vis = vis_score();
    { // 盤面の入り組み具合を評価(**先にdistを計算しておくこと)
      // 到達可能なマスの数を評価  
        if (!reachable) return -INF;
        res += (d_curve - t_curve_rate)*t_curve;
        res -= abs(d_spread - t_spread_rate)*t_spread;
        res += (d_corner - t_corner_rate)*t_corner;
        res += d_sg * t_sg;
        res += d_loop * t_loop;
        res -= abs((double)global_trees/(n*n)-t_tree_density)*t_tree;
        res -= (d_vis - t_vis_rate)*t_vis;
    }
    { // debug
        debug_tmp_curve_rate = d_curve;
        debug_tmp_spread_rate = d_spread;
        debug_tmp_corner_rate = d_corner;
        debug_tmp_sg = d_sg;
        debug_tmp_loop = d_loop;
        debug_tmp_vis = d_vis;
    }
    return res;
}

// ランダムに近傍操作を繰り返して、評価関数の値を最適化する
vector<pii> alignment()
{
    auto start = chrono::steady_clock::now();
    auto res_g = goal_alignment();
    vector<pii> res, cand_add;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (g[i][j] == '.' && abs(i-gx)+abs(j-gy) > t_g_edir) cand_add.emplace_back(i, j);
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
        // add 1 (見通しが悪くなるように配置)
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
        // add 2 (ランダムに配置)
        else if (op_kind == 1 && cand_add.size())
        {
            shuffle(rng(cand_add), gen);
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
        // add 3 一直線を分断して配置
        else if (op_kind == 2 && cand_add.size())
        {
            shuffle(rng(cand_add), gen);
            vector<pii> tmp_cand_add;
            for (auto [x, y]: cand_add)
            {
                if (tmp_cand_add.size() >= op_times) break;
                bool flag = false;
                rep(d, 2)
                {
                    int nx0 = x+dx[d], ny0 = y+dy[d];
                    int nx1 = x+dx[d+1], ny1 = y+dy[d+1];
                    int nx2 = x+dx[d+2], ny2 = y+dy[d+2];
                    int nx3 = x+dx[(d+3)%4], ny3 = y+dy[(d+3)%4];
                    if (g[nx0][ny0] == 'T' && g[nx2][ny2] == 'T' 
                        && g[nx1][ny1] == '.' && g[nx3][ny3] == '.')
                    {
                        flag = true;
                        break;
                    }
                }
                if (flag) tmp_cand_add.emplace_back(x, y);
            }
            for (auto [x, y]: tmp_cand_add)
            {
                g[x][y] = 'T';
                res.emplace_back(x, y);
                tmp_add.emplace_back(x, y);
                rows[x].emplace(y);
                cols[y].emplace(x);
                erase_one(cand_add, pii{x, y});
            }
            global_trees += tmp_add.size();
        }
        // del 1
        else if (op_kind == 3 && res.size())
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
        else if (op_kind == 4 && res.size())
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
        else if (op_kind == 5 && res.size())
        {
            shuffle(rng(res), gen);
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
        // move
        else if (op_kind == 6 && res.size())
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
                        if (abs(nx-gx)+abs(ny-gy) <= t_g_edir) continue;
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
            debug_curve_rate = debug_tmp_curve_rate;
            debug_spread_rate = debug_tmp_spread_rate;
            debug_corner_rate = debug_tmp_corner_rate;
            debug_sg = debug_tmp_sg;
            debug_loop = debug_tmp_loop;
            debug_vis = debug_tmp_vis; 
            best_score = score;
            debug_update++;
            continue;
        }

        // ------------------------------ revert ------------------------------
        if (op_kind == 0 || op_kind == 1 || op_kind == 2)
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
        else if (op_kind == 3 || op_kind == 4 || op_kind == 5)
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
        else if (op_kind == 6)
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

    rep(i, n+2)rep(j, n+2)
    {
        if (g[i][j] == 'T')
        {
            rows[i].insert(j);
            cols[j].insert(i);
        }
    }
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
        cout << Sim::sim_get_steps() << endl;
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
        // cerr << "------------main information------------" << endl;
        cerr << "debug_all_times: " << debug_all_times << endl;
        // cerr << "debug_update: " << debug_update << endl;
        // cerr << "debug_elapsed: " << debug_elapsed << endl;
        // cerr << "debug_score: " << debug_score << endl;
        // cerr << "------------detailed information------------" << endl;
        // rep(i, debug_times.size())
        // {
        //     cerr << "debug_times[" << i << "]: " << debug_times[i] << endl;
        // }
        // rep(i, debug_op_times.size())
        // {
        //     cerr << "debug_op_times[op = " << i << "]: " << debug_op_times[i] << endl;
        // }
        cerr << "debug_trees_rate: " << debug_trees_rate << " / " << t_tree_density << endl;
        cerr << "debug_curve_rate: " << debug_curve_rate << " / " << t_curve_rate << endl;
        cerr << "debug_spread_rate: " << debug_spread_rate << " / " << t_spread_rate << endl;
        cerr << "debug_corner_rate: " << debug_corner_rate << " / " << t_corner_rate << endl;
        cerr << "debug_sg: " << debug_sg << endl;
        cerr << "debug_loop: " << debug_loop << endl;
        cerr << "debug_vis_rate: " << debug_vis << " / " << t_vis_rate << endl;
        cerr << "system_score (n=" << n << ") : " << Sim::sim_get_steps() << endl;
    }
    return 0;
}