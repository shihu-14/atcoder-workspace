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

using namespace std;
using namespace atcoder;

using mint = modint998244353;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
using G = vector<vector<int>>;

#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = int(a)-1; i >= (b); --i)
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

// 方向定義: 0:U, 1:D, 2:L, 3:R
const int dr[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, -1, 1};
const char dir_char[] = {'U', 'D', 'L', 'R'};

int N, M, C;
int target_seq[205];
int board[20][20];
deque<pii> snake;
vector<int> snake_colors;

void solve()
{
    if (!(cin >> N >> M >> C)) return;
    for (int i = 0; i < M; ++i) {
        cin >> target_seq[i];
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> board[i][j];
        }
    }

    // 初期状態のセットアップ
    snake.push_back({4, 0});
    snake.push_back({3, 0});
    snake.push_back({2, 0});
    snake.push_back({1, 0});
    snake.push_back({0, 0});
    for(int i = 0; i < 5; ++i) {
        snake_colors.push_back(1);
    }

    // マクロ rng との衝突を避けるため変数名を rnd に変更
    mt19937 rnd(1337);
    vector<char> ans;
    int turn = 0;

    while (snake_colors.size() < M && turn < 100000) {
        int target_color = target_seq[snake_colors.size()];
        vector<pii> targets;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == target_color) {
                    targets.push_back({i, j});
                }
            }
        }

        int dist[20][20];
        int fstep[20][20];
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                dist[i][j] = INF;
                fstep[i][j] = -1;
            }
        }

        queue<pii> q;
        q.push({snake[0].fi, snake[0].se});
        dist[snake[0].fi][snake[0].se] = 0;

        // BFSによる最短経路探索
        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            for (int dir = 0; dir < 4; ++dir) {
                int nr = r + dr[dir];
                int nc = c + dc[dir];

                if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
                if (dist[nr][nc] != INF) continue;

                // 目的の餌以外の餌は障害物とみなす
                if (board[nr][nc] > 0 && board[nr][nc] != target_color) continue;

                int d = dist[r][c] + 1;
                bool is_body = false;

                for (int i = 0; i < (int)snake.size(); ++i) {
                    if (snake[i].fi == nr && snake[i].se == nc) {
                        // dターン後には尻尾からd個のマスは空くはず
                        if (i < (int)snake.size() - d) {
                            is_body = true;
                        }
                    }
                }
                
                // Uターン禁止チェック
                if (d == 1 && snake.size() >= 2 && nr == snake[1].fi && nc == snake[1].se) {
                    is_body = true;
                }

                if (is_body) continue;

                dist[nr][nc] = d;
                if (d == 1) fstep[nr][nc] = dir;
                else fstep[nr][nc] = fstep[r][c];

                q.push({nr, nc});
            }
        }

        int best_dir = -1;
        int min_d = INF;
        for (auto t : targets) {
            if (dist[t.fi][t.se] < min_d) {
                min_d = dist[t.fi][t.se];
                best_dir = fstep[t.fi][t.se];
            }
        }

        int move_dir = -1;
        if (best_dir != -1) {
            move_dir = best_dir;
        } else {
            // BFSで到達できない場合はランダムウォークで盤面を乱す
            vector<int> valid_dirs;
            for (int dir = 0; dir < 4; ++dir) {
                int nr = snake[0].fi + dr[dir];
                int nc = snake[0].se + dc[dir];

                if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
                if (board[nr][nc] > 0 && board[nr][nc] != target_color) continue; // 違う色の餌は絶対に食べない

                bool is_body = false;
                for (int i = 0; i < (int)snake.size() - 1; ++i) { // 尻尾は動くので直進可能
                    if (snake[i].fi == nr && snake[i].se == nc) is_body = true;
                }
                if (snake.size() >= 2 && nr == snake[1].fi && nc == snake[1].se) is_body = true;

                if (!is_body) valid_dirs.push_back(dir);
            }

            if (!valid_dirs.empty()) {
                move_dir = valid_dirs[rnd() % valid_dirs.size()];
            } else {
                // 完全に詰まった場合の最終手段（胴体への噛みちぎりを許容）
                for (int dir = 0; dir < 4; ++dir) {
                    int nr = snake[0].fi + dr[dir];
                    int nc = snake[0].se + dc[dir];
                    if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
                    if (snake.size() >= 2 && nr == snake[1].fi && nc == snake[1].se) continue; // Uターンは禁止
                    if (board[nr][nc] > 0 && board[nr][nc] != target_color) continue; // 違う色の餌は絶対に食べない
                    valid_dirs.push_back(dir);
                }
                if (valid_dirs.empty()) break; // 完全なデッドロック（ルール違反を避けるため終了）
                move_dir = valid_dirs[rnd() % valid_dirs.size()];
            }
        }

        // 決定した方向へ進む
        int nr = snake[0].fi + dr[move_dir];
        int nc = snake[0].se + dc[move_dir];
        ans.push_back(dir_char[move_dir]);

        if (board[nr][nc] > 0) {
            // 餌を食べる（ターゲット色であることが保証されている）
            snake_colors.push_back(board[nr][nc]);
            board[nr][nc] = 0;
            snake.push_front({nr, nc});
        } else {
            // 噛みちぎり判定
            bool bit_self = false;
            int bit_idx = -1;
            // 尻尾 (size - 1) に衝突する場合は単なる移動になるため除外
            for (int i = 1; i < (int)snake.size() - 1; ++i) { 
                if (snake[i].fi == nr && snake[i].se == nc) {
                    bit_self = true;
                    bit_idx = i;
                    break;
                }
            }

            snake.push_front({nr, nc});

            if (bit_self) {
                // 噛みちぎり発生：切断された部分を餌として盤面に再配置
                // 衝突した胴体は古い配列の bit_idx の位置。新しい長さは bit_idx + 2 になる。
                int new_len = bit_idx + 2; 
                while ((int)snake.size() > new_len) {
                    pii drop_pos = snake.back();
                    int drop_color = snake_colors.back();
                    board[drop_pos.fi][drop_pos.se] = drop_color;
                    snake.pop_back();
                    snake_colors.pop_back();
                }
            } else {
                // 通常の移動
                snake.pop_back();
            }
        }
        turn++;
    }

    for (char c : ans) {
        cout << c << "\n";
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
    return 0;
}