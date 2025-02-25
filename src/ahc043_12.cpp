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
using T9 = tuple<int, int, int, int, int, int, int, int, int>;
using T12 = tuple<int, int, int, int, int, int, int, int, int, int, int, int>;
using T13 = tuple<int, int, int, int, int, int, int, int, int, int, int, int, int>;

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
T9 calc_balance(vector<vector<int>> &board, vector<pii> &stations, vector<vector<int>> &earnlist, int turn, int T, 
        int pos_x1, int pos_y1, int pos_x2, int pos_y2, int now_per_earn, int now_money){
    int min_dist1 = INF, min_dist2 = INF;
    int near_x1 = -1, near_y1 = -1, near_x2 = -1, near_y2 = -1;
    for (auto [sta_x, sta_y]: stations){ // pos_x, pos_yから一番近い駅near_x, near_yを求める。
        int d1 = abs(sta_x-pos_x1)+abs(sta_y-pos_y1);
        int d2 = abs(sta_x-pos_x2)+abs(sta_y-pos_y2);
        if (d1 < min_dist1){
            min_dist1 = d1;
            near_x1 = sta_x, near_y1 = sta_y;
        }
        if (d2 < min_dist2){
            min_dist2 = d2;
            near_x2 = sta_x, near_y2 = sta_y;
        }
    }
    int d = abs(pos_x1-pos_x2)+abs(pos_y1-pos_y2);
    if (min_dist1 > d || min_dist2 > d){
        if (min_dist1 > min_dist2){
            near_x1 = pos_x2, near_y1 = pos_y2;
            min_dist1 = d;
        }
        else{
            near_x2 = pos_x1, near_y2 = pos_y1;
            min_dist2 = d;
        }
    }

    int turn_for_earn = ((min_dist1-1+min_dist2-1)*100-(now_money-10000)+now_per_earn-1)/now_per_earn; // 橋の建設費用を稼ぐ。
    if (turn_for_earn < 0) turn_for_earn = 0;
    if (turn+min_dist1+min_dist2+turn_for_earn > T) return {INF, 0, -1, -1, -1, -1, -1, -1, -1};
    int n = board.size();
    int earn = now_per_earn*(T-turn), new_earn = 0;
    new_earn -= 5000+(min_dist1-1)*100;
    new_earn -= 5000+(min_dist2-1)*100;
    int tmp_sum1 = now_per_earn, tmp_sum2 = 0;
    for (auto [sta_x, sta_y]: stations){
        tmp_sum1 += earnlist[pos_x1*n+pos_y1][sta_x*n+sta_y]+earnlist[sta_x*n+sta_y][pos_x1*n+pos_y1];
        tmp_sum2 += earnlist[pos_x2*n+pos_y2][sta_x*n+sta_y]+earnlist[sta_x*n+sta_y][pos_x2*n+pos_y2];
    }
    tmp_sum2 += tmp_sum1;
    new_earn += tmp_sum2*(T-turn-min_dist1-min_dist2-turn_for_earn);
    return {earn, new_earn, near_x1, near_y1, near_x2, near_y2, tmp_sum1, tmp_sum2, turn_for_earn};
}
int able_to_construct(vector<vector<int>> &board, int set_x, int set_y, int near_x, int near_y){
    if (set_x > near_x){
        swap(set_x, near_x);
        swap(set_y, near_y);
    }
    int x = set_x, y = set_y;
    int flag = 1;
    while(x != near_x){
        x++;
        if (x != near_x && board[x][y] != 0){
            flag = 0;
            break;
        }
        else if (x == near_x && y != near_y && board[x][y] != 0){
            flag = 0;
            break;
        }
    }
    if (flag == 0) return 0;
    while(y != near_y){
        if (y < near_y) y++;
        else y--;
        if (y != near_y && board[x][y] != 0){
            flag = 0;
            break;
        }
    }
    if (flag) return flag;
    flag = 2;
    x = set_x, y = set_y;
    while(y != near_y){
        if (y < near_y) y++;
        else y--;
        if (y != near_y && board[x][y] != 0){
            flag = 0;
            break;
        }
        else if (y == near_y && x != near_x && board[x][y] != 0){
            flag = 0;
            break;
        }
    }
    if (flag == 0) return 0;
    while(x != near_x){
        x++;
        if (x != near_x && board[x][y] != 0){
            flag = 0;
            break;
        }
    }
    return flag;
}
T13 decide_next_station(vector<vector<int>> &board, vector<pii> &stations, vector<vector<int>> &earnlist, int turn, int T, int now_per_earn, int now_money){
    int n = board.size();
    int max_diff = 0;
    int set_x1 = -1, set_y1 = -1, near_x1 = -1, near_y1 = -1, set_x2 = -1, set_y2 = -1, near_x2 = -1, near_y2 = -1, 
        dir1 = -1, dir2 = -1, per_earn1 = -1, per_earn2 = -1, turn_for_earn = -1;
    rep(x1, n)rep(y1, n)rep(x2, n)rep(y2, n) if (board[x1][y1] == 0 && board[x2][y2] == 0){
        if (abs(x1-x2)+abs(y1-y2) <= 4) continue;
        bool ok = false;
        for (auto [sta_x, sta_y]: stations){
            if (abs(sta_x-x1)+abs(sta_y-y1) <= 4 || abs(sta_x-x2)+abs(sta_y-y2) <= 4){
                ok = true;
                break;
            }
        }
        if (ok) continue;
        auto [earn, new_earn, tmp_near_x1, tmp_near_y1, tmp_near_x2, tmp_near_y2, tmp_per_earn1, tmp_per_earn2, tmp_turn_for_earn] = calc_balance(board, stations, earnlist, turn, T, x1, y1, x2, y2, now_per_earn, now_money);
        int diff = new_earn-earn;
        if (diff <= 0) continue;
        int flag1 = able_to_construct(board, x1, y1, tmp_near_x1, tmp_near_y1);
        int flag2 = able_to_construct(board, x2, y2, tmp_near_x2, tmp_near_y2);
        if (flag1 != 0 && flag2 != 0){
            if (max_diff < diff){
                max_diff = diff;
                set_x1 = x1, set_y1 = y1, near_x1 = tmp_near_x1, near_y1 = tmp_near_y1, 
                set_x2 = x2, set_y2 = y2, near_x2 = tmp_near_x2, near_y2 = tmp_near_y2, 
                dir1 = flag1, dir2 = flag2, per_earn1 = tmp_per_earn1, per_earn2 = tmp_per_earn2, turn_for_earn = tmp_turn_for_earn;
            }
        }
    }
    return {set_x1, set_y1, near_x1, near_y1, set_x2, set_y2, near_x2, near_y2, dir1, dir2, per_earn1, per_earn2, turn_for_earn};
}


void construct(vector<vector<int>> &board, vector<T3> &ans, vector<pii> &stations, int sta_x, int sta_y, int near_x, int near_y, int dir,
    int &now_money, int &now_turn, int &per_earn, int new_per_earn){

    int d = abs(sta_x-near_x)+abs(sta_y-near_y);
    now_money -= 5000+(d-1)*100;
    now_turn += d;
    now_money += (d-1)*per_earn;
    now_money += new_per_earn;
    per_earn = new_per_earn;

    board[sta_x][sta_y] = -1;
    ans.emplace_back(0, sta_x, sta_y);
    stations.emplace_back(sta_x, sta_y);
    if (sta_x > near_x){
        swap(sta_x, near_x);
        swap(sta_y, near_y);
    }
    if (dir == 1){
        while(sta_x != near_x){
            sta_x++;
            if (sta_x != near_x) ans.emplace_back(2, sta_x, sta_y);
            else{
                if (sta_y == near_y) break;
                if (sta_y < near_y) ans.emplace_back(5, sta_x, sta_y);
                else ans.emplace_back(4, sta_x, sta_y);
            }
            board[sta_x][sta_y] = 1;
        }
        while(sta_y != near_y){
            if (sta_y < near_y) sta_y++;
            else sta_y--;
            if (sta_y != near_y){
                ans.emplace_back(1, sta_x, sta_y);
                board[sta_x][sta_y] = 1;
            }
        }
    }
    else if (dir == 2){
        while(sta_y != near_y){
            int y_d = near_y-sta_y;
            if (sta_y < near_y) sta_y++;
            else sta_y--;
            if (sta_y != near_y) ans.emplace_back(1, sta_x, sta_y);
            else{
                if (sta_x == near_x) break;
                if (y_d > 0) ans.emplace_back(3, sta_x, sta_y);
                else ans.emplace_back(6, sta_x, sta_y);
            }
            board[sta_x][sta_y] = 1;
        }
        while(sta_x != near_x){
            sta_x++;
            if (sta_x != near_x){
                ans.emplace_back(2, sta_x, sta_y);
                board[sta_x][sta_y] = 1;
            }
        }
    }
}

void await(vector<T3> &ans, int &now_money, int &now_turn, int turn, int per_earn){
    now_money += turn*per_earn;
    now_turn += turn;
    rep(t, turn) ans.emplace_back(-1, -1, -1);
}


pii two_station(vector<vector<int>> &earnlist, int n, int K, int T){
    int max_money = K, max_p1 = -1, max_p2 = -1;
    rep(x1, n)rep(y1, n)rep(x2, n)rep(y2, n){
        int d = abs(x1-x2)+abs(y1-y2);
        if (d <= 4) continue;
        int money = K, turn = 0;
        turn += d+1;
        money -= (d-1)*100+2*5000;
        if (money < 0) continue;
        money += (earnlist[x1*n+y1][x2*n+y2]+earnlist[x2*n+y2][x1*n+y1])*(T-turn);
        if (money > max_money){
            max_money = money;
            max_p1 = x1*n+y1, max_p2 = x2*n+y2;
        }
    }
    return {max_p1, max_p2};
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
    auto earnlist = make_earnlist_with_station(start, goal, n);
    vector<vector<int>> board(n, vector<int>(n));
    vector<pii> stations;
    vector<T3> ans;
    int now_money = K-5000, now_turn = 1, per_earn = 0;
    auto [p1, p2] = two_station(earnlist, n, K, T);
    int x1 = p1/n, y1 = p1%n, x2 = p2/n, y2 = p2%n;
    int new_per_earn = (earnlist[x1*n+y1][x2*n+y2]+earnlist[x2*n+y2][x1*n+y1]);
    board[x1][y1] = -1;
    ans.emplace_back(0, x1, y1);
    stations.emplace_back(x1, y1);
    construct(board, ans, stations, x2, y2, x1, y1, 1,
        now_money, now_turn, per_earn, new_per_earn);
    while(now_turn < T){
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        if (elapsed.count() > 2850) break;
        if (now_money > 10000){
            auto [set_x1, set_y1, near_x1, near_y1, set_x2, set_y2, near_x2, near_y2, 
                dir1, dir2, new_per_earn1, new_per_earn2, turn_for_earn] 
                = decide_next_station(board, stations, earnlist, now_turn, T, per_earn, now_money);
            if (dir1 == -1 || dir2 == -1){
                await(ans, now_money, now_turn, T-now_turn, per_earn);
                break;
            }
            await(ans, now_money, now_turn, turn_for_earn, per_earn);
            construct(board, ans, stations, set_x1, set_y1, near_x1, near_y1, dir1,
                now_money, now_turn, per_earn, new_per_earn1);
            construct(board, ans, stations, set_x2, set_y2, near_x2, near_y2, dir2,
                now_money, now_turn, per_earn, new_per_earn2);
        }
        else{
            int tmp_q = (10000-now_money+per_earn)/per_earn;
            int q = min(tmp_q, T-now_turn);
            await(ans, now_money, now_turn, q, per_earn);
        }
    }
    if (ans.size() < T){
        await(ans, now_money, now_turn, T-now_turn, per_earn);
    }

    cerr << "final-performance: " << now_money << endl;

    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        cerr << "elapsed: " << elapsed.count() << endl;
    }

    rep(i, ans.size()){
        auto [p, x, y] = ans[i];
        if (p == -1) cout << -1 << endl;
        else cout << p << " " << x << " " << y << endl;
    }
    return 0;
}