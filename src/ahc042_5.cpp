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
const char dc[] = {'U', 'R', 'D', 'L'};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}
int n=20; 
vector<pair<char, int>> calc(vector<T> op_que, vector<vector<int>> board){
    vector<pair<char, int>> res;
    // 前処理
    rep(i, n){
        int row_huku_cnt = 0, row_oni_cnt = 0;
        rep(j, n){
            if (board[i][j] == -1) row_huku_cnt++;
            else if (board[i][j] == 1) row_oni_cnt++;
        }
        if (row_huku_cnt || row_oni_cnt == 0) continue;
        int tl = 0, tr = 0;
        rep(j, n){
            if (board[i][j] == 1){
                tr = n-j;
                break;
            }
        }
        rrep2(j, n, 0){
            if (board[i][j] == 1){
                tl = j+1;
                break;
            }
        }
        if (tl < tr){
            rep(j, tl){
                res.emplace_back(dc[3], i);
            }
        }
        else{
            rep(j, tr){
                res.emplace_back(dc[1], i);
            }
        }
        rep(j, n) board[i][j] = 0;
    }
    rep(j, n){
        int col_huku_cnt = 0, col_oni_cnt = 0;
        rep(i, n){
            if (board[i][j] == -1) col_huku_cnt++;
            else if (board[i][j] == 1) col_oni_cnt++;
        }
        if (col_huku_cnt || col_oni_cnt == 0) continue;
        int tu = 0, td = 0;
        rep(i, n){
            if (board[i][j] == 1){
                td = n-i;
                break;
            }
        }
        rrep2(i, n, 0){
            if (board[i][j] == 1){
                tu = i+1;
                break;
            }
        }
        if (tu < td){
            rep(i, tu){
                res.emplace_back(dc[0], j);
            }
        }
        else{
            rep(i, td){
                res.emplace_back(dc[2], j);
            }
        }
        rep(i, n) board[i][j] = 0;
    }





    // 愚直に
    for (auto [r, k, d]: op_que){
        int x, y; 
        if (r == 1) x = k, y = (d == 1 ? n-1 : 0);
        else x = (d == 0 ? 0: n-1), y = k;
        int sx = x, sy = y;
        int cnt_oni = 0, bx = -1, by = -1;
        int cost = 0;
        while(1){
            if (board[x][y] == -1) break;
            else if (board[x][y] == 1){
                bx = x;
                by = y;
                cnt_oni++;
            }
            if (d == 0) x++;
            else if (d == 1) y--;
            else if (d == 2) x--;
            else y++;
            if (x < 0 || x >= n || y < 0 || y >= n) break;
        }
        if (cnt_oni == 0 || bx == -1 || by == -1) continue;
        x = bx, y = by;
        while(1){
            // assert(board[x][y] != -1);
            board[x][y] = 0;
            res.emplace_back(dc[d], k);
            x += dx[d], y += dy[d];
            if (x < 0 || x >= n || y < 0 || y >= n){
                break;
            }
        }
        // 復元するかどうか
        int cnt_obj = 0;
        if (d == 0 || d == 2){
            rep(i, n) if (board[i][k] != 0) cnt_obj++;
        }
        else{
            rep(j, n) if (board[k][j] != 0) cnt_obj++;
        }
        if (cnt_obj == 0) continue;
        // 復元
        if (d < 2) d += 2;
        else d -= 2;
        while(1){
            int nx = x+dx[d], ny = y+dy[d];
            if (nx == bx && ny == by){
                res.emplace_back(dc[d], k);
                break;
            }
            res.emplace_back(dc[d], k);
            x = nx, y = ny;
        }
    }
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = std::chrono::high_resolution_clock::now();
    cin >> n;
    vector<string> g(n);
    rep(i, n) cin >> g[i];
    vector<vector<int>> board(n, vector<int>(n, 0));
    rep(i, n){
        rep(j, n){
            if (g[i][j] == 'x'){
                board[i][j] = 1;
            }
            if (g[i][j] == 'o'){
                board[i][j] = -1;
            }
        }
    }
    vector<T> op_que;
    rep(i, n) op_que.emplace_back(1, i, 1), op_que.emplace_back(1, i, 3);
    rep(i, n) op_que.emplace_back(0, i, 0), op_que.emplace_back(0, i, 2);

    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, op_que.size()-1);
    vector<pair<char, int>> ans;
    int max_score = INF;
    int times = 0, update_times = 0;
    while(1){
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (elapsed.count() > 1975) break;
        int t = 1;
        while(t--){
            int x = dist(gen), y = dist(gen);
            swap(op_que[x], op_que[y]);
        }
        auto res = calc(op_que, board);
        if (8*n*n-res.size() < max_score){
            update_times++;
            max_score = 8*n*n-res.size();
            ans = res;
        }
        times++;
    }
    cerr << "times: " << times << endl;
    cerr << "update: " << update_times << endl;
    cerr << "score: " << 8*n*n-ans.size() << endl;
    for (auto [c, d]: ans) cout << c << " " << d << endl;
    return 0;
}