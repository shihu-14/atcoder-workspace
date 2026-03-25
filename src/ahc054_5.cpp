/*
山登り法で最適化する

-近傍操作
・木を1本追加
・木を1本削除
・木を1本移動

-評価の要素
・スタートから到達できるマスの数(多いほど良い)
・見通しの悪さ(悪いほど良い)
・盤面の入り組み具合(多いほど良い)
・(スタートからゴールまでの距離(到達不可能は場合は-∞))
・ゴールの周りの堅固さ(堅いほど良い)
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

// ====== LOCAL JUDGE ======
namespace Sim {
    // 内部状態
    static int N, TI, TJ;          // 花(目標)の座標（パディング）
    static int pi, pj;             // 現在位置（パディング）
    static bool finished=false;

    static vector<vector<char>> GG;   // 実盤面 '.' / 'T'（外周含む）
    static vector<vector<char>> seen; // 確認済み（外周 true）
    static vector<pair<int,int>> Q;   // 目的地候補（0-index, 内部で+1）
    static size_t qpos=0;

    static bool has_dest=false;    // 目的地の有無
    static int di=-1, dj=-1;       // 目的地（パディング）
    static long long steps = 0;          // ★総移動回数

    inline void sim_reset_steps(){ steps = 0; }     // ★
    inline long long sim_get_steps(){ return steps; } // ★

    // 暫定地図での通行可否：未確認は通行可、確認済みかつ木のみ遮断
    inline bool passable(int x,int y){
        return !(seen[x][y] && GG[x][y]=='T');
    }

    // 暫定地図での到達可能判定（BFS）
    inline bool reachable(int sx,int sy,int tx,int ty){
        if (sx==tx && sy==ty) return true;
        vector<vector<char>> vis(N+2, vector<char>(N+2,0));
        queue<pair<int,int>> q; q.emplace(sx,sy); vis[sx][sy]=1;
        static const int DX[4]={-1,0,1,0}, DY[4]={0,1,0,-1};
        while(!q.empty()){
            auto [x,y]=q.front(); q.pop();
            for(int k=0;k<4;k++){
                int nx=x+DX[k], ny=y+DY[k];
                if (!passable(nx,ny)) continue;
                if (vis[nx][ny]) continue;
                if (nx==tx && ny==ty) return true;
                vis[nx][ny]=1;
                q.emplace(nx,ny);
            }
        }
        return false;
    }

    // 暫定地図での目的地まで距離場（逆向きBFS）
    inline void dist_to_dest(vector<vector<int>>& dist){
        const int INF = 1e9;
        dist.assign(N+2, vector<int>(N+2, INF));
        if (!has_dest) return;
        queue<pair<int,int>> q; dist[di][dj]=0; q.emplace(di,dj);
        static const int DX[4]={-1,0,1,0}, DY[4]={0,1,0,-1};
        while(!q.empty()){
            auto [x,y]=q.front(); q.pop();
            for(int k=0;k<4;k++){
                int nx=x+DX[k], ny=y+DY[k];
                if (!passable(nx,ny)) continue;
                if (dist[nx][ny]!=INF) continue;
                dist[nx][ny]=dist[x][y]+1;
                q.emplace(nx,ny);
            }
        }
    }

    // 現在地から4方向、最初の木まで（その木を含む）を確認済みに
    inline void reveal_here(vector<pair<int,int>>& newly){
        auto push = [&](int ix,int iy){
            if (1<=ix && ix<=N && 1<=iy && iy<=N){
                if (!seen[ix][iy]){ seen[ix][iy]=true; newly.emplace_back(ix-1,iy-1); }
            }else{
                seen[ix][iy]=true; // 外周は内部のみtrue
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
    }

    // 目的地更新（③〜⑤）
    inline void update_dest(){
        // ③ 花が見えたら花へ確定
        if (seen[TI][TJ]) { has_dest=true; di=TI; dj=TJ; return; }

        // ④ 現目的地が暫定地図で到達不能、または見えていて花でない → 未定化
        if (has_dest){
            if (!reachable(pi,pj,di,dj)) has_dest=false;
            else if (seen[di][dj] && !(di==TI && dj==TJ)) has_dest=false;
        }

        // ⑤ 未定なら Q から：未確認 かつ 暫定地図で到達可能（実盤面が木でも未確認なら可）
        if (!has_dest){
            for(; qpos<Q.size(); ++qpos){
                int qi=Q[qpos].first+1, qj=Q[qpos].second+1;
                if (qi<1||qi>N||qj<1||qj>N) continue;
                if (seen[qi][qj]) continue;                 // 未確認のみ
                if (!reachable(pi,pj,qi,qj)) continue;      // 暫定地図で到達可
                has_dest=true; di=qi; dj=qj; ++qpos; return;
            }
        }
    }
} // namespace Sim

// 初期化（外部の n,gx,gy,g を使用。gx,gy は+1済み、gは外周 'T' 付き）
void sim_init(const vector<pair<int,int>>& Q0){
    using namespace Sim;
    extern int n, gx, gy;
    extern vector<vector<char>> g;

    N=n; TI=gx; TJ=gy;
    GG.assign(N+2, vector<char>(N+2,'T'));
    seen.assign(N+2, vector<char>(N+2,false));
    for(int i=0;i<=N+1;i++){ seen[i][0]=seen[i][N+1]=true; }
    for(int j=0;j<=N+1;j++){ seen[0][j]=seen[N+1][j]=true; }
    for(int i=1;i<=N;i++)for(int j=1;j<=N;j++) GG[i][j] = (g[i][j]=='T' ? 'T' : '.'); // 'S','G' は通路扱い

    pi=1; pj=N/2+1;
    finished=false; has_dest=false; qpos=0; di=dj=-1;
    Q=Q0;
}

// 1ターン進める： (現在位置0-index, 新たに確認済みになったマス群0-index) を返す
pair<pair<int,int>, vector<pair<int,int>>> sim_step(){
    using namespace Sim;
    vector<pair<int,int>> newly;
    bool moved = false;
    if (finished || (pi==TI && pj==TJ)){ finished=true; return {{pi-1,pj-1}, newly}; }

    // ② 視認 → ③〜⑤ 目的地更新
    reveal_here(newly);
    update_dest();

    // ⑥ 距離場を用いて1歩移動（暫定地図の空きマスで移動判定）
    if (has_dest){
        vector<vector<int>> dist; dist_to_dest(dist);
        int cur = dist[pi][pj];
        if (cur < (int)1e9){
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
    if (moved) steps++;       // ★総移動回数をインクリメント
    if (pi==TI && pj==TJ) finished=true;
    return {{pi-1,pj-1}, newly};
}

// 盤面に木を反映（パディング座標で渡す）
void sim_apply_trees(const vector<pair<int,int>>& v){
    using namespace Sim;
    for (auto [i,j] : v) GG[i][j] = 'T';
}

// 到達判定
bool sim_finished(){
    using namespace Sim;
    return finished;
}
// ====== END LOCAL JUDGE ======



const double TIME_LIMIT = 1900; // ms
int n, gx, gy;
vector<vector<char>> g, g_origin;
vector<vector<ll>> dist;

// parameter
ll a0 = 40; // 近傍操作で木を追加する確率(%)
ll a1 = 20; // 近傍操作で木を削除する確率(%)

double t_vis = 100; // 見通しの悪さを評価する
double t_curve = 1000; // 盤面の入り組み具合を評価する
double t_spread = 1; // 到達可能なますの数を評価
double t_goal1 = 100; // ゴールの周りの半径1周の堅固さ
double t_goal5 = 0; // ゴールの周りの半径5周の堅固さ
int t_dir = 10;
double T0 = 5e2, Tend = 1e-4; // SAの温度パラメータ


// debug
int debug_times = 0;
int debug_first_times = 0;
int debug_middle_times = 0;
int debug_last_times = 0;
int debug_update = 0;
int debug_first_accept = 0;
int debug_middle_accept = 0;
int debug_last_accept = 0;
int debug_elapsed = 0;
double debug_score = 0;
vector<vector<char>> debug_best_g;

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
pdd curve_score(int sx=1, int sy=n/2+1)
{
    queue<pii> q;
    q.emplace(sx, sy);
    dist.assign(n+2, vector<ll>(n+2, INF));
    dist[sx][sy] = 0;
    vector<vector<pii>> from(n+2, vector<pii>(n+2, {-1, -1}));
    vector<vector<int>> turns(n+2, vector<int>(n+2, 0));
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
    res /= dists;
    if (dist[gx][gy] == INF) res = -INF;
    return {res, cnt};
}


// 盤面を評価する関数
double calculate()
{
    double res = 0;
    { // 盤面の入り組み具合を評価(**先にdistを計算しておくこと)
      // 到達可能なマスの数を評価  
        auto [d_curve, d_spread] = curve_score();
        res += d_curve * t_curve;
        res += d_spread * t_spread;
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
        res += goal_cnt * t_goal1;
    }
    { // ゴールの周りの半径5周の堅固さ
        int goal_cnt = 0;
        for (int i = -t_dir; i <= t_dir; i++)
        {
            for (int j = -t_dir; j <= t_dir; j++)
            {
                int ni = gx + i;
                int nj = gy + j;
                if (ni < 0 || ni >= n+2 || nj < 0 || nj >= n+2) continue;
                if (g[ni][nj] == 'T') goal_cnt++;
            }
        }
        res += goal_cnt * t_goal5;
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
    // for output
    double best_score = calculate();
    vector<pii> best_res = res;
    // rand
    random_device rd;
    mt19937 gen(rd());
    std::uniform_int_distribution<> op_dist(1, 100);
    // SA
    double cur_score = best_score;

    while(1)
    {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start).count();
        debug_elapsed = elapsed;
        if (elapsed > TIME_LIMIT) break;

        // tmp for revert
        int tmp_x = -1, tmp_y = -1;
        int tmp_nx = -1, tmp_ny = -1;
        int tmp_id1 = -1, tmp_id2 = -1;
        // operation
        int op = -1;
        int op_d = op_dist(gen);
        if (op_d <= a0) op = 0;
        else if (op_d <= a0+a1) op = 1;
        else op = 2;

        // ----------------近傍操作-----------------
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
        // move
        else if (op == 2 && cand_del.size() && cand_add.size())
        {
            std::uniform_int_distribution<> dist1(0, (int)cand_del.size()-1);
            std::uniform_int_distribution<> dist2(0, (int)cand_add.size()-1);
            int id1 = dist1(gen);
            int id2 = dist2(gen);
            tie(tmp_x, tmp_y) = cand_del[id1];
            tie(tmp_nx, tmp_ny) = cand_add[id2];
            tmp_id1 = id1;
            tmp_id2 = id2;
            g[tmp_x][tmp_y] = '.';
            g[tmp_nx][tmp_ny] = 'T';
            cand_del[id1] = {tmp_nx, tmp_ny};
            cand_add[id2] = {tmp_x, tmp_y};
            res.erase(find(rng(res), pii(tmp_x, tmp_y)));
            res.emplace_back(tmp_nx, tmp_ny);
        }
        else
        {
            continue;
        }

        // ----------------------- SA -----------------------
        // evaluate
        auto score = calculate();
        double ratio = min(1.0, (double)elapsed/TIME_LIMIT);    
        debug_times++;
        if (ratio < 0.2) debug_first_times++;
        else if (ratio < 0.5) debug_middle_times++;
        else debug_last_times++;
        double T = T0 * pow(Tend / T0, ratio); 
        double delta = score - cur_score;
        static uniform_real_distribution<double> ur01(0.0, 1.0);
        bool accept = (delta >= 0) || (exp(delta / T) > ur01(gen));
        if (accept) // update
        {
            cur_score = score;
            if (cur_score > best_score)
            {
                best_score = cur_score;
                best_res = res;
                debug_update++;
            }
            else if (delta < 0)
            {
                if (ratio < 0.2) debug_first_accept++;
                else if (ratio < 0.5) debug_middle_accept++;
                else debug_last_accept++;
            }
            continue;
        }

        // ------------------------------ revert ------------------------------
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
        else if (op == 2)
        {
            g[tmp_x][tmp_y] = 'T';
            g[tmp_nx][tmp_ny] = '.';
            cand_del[tmp_id1] = {tmp_x, tmp_y};
            cand_add[tmp_id2] = {tmp_nx, tmp_ny};
            res.pop_back();
            res.emplace_back(tmp_x, tmp_y);
        }
    }
    
    debug_score = best_score;
    g = g_origin;
    debug_best_g = g_origin;
    for (auto [i, j] : best_res) 
    {
        g[i][j] = 'T';
        debug_best_g[i][j] = 'T';
    }
    return best_res;
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
    g_origin = g;
    if (IS_LOCAL_JUDGE)
    { // for local test (sim init)
        vector<pii> Q;
        rep(i, n*n-1)
        {
            int qi, qj; cin >> qi >> qj;
            Q.emplace_back(qi, qj);
        }
        sim_init(Q);
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
        }
        // for local test
        if (IS_LOCAL_JUDGE)
        {
            { // for one shot output
                // cout << res.size() << " ";
                // for (auto [a, b] : res) cout << a-1 << " " << b-1 << " ";
                // cout << endl;
            }
            sim_apply_trees(res); 
            sim_step();
            if (sim_finished()) return;
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
            if (sim_finished()) return;
            // for one shot output
            {
                // cout << 0 << endl; 
            }
            sim_step();   
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
        cerr << "debug_times: " << debug_times << endl;
        cerr << "debug_update: " << debug_update << endl;
        cerr << "debug_elapsed: " << debug_elapsed << endl;
        // cerr << "debug_score: " << debug_score << endl;
        cerr << "debug_first_accept_rate: " << debug_first_accept * 100.0 / debug_first_times << "%" << endl;
        cerr << "debug_middle_accept_rate: " << debug_middle_accept * 100.0 / debug_middle_times << "%" << endl;
        cerr << "debug_last_accept_rate: " << debug_last_accept * 100.0 / debug_last_times << "%" << endl;   
        cerr << "system_score: " << Sim::sim_get_steps() << endl;
        // rep(i, n+2)
        // {
        //     rep(j, n+2) cerr << debug_best_g[i][j];
        //     cerr << endl;
        // }
        // printvv(debug_best_g);
    }
    return 0;
}