/*
山登り法で最適化する

-初期配置
・何もしない

-近傍操作
・木をn本追加
・木をn本削除
・木をn本移動

-評価の要素
・ギザギザ一本道の数(多いほど良い)
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

// parameter
double a_limit = 1900;
vector<pll> a_r = {{90, 5}, {40, 30}, {30, 30}, {5, 10}}; // 時間帯ごとの操作の割合%(add, del)
vector<double> a_l = {0.15, 0.4, 0.70}; // 時間帯の境界(%)
vector<ll> a_t = {18, 10, 8, 10}; // 各時間帯で一度に操作する回数

double t_vis = 100; // 見通しの悪さを評価する
double t_curve = 160.413; // 盤面の入り組み具合を評価する
double t_spread = 1; // 到達可能なますの数を評価
double t_sg = 1; // スタートからゴールまでの距離を評価する
ll t_g_edir = 6; // ゴール周りには木を置かない
ll t_g_dir = 8; // ゴール周りの堅固さを評価する際の範囲
ll t_g_straight = 3; // ゴール周りの堅固さを評価する際の直線距離
double T0 = 5e-12, Tend = 5e-12; // SAの温度パラメータ

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

// 初期配置
vector<pii> initial_alignment(){
    vector<pii> res;
    return res;
}

// ゴール周りの配置
vector<pii> goal_alignment(int dir, int guard){
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
    auto reachable = [&](vector<pii> path) -> int
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
        vector<vector<int>> vis(n+2, vector<int>(n+2, INF));
        vis[gx][gy] = 0;
        while (q.size())
        {
            auto [x, y] = q.front(); q.pop();
            if (x == sx && y == sy) return vis[x][y];
            rep(d, 4)
            {
                int nx = x + dx[d];
                int ny = y + dy[d];
                if (board[nx][ny] == 'T') continue;
                if (vis[nx][ny] != INF) continue;
                vis[nx][ny] = vis[x][y] + 1;
                q.emplace(nx, ny);
            }
        }
        return INF;
    };
    for (int d1: {2, 1, 3, 0})
    {
        for (int d2: {0, 2})
        {
            // start simulation
            vector<pii> res, stk;
            T3 score = {0, 0, 0};
            auto dfs = [&](auto f, int x, int y, int p1x, int p1y, 
                int d1, int d2, bool ok, int curve, int straight) -> void
            {
                if (straight > guard) return;
                if (curve >= dir) 
                {
                    int r = reachable(stk);
                    if (r == INF) return;
                    if (chmax(score, {curve, -straight, r}))
                    {
                        res = stk;
                    }
                    return;
                }
                vector<int> ds;
                if (ok) ds = {d2, d1};
                else ds = {(d1+1)%4, (d1+3)%4, d1};

                for (auto d: ds)
                {
                    int nx = x + dx[d];
                    int ny = y + dy[d];
                    if (g[nx][ny] == '.') 
                    {
                        stk.emplace_back(nx, ny);
                        int nd1 = calc_dir({nx-x, ny-y});
                        int nd2 = calc_dir({x-p1x, y-p1y});
                        bool nok = is_ok({dx[nd1], dy[nd1]}, {dx[nd2], dy[nd2]});
                        f(f, nx, ny, x, y, nd1, nd2, nok, curve+nok, straight+!nok);
                        stk.pop_back();
                    }
                    else
                    {
                        int r = reachable(stk);
                        if (r == INF) return;
                        if (chmax(score, {curve, -straight, r}))
                        {
                            res = stk;
                        }
                    }
                }
            };

            int x = gx, y = gy;
            stk.emplace_back(x, y);
            x += dx[d1]; y += dy[d1];
            stk.emplace_back(x, y);
            if (g[x][y] == 'T') continue;
            if (d1%2 == 0) d2++;
            x += dx[d2]; y += dy[d2];
            stk.emplace_back(x, y);
            if (g[x][y] == 'T') continue;
            dfs(dfs, x, y, x-dx[d2], y-dy[d2], d2, d1, true, 1, 0);

            if (score > best_score)
            {
                best_score = score;
                best_path = res;
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
            g[x][y] = 'A';
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

// 見通しの悪さを評価する関数
double vis_score()
{
    vector<vector<int>> rows(n+2);
    vector<vector<int>> cols(n+2);
    rep(i, n+2)rep(j, n+2)
    {
        if (g[i][j] == 'T')
        {
            rows[i].push_back(j);
            cols[j].push_back(i);
        }
    }

    double res = 0, cnt = 0;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (dist[i][j] == INF) continue;
        cnt += 1.0;
        int r = 0, c = 0;
        if (rows[i].size())
        {
            auto it = lower_bound(rng(rows[i]), j);
            if (it != rows[i].begin())
            {
                r += abs(j - *(it-1) - 1);
            }
            if (it != rows[i].end())
            {
                r += abs(*it - 1 - j);
            }
        }
        if (cols[j].size())
        {
            auto it = lower_bound(rng(cols[j]), i);
            if (it != cols[j].begin())
            {
                c += abs(i - *(it-1) - 1);
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
tuple<double, double, double, bool> curve_score()
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
            if(g[nx][ny] == 'T') continue;
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
    double res = 0, dists = 0, cnt = 0;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (dist[i][j] != INF) 
        {
            res += turns[i][j];
            dists += dist[i][j];
            cnt++;
        }
    }
    if (dists) res /= dists;
    return {res, cnt, dist[sx][sy], dist[sx][sy] != INF && dists != 0};
}

// 盤面を評価する関数
double calculate()
{
    double res = 0;
    { // 盤面の入り組み具合を評価(**先にdistを計算しておくこと)
      // 到達可能なマスの数を評価  
        auto [d_curve, d_spread, d_sg, reachable] = curve_score();
        res += d_curve * t_curve;
        res += d_spread * t_spread;
        res += d_sg * t_sg;
        if (!reachable) return -LINF;
    }
    { // 見通しの悪さを評価
        res -= vis_score() * t_vis; 
    }
    return res;
}

// ランダムに近傍操作を繰り返して、評価関数の値を最適化する
vector<pii> alignment()
{
    auto start = chrono::steady_clock::now();
    auto res_g = goal_alignment(t_g_dir, t_g_straight);
    vector<pii> res = initial_alignment(), cand_add;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (abs(i-gx) + abs(j-gy) <= t_g_edir) continue;
        if (g[i][j] == '.') cand_add.emplace_back(i, j);
    }
    // for output
    double best_score = calculate();
    // rand
    random_device rd;
    mt19937 gen(rd());
    std::uniform_int_distribution<> op_dist(1, 100);
    // SA

    while(1)
    {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start).count();
        debug_elapsed = elapsed;
        if (elapsed > a_limit) break;
        double ratio = min(1.0, (double)elapsed/a_limit);   

        // tmp for revert
        vector<pii> tmp_p, tmp_np;

        // operation
        int op_kind = -1, op_times = 0;
        int op_r0 = 0, op_r1 = 0;
        if (ratio < a_l[0])
        {
            std::uniform_int_distribution<> t_dist(1, a_t[0]);
            op_times = t_dist(gen);
            op_r0 = a_r[0].fi; op_r1 = a_r[0].se;
        }
        else if (ratio < a_l[1])
        {
            std::uniform_int_distribution<> t_dist(1, a_t[1]);
            op_times = t_dist(gen);
            op_r0 = a_r[1].fi; op_r1 = a_r[1].se;
        }
        else if (ratio < a_l[2])
        {
            std::uniform_int_distribution<> t_dist(1, a_t[2]);
            op_times = t_dist(gen);
            op_r0 = a_r[2].fi; op_r1 = a_r[2].se;
        }
        else
        {
            std::uniform_int_distribution<> t_dist(1, a_t[3]);
            op_times = t_dist(gen);
            op_r0 = a_r[3].fi; op_r1 = a_r[3].se;
        }

        int rand_op = op_dist(gen);
        if (rand_op <= op_r0) op_kind = 0;
        else if (rand_op <= op_r0+op_r1) op_kind = 1;
        else op_kind = 2;

        // ----------------近傍操作-----------------
        // add
        if (op_kind == 0 && cand_add.size() >= op_times)
        {
            shuffle(cand_add.begin(), cand_add.end(), gen);
            rep(i, op_times)
            {
                auto [x, y] = cand_add.back();
                g[x][y] = 'T';
                res.emplace_back(x, y);
                tmp_p.emplace_back(x, y);
                cand_add.pop_back();
            }
        }   
        // del
        else if (op_kind == 1 && res.size() >= op_times)
        {
            shuffle(res.begin(), res.end(), gen);
            rep(i, op_times)
            {
                auto [x, y] = res.back();
                g[x][y] = '.';
                tmp_p.emplace_back(x, y);
                cand_add.emplace_back(x, y);
                res.pop_back();
            }
        }
        // move
        else if (op_kind == 2 && res.size() >= op_times)
        {
            shuffle(res.begin(), res.end(), gen);
            {
                set<pii> st;
                rep(i, res.size())
                {
                    if (tmp_p.size() >= op_times) break;
                    auto [x, y] = res[i];
                    rep(d, 4)
                    {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if (abs(nx - gx) + abs(ny - gy) <= t_g_edir) continue;
                        if (g[nx][ny] == '.' && !st.count({nx, ny}))
                        {
                            tmp_p.emplace_back(x, y);
                            tmp_np.emplace_back(nx, ny);
                            st.emplace(nx, ny);
                            break;
                        }
                    }
                }
            }
            // del
            for (auto [x, y]: tmp_p)
            {
                g[x][y] = '.';
                res.erase(find(rng(res), pii{x, y}));
            }
            for (auto [nx, ny]: tmp_np)
            {
                g[nx][ny] = 'T';
                cand_add.erase(find(rng(cand_add), pii{nx, ny}));
            }
            // add
            for (auto [x, y]: tmp_p)
            {
                cand_add.emplace_back(x, y);
            }
            for (auto [nx, ny]: tmp_np)
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
        auto score = calculate();
         
        debug_times++;
        if (ratio < 0.2) debug_first_times++;
        else if (ratio < 0.5) debug_middle_times++;
        else debug_last_times++;

        double T = T0 * pow(Tend / T0, ratio); 
        double delta = score - best_score;
        static uniform_real_distribution<double> ur01(0.0, 1.0);
        bool accept = (delta > 0) || (exp(delta / T) > ur01(gen));
        if (accept) // update
        {
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
            rep(i, op_times)
            {
                auto [x, y] = tmp_p[i];
                g[x][y] = '.';
                res.pop_back();
                cand_add.emplace_back(x, y);
            }
        }
        else if (op_kind == 1)
        {
            rep(i, op_times)
            {
                auto [x, y] = tmp_p[i];
                g[x][y] = 'T';
                res.emplace_back(x, y);
                cand_add.pop_back();
            }
        }
        else if (op_kind == 2)
        {
            // del
            for (auto [x, y]: tmp_p)
            {
                g[x][y] = 'T';
                cand_add.pop_back();
            }
            for (auto [nx, ny]: tmp_np)
            {
                g[nx][ny] = '.';
                res.pop_back();
            }
            // add
            for (auto [x, y]: tmp_p)
            {
                res.emplace_back(x, y);
            }
            for (auto [nx, ny]: tmp_np)
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
    dist.resize(n+2, vector<ll>(n+2, INF));
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