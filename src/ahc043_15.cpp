// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
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
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
using T6 = tuple<int, int, int, int, int, int>;
using T7 = tuple<int, int, int, int, int, int, int>;
using T8 = tuple<int, int, int, int, int, int, int, int>;

/*

山登り法

*/

unsigned int randxor(){
    static unsigned int x=123456789,y=362436069,z=521288629,w=88675123;
    unsigned int t;
    t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) );
}
// ある2つの位置に駅を置いたときに得られる総利益
vector<vector<int>> make_earnlist_with_station(vector<pii> &start, vector<pii> &goal, int n){
    vector<vector<int>> dist(n*n, vector<int>(n*n));
    rep(i, start.size()){
        auto [x1, y1] = start[i];
        auto [x2, y2] = goal[i];
        rep2(dx1, -2, 3)rep2(dy1, -2, 3){
            rep2(dx2, -2, 3)rep2(dy2, -2, 3){
                int nx1 = x1+dx1, ny1 = y1+dy1;
                int nx2 = x2+dx2, ny2 = y2+dy2;
                if (abs(nx1-x1)+abs(ny1-y1) > 2) continue;
                if (abs(nx2-x2)+abs(ny2-y2) > 2) continue;
                if (nx1<0 || nx1>=n || ny1<0 || ny1>=n) continue;
                if (nx2<0 || nx2>=n || ny2<0 || ny2>=n) continue;
                dist[nx1*n+ny1][nx2*n+ny2] += abs(x1-x2)+abs(y1-y2);
            }
        }
    }
    return dist;
}

T6 calc_balance(vector<pii> &stations, vector<vector<int>> &earnlist, int turn, int n, int T, 
                                            int pos_x, int pos_y, int now_per_earn, int now_money){
    int min_dist = INF;
    int near_x = -1, near_y = -1;
    for (auto [sta_x, sta_y]: stations){ // pos_x, pos_yから一番近い駅near_x, near_yを求める。
        int d = abs(sta_x-pos_x)+abs(sta_y-pos_y);
        if (d < min_dist){
            min_dist = d;
            near_x = sta_x, near_y = sta_y;
        }
    }
    int turn_for_earn = ((min_dist-1)*100-(now_money-5000)+now_per_earn-1)/now_per_earn; // 橋の建設費用を稼ぐ。
    if (turn_for_earn < 0) turn_for_earn = 0;
    if (turn+min_dist+turn_for_earn > T) return {INF, 0, -1, -1, -1, -1};
    int earn = now_per_earn*(T-turn), new_earn = 0;
    new_earn -= 5000+(min_dist-1)*100;
    int tmp_sum = now_per_earn;
    for (auto [sta_x, sta_y]: stations){
        tmp_sum += earnlist[pos_x*n+pos_y][sta_x*n+sta_y];
        tmp_sum += earnlist[sta_x*n+sta_y][pos_x*n+pos_y];
    }
    new_earn += tmp_sum*(T-turn-min_dist-turn_for_earn);
    return {earn, new_earn, near_x, near_y, tmp_sum, turn_for_earn};
}


T7 decide_next_station(vector<pii> &stations, vector<vector<int>> &earnlist, int turn, int n, int T, int now_per_earn, int now_money){
    int max_diff = 0;
    int set_x = -1, set_y = -1, near_x = -1, near_y = -1, dir = -1, per_earn = -1, turn_for_earn = -1;
    rep(x, n)rep(y, n){
        bool ok = true;
        for (auto [sta_x, sta_y]: stations){
            if (abs(sta_x-x)+abs(sta_y-y) <= 4){
                ok = false;
                break;
            }
        }
        if (ok == false) continue;
        auto [earn, new_earn, tmp_near_x, tmp_near_y, tmp_per_earn, tmp_turn_for_earn] = calc_balance(stations, earnlist, turn, n, T, x, y, now_per_earn, now_money);
        int diff = new_earn-earn;
        if (diff <= 0) continue;
        // int flag = able_to_construct(board, x, y, tmp_near_x, tmp_near_y);
        int flag = 1;
        if (max_diff < diff){
            max_diff = diff;
            set_x = x, set_y = y, near_x = tmp_near_x, near_y = tmp_near_y, 
                dir = flag, per_earn = tmp_per_earn, turn_for_earn = tmp_turn_for_earn;
        }
    }
    return {set_x, set_y, near_x, near_y, dir, per_earn, turn_for_earn};
}

vector<pii> able_to_construct(vector<vector<int>> &board, int set_x, int set_y){
    int n = board.size();
    vector<vector<int>> dist(n, vector<int>(n, INF)); dist[set_x][set_y] = 0;
    vector<vector<pii>> par(n, vector<pii>(n)); par[set_x][set_y] = {-1, -1};
    queue<pii> q; q.emplace(set_x, set_y);
    int min_dist = INF, near_x = -1, near_y = -1;
    if (board[set_x][set_y] == 1){
        return {{set_x, set_y}};
    }
    while(!q.empty()){
        auto [x, y] = q.front(); q.pop();
        if (min_dist <= dist[x][y]) break;
        rep(t, 4){
            int nx = x+dx[t], ny = y+dy[t];
            if (nx<0 || nx>=n || ny<0 || ny>=n) continue;
            if (dist[nx][ny] != INF) continue;
            if (board[nx][ny] == -1){
                dist[nx][ny] = dist[x][y]+1;
                par[nx][ny] = {x, y};
                if (chmin(min_dist, dist[nx][ny])){
                    near_x = nx, near_y = ny;
                }
                continue;
            }
            if (board[nx][ny] != 0) continue;
            dist[nx][ny] = dist[x][y]+1;
            par[nx][ny] = {x, y};
            q.emplace(nx, ny);
        }
    }
    vector<pii> op;
    int cur_x = near_x, cur_y = near_y;
    while(cur_x != -1 && cur_y != -1){
        op.emplace_back(cur_x, cur_y);
        tie(cur_x, cur_y) = par[cur_x][cur_y];
    }
    return op;
}

void await(vector<T3> &ans, int &now_money, int &now_turn, int turn, int per_earn){
    rep(t, turn){
        now_money += per_earn;
        now_turn++;
        ans.emplace_back(-1, -1, -1);
    }
}

void construct(vector<vector<int>> &board, vector<T3> &ans, vector<pii> &op, int &now_money, int &now_turn, int &per_earn, int new_per_earn){
    int n = board.size();
    if (now_money < 5000){ // 駅の建設のための資金稼ぎ
        if (per_earn == 0){
            cerr << "Money is not enough" << endl;
            return;
        }
        int q = max(0, (5000-now_money+per_earn-1)/per_earn);
        await(ans, now_money, now_turn, q, per_earn);
    }
    { // 駅の建設
        if (op.size() == 1) per_earn = new_per_earn;
        now_money -= 5000;
        now_money += per_earn;
        now_turn++;
        auto [sta_x, sta_y] = op.back();
        board[sta_x][sta_y] = -1;
        ans.emplace_back(0, sta_x, sta_y);
    }
    if (op.size() == 1) return;
    if (per_earn > 0){ // 橋の建設のための最低限の待ち状態
        int d = op.size()-2;
        int need = 100+(d-1)*(100-per_earn);
        if (per_earn >= 100) need = 100;
        int q = max(0, (need-now_money+per_earn-1)/per_earn);
        await(ans, now_money, now_turn, q, per_earn);
    }
    rrep2(i, op.size()-1, 1){ // 橋を建設する
        auto [px, py] = op[i+1];
        auto [x, y] = op[i];
        auto [nx, ny] = op[i-1];
        int dx1 = x - px, dy1 = y - py;
        int dx2 = nx - x, dy2 = ny - y;
        int track = -1; // 線路の種類 (1..6)
        // 1: 左右 (x 行が同じ => dx=0)
        if(dx1 == 0 && dx2 == 0) {
            track = 1;
        }
        // 2: 上下 (y 列が同じ => dy=0)
        else if(dy1 == 0 && dy2 == 0) {
            track = 2;
        }
        // 3: 左下をつなぐ (left->down or down->left)
        else if(
            (dx1== 0 && dy1==+1 && dx2==+1 && dy2== 0) ||  // 左→下
            (dx1==-1 && dy1== 0 && dx2== 0 && dy2==-1)     // 下→左
        ){
            track = 3;
        }
        // 4: 左上をつなぐ (left->up or up->left)
        else if(
            (dx1== 0 && dy1==1 && dx2== -1 && dy2== 0) ||  // 左→上
            (dx1== 1 && dy1== 0 && dx2== 0 && dy2==-1)     // 上→左
        ){
            track = 4;
        }
        // 5: 上右をつなぐ (up->right or right->up)
        else if(
            (dx1==0 && dy1==-1 && dx2==-1 && dy2==0) ||  // 上→右
            (dx1==1 && dy1==0 && dx2==0 && dy2==1)     // 右→上
        ){
            track = 5;
        }
        // 6: 右下をつなぐ (right->down or down->right)
        else if(
            (dx1== 0 && dy1==-1 && dx2==+1 && dy2== 0) ||  // 右→下
            (dx1==-1 && dy1== 0 && dx2== 0 && dy2==+1)     // 下→右
        ){
            track = 6;
        }
        board[x][y] = 1;
        ans.emplace_back(track, x, y);
    }
    { // renew now_money, now_turn, per_earn
        int d = op.size()-2;
        now_money -= 100*d;
        now_money += per_earn*(d-1)+new_per_earn;
        now_turn += d;
        per_earn = new_per_earn;
    }
}

pair<vector<pii>, pii> two_station(vector<vector<int>> &earnlist, int n, int K, int T){
    vector<pii> res;
    int max_money = K, max_p1 = -1, max_p2 = -1;
    rep(x1, n)rep(y1, n)rep(x2, n)rep(y2, n){
        int d = abs(x1-x2)+abs(y1-y2);
        if (d <= 4) continue;
        int money = K, turn = 0;
        turn += d+1;
        money -= (d-1)*100+2*5000;
        if (money < 0) continue;
        money += (earnlist[x1*n+y1][x2*n+y2]+earnlist[x2*n+y2][x1*n+y1])*(T-turn);
        res.emplace_back(x1*n+y1, x2*n+y2);
        if (money > max_money){
            max_money = money;
            max_p1 = x1*n+y1, max_p2 = x2*n+y2;
        }
    }
    return {res, {max_p1, max_p2}};
}

int calc_money(vector<pii> &que, vector<vector<int>> &earnlist, int n, int K, int T){
    int now_money = K-5000, now_turn = 1, per_earn = 0, m = que.size();
    rep2(i, 1, m){
        auto [sta_x, sta_y] = que[i];
        int near_j = -1, min_dist = INF, new_per_earn = per_earn;
        rep(j, i){
            auto [x, y] = que[j];
            int d = abs(sta_x-x)+abs(sta_y-y);
            new_per_earn += earnlist[x*n+y][sta_x*n+sta_y]+earnlist[sta_x*n+sta_y][x*n+y];
            if (d < min_dist){
                min_dist = d;
                near_j = j;
            }
        }
        if ((min_dist-1)*100+5000 > now_money){
            if (per_earn == 0) return -INF;
            int turn_for_earn = ((min_dist-1)*100+5000-now_money+per_earn-1)/per_earn;
            now_money += turn_for_earn*per_earn;
            now_turn += turn_for_earn;
        }
        now_money -= 5000+(min_dist-1)*100;
        now_turn += min_dist;
        now_money += (min_dist-1)*per_earn;
        now_money += new_per_earn;
        per_earn = new_per_earn;
        if (now_turn > T) return -INF;
    }
    now_money += (T-now_turn)*per_earn;
    return now_money;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start_time = std::chrono::high_resolution_clock::now();
    int n, m, K, T; cin >> n >> m >> K >> T;
    vector<pii> start, goal;
    rep(i, m){
        int sx, sy, gx, gy; cin >> sx >> sy >> gx >> gy;
        start.emplace_back(sx, sy);
        goal.emplace_back(gx, gy);
    }
    int max_money = K;
    vector<pii> que;
    auto earnlist = make_earnlist_with_station(start, goal, n);
    auto [init_list, p] = two_station(earnlist, n, K, T);
    // cerr << "init process..." << endl;
    { // give initial solution by greedy method
        vector<pii> stations;
        int now_money = K-10000, now_turn = 0, per_earn = 0;
        auto [p1, p2] = p;
        int x1 = p1/n, y1 = p1%n, x2 = p2/n, y2 = p2%n;
        int d = abs(x1-x2)+abs(y1-y2);
        per_earn = earnlist[x1*n+y1][x2*n+y2]+earnlist[x2*n+y2][x1*n+y1];
        now_money -= (d-1)*100;
        now_money += per_earn;
        now_turn += d+1;
        stations.emplace_back(x1, y1);
        stations.emplace_back(x2, y2);
        while(now_turn < T){
            if (now_money > 5000){
                auto [set_x, set_y, near_x, near_y, dir, new_per_earn, turn_for_earn] 
                    = decide_next_station(stations, earnlist, now_turn, n, T, per_earn, now_money);
                if (dir == -1){
                    now_money += (T-now_turn)*per_earn;
                    break;
                }
                now_money += turn_for_earn*per_earn;
                now_turn += turn_for_earn;
                int d = abs(set_x-near_x)+abs(set_y-near_y);
                now_money -= 5000+(d-1)*100;
                now_turn += d;
                now_money += (d-1)*per_earn;
                now_money += new_per_earn;
                per_earn = new_per_earn;
                stations.emplace_back(set_x, set_y);
            }
            else{
                int tmp_q = (5000-now_money+per_earn)/per_earn;
                int q = min(tmp_q, T-now_turn);
                now_money += q*per_earn;
                now_turn += q;
            }
        }
        que = stations;
        max_money = now_money;

        // cerr << "perf for greedy: " << now_money << endl;
        // cerr << "per-earn for greedy: " << per_earn << endl;
        // cerr << "num of station for greedy: " << que.size() << endl;
    }

    // cerr << "yaki process..." << endl;

    int times = 0, update = 0, accept = 0, alpha = 5, beta = 7, gamma = 30, del_rate = 7;
    std::random_device rd; std::mt19937 gen(rd());
    std::random_device rd2; std::mt19937 gen2(rd2());
    std::uniform_int_distribution<> dist(0, n-1);
    std::uniform_int_distribution<> prob(0, INF);
    double T_now = 1e7;
    vector<vector<bool>> used(n, vector<bool>(n));
    for (auto [x, y]: que) used[x][y] = true;
    while(1){
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        if (elapsed.count() > 2900) break;
        times++;
        auto pre_que = que;
        int op_kind = -1, op_x = -1, op_y = -1, p_op_x = -1, p_op_y = -1;
        if (times % alpha == 0){
            int isAdd = (times % (del_rate*alpha) != 0);
            if (isAdd){ // add station
                std::uniform_int_distribution<> dist2(2, que.size());
                int pos_add = dist2(gen);
                int time_limits = 0;
                while(time_limits++ < gamma){
                    int pos_x = dist(gen), pos_y = dist(gen);
                    if (used[pos_x][pos_y]) continue;
                    bool flag = true;
                    rep(i, que.size()){
                        auto [x, y] = que[i];
                        if (abs(x-pos_x)+abs(y-pos_y) <= 4){
                            flag = false;
                            break;
                        }
                    } 
                    if (flag){
                        op_x = pos_x, op_y = pos_y;
                        used[pos_x][pos_y] = true;
                        que.emplace(que.begin()+pos_add, pos_x, pos_y);
                        op_kind = 0;
                        break;
                    }
                }
            }
            else{ // delete station
                if (que.size() <= 2) continue;
                std::uniform_int_distribution<> dist2(2, que.size()-1);
                int pos_del = dist2(gen);
                auto [sta_x, sta_y] = que[pos_del];
                op_x = sta_x, op_y = sta_y;
                used[sta_x][sta_y] = false;
                que.erase(que.begin()+pos_del);
                op_kind = 1;
            }
        }
        else if (times % beta == 0){ // change init station
            std::uniform_int_distribution<> dist2(0, init_list.size()-1);
            int time_limits = 0;
            while(time_limits++ < gamma/2){
                int pos_ch = dist2(gen);
                auto [p1, p2] = init_list[pos_ch];
                int x1 = p1/n, y1 = p1%n, x2 = p2/n, y2 = p2%n;
                if (used[x1][y1] || used[x2][y2]) continue;
                bool flag = true;
                rep2(i, 2, que.size()){
                    auto [x, y] = que[i];
                    if (abs(x-x1)+abs(y-y1) <= 4 || abs(x-x2)+abs(y-y2) <= 4){
                        flag = false;
                        break;
                    }
                }
                if (flag){
                    auto [px1, py1] = que[0];
                    auto [px2, py2] = que[1];
                    p_op_x = px1*n+py1, p_op_y = px2*n+py2;
                    op_x = x1*n+y1, op_y = x2*n+y2;
                    used[px1][py1] = false, used[px2][py2] = false;
                    que[0] = {x1, y1}, que[1] = {x2, y2}; 
                    used[x1][y1] = true, used[x2][y2] = true;
                    op_kind = 2;
                    break;
                }
            }
        }
        else{
            if (times&1){ // swap
                if (que.size() <= 3) continue;
                std::uniform_int_distribution<> dist2(2, que.size()-1);
                int time_limits = 0;
                while(time_limits++ < gamma/2){
                    int pos1 = dist2(gen), pos2 = dist2(gen);
                    if (pos1 != pos2){
                        swap(que[pos1], que[pos2]);
                        op_kind = 3;
                        break;
                    }
                }
            }
            else{ // change pos of station
                if (que.size() <= 2) continue;
                std::uniform_int_distribution<> dist2(2, que.size()-1);
                int pos_ch = dist2(gen);
                int time_limits = 0;
                while(time_limits++ < gamma){
                    int pos_x = dist(gen), pos_y = dist(gen);
                    if (used[pos_x][pos_y]) continue;
                    bool flag = true;
                    rep(i, que.size()){
                        if (i == pos_ch) continue;
                        auto [x, y] = que[i];
                        if (abs(x-pos_x)+abs(y-pos_y) <= 4){
                            flag = false;
                            break;
                        }
                    } 
                    if (flag){
                        auto [px, py] = que[pos_ch];
                        p_op_x = px, p_op_y = py;
                        op_x = pos_x, op_y = pos_y;
                        used[px][py] = false;
                        que[pos_ch] = {pos_x, pos_y};
                        used[pos_x][pos_y] = true;
                        op_kind = 4;
                        break;
                    }
                }
            }
        }
        if (op_kind == -1) continue;
        int now_money = calc_money(que, earnlist, n, K, T);
        T_now *= 0.99;
        int delta = now_money - max_money;
        double p;
        if (T_now == 0) p = 0;
        else p = exp(delta/T_now);
        printf("%.012f, %.012f, %.012f, %.012f\n", T_now, p, delta, (double)randxor()/INF);
        // cout << T_now << " " << p << "/ " << delta << " " << prob(rd()) << endl;
        if (delta > 0){
            max_money = now_money;
            update++;
            // cerr << op_kind << endl;
            continue;
        }
        else if (p > (double)prob(gen2)/INF){
            max_money = now_money;
            update++;
            accept++;
            // cerr << op_kind << endl;
            continue;
        }
        if (op_kind == 0){ // add
            used[op_x][op_y] = false;
        }
        else if(op_kind == 1){ // del
            used[op_x][op_y] = true;
        }
        else if (op_kind == 2){ // ch-init
            int px1 = p_op_x/n, py1 = p_op_x%n, px2 = p_op_y/n, py2 = p_op_y%n;
            int x1 = op_x/n, y1 = op_x%n, x2 = op_y/n, y2 = op_y%n;
            used[px1][py1] = true, used[px2][py2] = true;
            used[x1][y1] = false, used[x2][y2] = false;
        }
        else if (op_kind == 3){ // swap
            // nothing
        }
        else{ // ch-pos
            used[p_op_x][p_op_y] = true;
            used[op_x][op_y] = false;
        }
        que = pre_que;
    }

    // cerr << "perf for SA: " << max_money << endl;
    // cerr << "num of station for SA: " << que.size() << endl;
    
    // cerr << "output process..." << endl;
    vector<T3> ans;
    int final_money = -1;
    { // output
        vector<vector<int>> board(n, vector<int>(n, 0));
        int now_money = K-5000, now_turn = 1, per_earn = 0;
        ans.emplace_back(0, que[0].fi, que[0].se);
        board[que[0].fi][que[0].se] = -1;
        vector<bool> dame(que.size());
        rep2(i, 1, que.size()){
            auto [sta_x, sta_y] = que[i];
            int near_j = -1, new_per_earn = per_earn;
            rep(j, i){
                if (dame[j]) continue;
                auto [x, y] = que[j];
                int d = abs(sta_x-x)+abs(sta_y-y);
                new_per_earn += earnlist[x*n+y][sta_x*n+sta_y]+earnlist[sta_x*n+sta_y][x*n+y];
            }
            auto op = able_to_construct(board, sta_x, sta_y);
            if (op.size() == 0){
                dame[i] = true;
                continue;
            }
            construct(board, ans, op, now_money, now_turn, per_earn, new_per_earn);
        }
        await(ans, now_money, now_turn, T-now_turn, per_earn);
        final_money = now_money;
    }
    

    // cerr << final_money << endl;
    cerr << "final-performance: " << final_money << endl;
    // cerr << "num of station: " << que.size() << endl;
    // cerr << "ans-size: " << ans.size() << endl;
    cerr << "times: " << times << endl;
    cerr << "update: " << update << endl;
    cerr << "accept: " << accept << endl;
    printf("%.016f\n", accept/times);

    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        cerr << "elapsed: " << elapsed.count() << endl;
    }
    // cout << final_money << endl;
    // rep(i, T){
    //     auto [p, x, y] = ans[i];
    //     if (p == -1) cout << -1 << endl;
    //     else cout << p << " " << x << " " << y << endl;
    // }
    return 0;
}