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

焼きなまし

*/

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
tuple<int, int, int, vector<pii>> calc_balance(vector<vector<int>> &board, vector<pii> &stations, vector<vector<int>> &earnlist,
    int pos_x, int pos_y, int now_money, int now_turn, int per_earn, int T){
    int n = board.size();
    auto op = able_to_construct(board, pos_x, pos_y);
    int d = max(0, (int)op.size()-2);
    int need = 100+(d-1)*(100-per_earn);
    if (per_earn >= 100) need = 100;
    int q = max(0, (need-now_money+per_earn-1)/per_earn);
    if (d == 0) q = 0;
    if (now_turn+d+1+q > T) return {INF, 0, -1, {}};
    int earn = per_earn*(T-now_turn), new_earn = 0;
    int new_per_earn = per_earn;
    for (auto [sta_x, sta_y]: stations){
        new_per_earn += earnlist[pos_x*n+pos_y][sta_x*n+sta_y];
        new_per_earn += earnlist[sta_x*n+sta_y][pos_x*n+pos_y];
    }
    new_earn -= 5000+d*100;
    new_earn += per_earn*d+new_per_earn;
    new_earn += new_per_earn*(T-now_turn-d-q);
    return {earn, new_earn, new_per_earn, op};
}

tuple<vector<pii>, int> decide_next_station(vector<vector<int>> &board, vector<pii> &stations, vector<vector<int>> &earnlist, int now_money, int now_turn, int per_earn, int T){
    int max_diff = 0, new_per_earn = -1, n = board.size();
    vector<pii> op;
    rep(x, n)rep(y, n){
        bool ok = true;
        for (auto [sta_x, sta_y]: stations){
            if (abs(sta_x-x)+abs(sta_y-y) <= 4){
                ok = false;
                break;
            }
        }
        if (ok == false) continue;
        auto [earn, new_earn, tmp_per_earn, tmp_op] = calc_balance(board, stations, earnlist, x, y, now_money, now_turn, per_earn, T);
        int diff = new_earn-earn;
        if (diff <= 0) continue;
        if (max_diff < diff){
            max_diff = diff;
            new_per_earn = tmp_per_earn;
            op = tmp_op;
        }
    }
    return {op, new_per_earn};
}

void await(vector<T3> &ans, int &now_money, int &now_turn, int turn, int per_earn){
    rep(t, turn){
        now_money += per_earn;
        now_turn++;
        ans.emplace_back(-1, -1, -1);
    }
}


void construct(vector<vector<int>> &board, vector<pii> &stations, vector<T3> &ans, vector<pii> &op, int &now_money, int &now_turn, int &per_earn, int new_per_earn){
    int n = board.size();
    if (now_money < 5000){ // 駅の建設のための資金稼ぎ
        if (per_earn == 0){
            cerr << -1 << endl;
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
        stations.emplace_back(sta_x, sta_y);
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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start_time = std::chrono::high_resolution_clock::now();
    int n, m, K, T; cin >> n >> m >> K >> T;
    // T = 200;
    vector<pii> start, goal;
    rep(i, m){
        int sx, sy, gx, gy; cin >> sx >> sy >> gx >> gy;
        start.emplace_back(sx, sy);
        goal.emplace_back(gx, gy);
    }
    auto earnlist = make_earnlist_with_station(start, goal, n);
    auto [init_list, p] = two_station(earnlist, n, K, T);
    // cerr << "inti process..." << endl;
    vector<vector<int>> board(n, vector<int>(n));
    vector<pii> stations;
    vector<T3> ans;
    int now_money = K-5000, now_turn = 1, per_earn = 0;
    {
        auto [p1, p2] = p;
        int x1 = p1/n, y1 = p1%n, x2 = p2/n, y2 = p2%n;
        int new_per_earn = earnlist[x1*n+y1][x2*n+y2]+earnlist[x2*n+y2][x1*n+y1];
        board[x1][y1] = -1;
        stations.emplace_back(x1, y1);
        ans.emplace_back(0, x1, y1);
        auto op = able_to_construct(board, x2, y2);
        construct(board, stations, ans, op, now_money, now_turn, per_earn, new_per_earn);
    }
    while(now_turn < T){
        cerr << now_turn << " " << now_money << " " << ans.size() << endl;
        if (now_money > 5000){
            auto [op, new_per_earn] = decide_next_station(board, stations, earnlist, now_money, now_turn, per_earn, T);
            if (op.size() == 0){
                await(ans, now_money, now_turn, T-now_turn, per_earn);
                break;
            }
            construct(board, stations, ans, op, now_money, now_turn, per_earn, new_per_earn);
        }
        else{
            int tmp_q = (5000-now_money+per_earn)/per_earn;
            int q = min(tmp_q, T-now_turn);
            await(ans, now_money, now_turn, q, per_earn);
        }
    }

    cerr << "final-performance: " << now_money << endl;
    // cerr << "num of station: " << que.size() << endl;
    // cerr << "ans-size: " << ans.size() << endl;
    // cerr << "times: " << times << endl;
    // cerr << "update: " << update << endl;

    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        cerr << "elapsed: " << elapsed.count() << endl;
    }

    rep(i, T){
        auto [p, x, y] = ans[i];
        if (p == -1) cout << -1 << endl;
        else cout << p << " " << x << " " << y << endl;
    }
    return 0;
}