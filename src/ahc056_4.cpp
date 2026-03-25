#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <tuple>
#include <set> // map<pair<int,int>, bool> の代わりに set を使う

using namespace std;

// --- 戦略パラメータ ---
const int C = 32;
const int Q = 64;
// --------------------

int N, K, T;
vector<string> v, h; // 壁情報
vector<pair<int, int>> gv; // 目的地リスト

// (i,j) 座標を C 色に圧縮
int get_color(int i, int j) {
    long long hash_val = (long long)i * 1000000007 + j;
    return (int)(hash_val % C);
}

// 移動方向 (U, R, D, L, Stay)
const int dx[] = {-1, 0, 1, 0, 0};
const int dy[] = {0, 1, 0, -1, 0};
const char dc[] = {'U', 'R', 'D', 'L', 'S'};

// (x,y) から方向 d に壁を無視して移動できるか
bool can_move(int x, int y, int d) {
    int nx = x + dx[d];
    int ny = y + dy[d];
    if (nx < 0 || nx >= N || ny < 0 || ny >= N) return false;
    if (d == 0 && h[nx][ny] == '1') return false; // U
    if (d == 1 && v[x][y] == '1') return false;   // R
    if (d == 2 && h[x][y] == '1') return false;   // D
    if (d == 3 && v[nx][ny] == '1') return false; // L
    return true;
}

void solve() {
    // 1. 入力
    cin >> N >> K >> T;
    v.resize(N);
    h.resize(N - 1);
    gv.resize(K);
    for (int i = 0; i < N; ++i) cin >> v[i];
    for (int i = 0; i < N - 1; ++i) cin >> h[i];
    for (int i = 0; i < K; ++i) cin >> gv[i].first >> gv[i].second;

    // 2. 「完璧な」最短経路の事前計算
    vector<vector<vector<int>>> perfect_dir(K - 1, vector<vector<int>>(N, vector<int>(N, 4)));
    for (int k = 0; k < K - 1; ++k) {
        int gx = gv[k + 1].first;
        int gy = gv[k + 1].second;
        queue<pair<int, int>> q_bfs;
        q_bfs.push({gx, gy});
        vector<vector<int>> dist(N, vector<int>(N, -1));
        dist[gx][gy] = 0;
        perfect_dir[k][gx][gy] = 4; // 'S'

        while (!q_bfs.empty()) {
            pair<int, int> curr = q_bfs.front();
            q_bfs.pop();
            int x = curr.first;
            int y = curr.second;
            for (int d = 0; d < 4; ++d) {
                if (can_move(x, y, d)) {
                    int nx = x + dx[d];
                    int ny = y + dy[d];
                    if (dist[nx][ny] == -1) {
                        dist[nx][ny] = dist[x][y] + 1;
                        perfect_dir[k][nx][ny] = (d + 2) % 4;
                        q_bfs.push({nx, ny});
                    }
                }
            }
        }
    }

    // 3. 遷移規則 A, S, D の「全 2048 パターン」を生成
    vector<vector<int>> A(C, vector<int>(Q));
    vector<vector<int>> S(C, vector<int>(Q));
    vector<vector<char>> D(C, vector<char>(Q));

    for (int c = 0; c < C; ++c) {
        for (int q = 0; q < Q; ++q) {
            A[c][q] = c; // 色は変更しない
            S[c][q] = q; // 状態は維持 (デフォルト)

            vector<int> votes(5, 0); // U, R, D, L, S
            for (int k = 0; k < K - 1; ++k) {
                if (k % Q != q) continue;
                for (int i = 0; i < N; ++i) {
                    for (int j = 0; j < N; ++j) {
                        if (get_color(i, j) != c) continue;
                        votes[perfect_dir[k][i][j]]++;
                    }
                }
            }
            int max_votes = -1;
            int best_dir = 4;
            for(int d = 0; d < 5; ++d) {
                if(votes[d] > max_votes) {
                    max_votes = votes[d];
                    best_dir = d;
                }
            }
            D[c][q] = dc[best_dir];
        }
    }

    // S (状態更新) の例外ルールを上書き
    for (int k = 0; k < K - 1; ++k) {
        int arrival_x = gv[k + 1].first;
        int arrival_y = gv[k + 1].second;
        int arrival_color = get_color(arrival_x, arrival_y);
        int current_q = k % Q;
        int next_q = (k + 1) % Q;
        S[arrival_color][current_q] = next_q;
    }

    // 4. 【修正箇所】実際に使用するルールをシミュレーションして収集
    set<pair<int, int>> used_rules_set; // 遭遇した (c, q) のペアを記録
    int current_x = gv[0].first;
    int current_y = gv[0].second;
    int current_q = 0; // k=0 (gv[0]->gv[1]) は 0%Q

    for (int step = 0; step < T; ++step) {
        int c = get_color(current_x, current_y);
        int q = current_q;

        // この (c, q) ルールを初めて使った
        used_rules_set.insert({c, q});

        // ルールを適用
        int next_q = S[c][q];
        char move_dir = D[c][q];
        
        current_q = next_q; // 状態を更新

        // 移動
        int d = 4; // 'S'
        if (move_dir == 'U') d = 0;
        else if (move_dir == 'R') d = 1;
        else if (move_dir == 'D') d = 2;
        else if (move_dir == 'L') d = 3;

        if (d != 4 && can_move(current_x, current_y, d)) {
            current_x += dx[d];
            current_y += dy[d];
        }
        
        // 全ての目的地を訪問したかチェック (シミュレーションの早期終了)
        bool all_visited = true;
        int target_idx = 0; // ロボットが今目指すべき目的地 (gv[target_idx])
        // current_q から k を復元するのは困難なため、
        // 単純に T ステップ回す (T=X*2 なので、十分間に合うはず)
        // ...と思ったが、Tステップ回るだけだと T < 2048 のケースでM=2048になる
        // シミュレーションは T ステップで打ち切る必要がある
    }
    
    // Tステップのシミュレーションで遭遇したルールだけを出力する
    // (注意: T ステップで全目的地に到達できるとは限らないが、
    //  T <= 2X の制約と「多数決」の経路が X*2 ステップ以内に
    //  収まることを信じる)

    // 5. 出力
    int M = used_rules_set.size();
    cout << C << " " << Q << " " << M << endl;

    // 初期盤面の色
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << get_color(i, j) << (j == N - 1 ? "" : " ");
        }
        cout << endl;
    }

    // 遷移規則 (M行)
    for (const auto& rule_pair : used_rules_set) {
        int c = rule_pair.first;
        int q = rule_pair.second;
        cout << c << " " << q << " " << A[c][q] << " " << S[c][q] << " " << D[c][q] << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}