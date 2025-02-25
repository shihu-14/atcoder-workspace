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
using T = tuple<int, int, int>;
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

// ある2つの位置に駅を置いたときに得られる総利益
vector<vector<int>> make_earnlist_with_station(vector<pii> &start, vector<pii> &goal, int n){
    vector<vector<int>> dist(n*n, vector<int>(n*n));
    rep(i, start.size()){
        auto [x1, y1] = start[i];
        auto [x2, y2] = goal[i];
        rep2(dx1, -2, 2)rep2(dy1, -2, 2){
            rep2(dx2, -2, 2)rep2(dy2, -2, 2){
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
tuple<int, int, int, int, int> calc_balance(vector<vector<int>> &board, vector<pii> &stations, vector<vector<int>> &earnlist, int turn, int T, int pos_x, int pos_y, int now_per_earn){
    int min_dist = INF;
    int near_x = -1, near_y = -1;
    for (auto [sta_x, sta_y]: stations){
        int d = abs(sta_x-pos_x)+abs(sta_y-pos_y);
        if (d < min_dist){
            min_dist = d;
            near_x = sta_x, near_y = sta_y;
        }
    }
    if (turn+min_dist > T) return {0, INF, -1, -1, -1};
    int n = board.size();
    int earn = now_per_earn*(T-turn), new_earn = 0;
    new_earn -= 5000+(min_dist-1)*100;
    int tmp_sum = now_per_earn;
    for (auto [sta_x, sta_y]: stations){
        tmp_sum += earnlist[pos_x*n+pos_y][sta_x*n+sta_y];
        tmp_sum += earnlist[sta_x*n+sta_y][pos_x*n+pos_y];
    }
    new_earn += tmp_sum*(T-turn-min_dist);
    return {earn, new_earn, near_x, near_y, tmp_sum};
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
tuple<int, int, int, int, int, int> decide_next_station(vector<vector<int>> &board, vector<pii> &stations, vector<vector<int>> &earnlist, int turn, int T, int now_per_earn){
    int n = board.size();
    int max_diff = 0;
    int set_x = -1, set_y = -1, near_x = -1, near_y = -1, dir = -1, per_earn = -1;
    rep(x, n)rep(y, n) if (board[x][y] == 0){
        bool ok = true;
        for (auto [sta_x, sta_y]: stations){
            if (abs(sta_x-x)+abs(sta_y-y) <= 4){
                ok = false;
                break;
            }
        }
        if (ok == false) continue;
        auto [earn, new_earn, tmp_near_x, tmp_near_y, tmp_per_earn] = calc_balance(board, stations, earnlist, turn, T, x, y, now_per_earn);
        int diff = new_earn-earn;
        if (diff <= 0) continue;
        int flag = able_to_construct(board, x, y, tmp_near_x, tmp_near_y);
        if (flag != 0){
            if (max_diff < diff){
                max_diff = diff;
                set_x = x, set_y = y, near_x = tmp_near_x, near_y = tmp_near_y, dir = flag, per_earn = tmp_per_earn;
            }
        }
    }
    return {set_x, set_y, near_x, near_y, dir, per_earn};
}

void construct(vector<vector<int>> &board, vector<T> &ans, vector<pii> &stations, int sta_x, int sta_y, int near_x, int near_y, int dir){
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
        int y_d = near_y-sta_y;
        while(sta_y != near_y){
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

void await(vector<T> &ans, int turn){
    rep(t, turn) ans.emplace_back(-1, -1, -1);
}

// return [建設にかかる総コスト, かかる総ターン, per_earn]
tuple<int, int, int> two_station(vector<vector<int>> &board, vector<T> &ans, vector<pii> &stations, vector<vector<int>> &earnlist, int K, int T){
    int n = board.size();
    int max_money = -INF, max_x1 = -1, max_y1 = -1, max_x2 = -1, max_y2 = -1;
    int res_cost = -1, res_turn = -1, per_earn = -1;
    rep(x1, n)rep(y1, n)rep(x2, n)rep(y2, n){
        int d = abs(x1-x2)+abs(y1-y2);
        int money = K, turn = T;
        turn -= d+1;
        money -= (d-1)*100+2*5000;
        if (money < 0) continue;
        money += (earnlist[x1*n+y1][x2*n+y2]+earnlist[x2*n+y2][x1*n+y1])*turn;
        if (money > max_money){
            max_money = money;
            max_x1 = x1, max_y1 = y1;
            max_x2 = x2, max_y2 = y2;
            res_cost = (d-1)*100+2*5000;
            res_turn = d+1;
            per_earn = (earnlist[x1*n+y1][x2*n+y2]+earnlist[x2*n+y2][x1*n+y1]);
        }
    }
    board[max_x1][max_y1] = -1;
    ans.emplace_back(0, max_x1, max_y1);
    stations.emplace_back(max_x1, max_y1);
    construct(board, ans, stations, max_x2, max_y2, max_x1, max_y1, 1);
    return {res_cost, res_turn, per_earn};
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, K, T; cin >> n >> m >> K >> T;
    vector<pii> start, goal;
    rep(i, m){
        int sx, sy, gx, gy; cin >> sx >> sy >> gx >> gy;
        start.emplace_back(sx, sy);
        goal.emplace_back(gx, gy);
    }
    auto earnlist = make_earnlist_with_station(start, goal, n);
    vector<tuple<int, int, int>> ans;
    int now_turn = 0;
    int now_money = K, per_earn = 0;
    vector<vector<int>> board(n, vector<int>(n));
    vector<pii> stations;
    {
        auto [cost, turn, per_earn_money] = two_station(board, ans, stations, earnlist, K, T);
        now_money -= cost;
        now_turn += turn;
        per_earn = per_earn_money;
    }
    while(now_turn < T){
        if (now_money > 5000+100*2*n){
            auto [set_x, set_y, near_x, near_y, dir, new_per_earn] = decide_next_station(board, stations, earnlist, now_turn, T, per_earn);
            if (dir == -1){
                await(ans, T-now_turn);
                break;
            } 
            int d = abs(set_x-near_x)+abs(set_y-near_y);
            now_money -= 5000+(d-1)*100;
            now_turn += d;
            per_earn = new_per_earn;
            construct(board, ans, stations, set_x, set_y, near_x, near_y, dir);
        }
        else{
            int tmp_q = (5000+100*2*n-now_money+per_earn)/per_earn;
            int q = min(tmp_q, T-now_turn);
            now_turn += q;
            now_money += q*per_earn;
            await(ans, q);
        }
    }

    rep(i, ans.size()){
        auto [p, x, y] = ans[i];
        if (p == -1) cout << -1 << endl;
        else cout << p << " " << x << " " << y << endl;
    }
    return 0;
}

/*
2駅の最適解を求める。
次に設置する駅を全探査して、建設することで得られる残りの利益>建設にかかるコストである位置&&最短経路で橋を設計することが可能であるもののうち、この差が一番大きいものに決定。
更新するのは、
board, ans, stations -> construc()に任せる
now_turn, now_money, per_earnはmain()内で 
*/