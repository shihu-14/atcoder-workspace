/*
山登り法で最適化する

-初期配置
・何もしない

-近傍操作
・木をn本追加
・木をn本削除
・木をn本移動

-評価の要素
・スタートから到達できるマスの数(多いほど良い)
・見通しの悪さ(悪いほど良い)
・盤面の入り組み具合(多いほど良い)
・(スタートからゴールまでの距離(到達不可能は場合は-∞))
・ゴールの半径d周りの堅固さ(堅いほど良い)
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
// ====== END LOCAL JUDGE (fast) ======

// global variables
int n, gx, gy;
vector<vector<char>> g;
vector<vector<ll>> dist;

// parameter
double a_limit = 1900;
vector<pll> a_r = {{90, 5}, {40, 30}, {30, 30}, {5, 10}}; // 時間帯ごとの操作の割合(%)
vector<double> a_l = {0.15, 0.4, 0.75}; // 時間帯の境界(%)
vector<ll> a_t = {18, 10, 8, 10}; // 各時間帯で一度に操作する回数

double t_vis = 4.541; // 見通しの悪さを評価する
double t_curve = 160000.413; // 盤面の入り組み具合を評価する
double t_spread = 0.045; // 到達可能なますの数を評価
double t_g_around = 4.541; // ゴールの周りの堅固さ
double t_g_curve = 160000.413; // ゴールの周りの半径t_dirの堅固さ
double t_g_spread = 0.020; // ゴールの周りの半径t_dirの堅固さ
ll t_g_dir = 10; // ゴール周りの堅固さを評価する際の範囲
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

// 盤面の入り組み具合を評価する関数
tuple<double, double, double, double, bool> curve_score(int dir, int sx=1, int sy=n/2+1)
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
    double res_g = 0, dists_g = 0, cnt_g = 0;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
        if (dist[i][j] != INF) 
        {
            if (dist[i][j] <= dir)
            {
                res_g += turns[i][j];
                dists_g += dist[i][j];
                cnt_g++;
            }
            else
            {
                res += turns[i][j];
                dists += dist[i][j];
                cnt++;
            }
        }
    }
    // if (dists) res /= dists;
    // if (dists_g) res_g /= dists_g;
    return {res, cnt, res_g, cnt_g, dist[sx][sy] != INF && dists != 0 && dists_g != 0};
}

// 盤面を評価する関数
double calculate()
{
    double res = 0;
    { // 盤面の入り組み具合を評価(**先にdistを計算しておくこと)
      // 到達可能なマスの数を評価  
        auto [d_curve, d_spread, g_curve, g_spread, reachable] = curve_score(t_g_dir);
        res += d_curve * t_curve;
        res += d_spread * t_spread;
        res += g_curve * t_g_curve;
        res += g_spread * t_g_spread;
        if (!reachable) return -LINF;
    }
    { // 見通しの悪さを評価
        res -= vis_score() * t_vis; 
    }
    { // ゴールの周りの半径1周の堅固さ
        int goal_cnt = 0;
        rep(d, 4)
        {
            int ni = gx + dx[d];
            int nj = gy + dy[d];
            if (g[ni][nj] == 'T') goal_cnt++;
        }
        res += goal_cnt * t_g_around;
    }
    return res;
}

// ランダムに近傍操作を繰り返して、評価関数の値を最適化する
vector<pii> alignment()
{
    auto start = chrono::steady_clock::now();
    vector<pii> res = initial_alignment(), cand_add;
    rep2(i, 1, n+1)rep2(j, 1, n+1)
    {
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
            rep(i, op_times)
            {
                auto [x, y] = res.back();
                g[x][y] = '.';
                tmp_p.emplace_back(x, y);
                cand_add.emplace_back(x, y);
                res.pop_back();
            }
            shuffle(cand_add.begin(), cand_add.end(), gen);
            rep(i, op_times)
            {
                auto [nx, ny] = cand_add.back();
                g[nx][ny] = 'T';
                res.emplace_back(nx, ny);
                tmp_np.emplace_back(nx, ny);
                cand_add.pop_back();
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
                if (ratio < 0.2) debug_first_accept++;
                else if (ratio < 0.5) debug_middle_accept++;
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
            rep(i, op_times)
            {
                auto [nx, ny] = tmp_np[i];
                g[nx][ny] = '.';
                res.pop_back();
                cand_add.emplace_back(nx, ny);
            }
            rep(i, op_times)
            {
                auto [x, y] = tmp_p[i];
                g[x][y] = 'T';
                res.emplace_back(x, y);
                cand_add.erase(find(rng(cand_add), pii{x, y}));
            }
        }
    }

    debug_score = best_score;
    return res;
}

void solve()
{
    cin >> n >> gx >> gy; gx++; gy++;
    g.resize(n+2, vector<char>(n+2, 'T'));
    dist.resize(n+2, vector<ll>(n+2, INF));
    rep(i, n)rep(j, n)
    {
        cin >> g[i+1][j+1];
    }
    g[1][n/2+1] = 'S'; // start
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