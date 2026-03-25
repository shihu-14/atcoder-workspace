#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// 方向: 0=U,1=R,2=D,3=L
const int dx4[] = {-1, 0, 1, 0};
const int dy4[] = {0, 1, 0, -1};
const char dc4[] = {'U', 'R', 'D', 'L'};

// ルール (c, q, a, s, d)
using T5 = tuple<int, int, int, int, char>;

int N_global;
int K_global;
ll T_global;
vector<string> v_wall; // vertical walls: size N, each length N
vector<string> h_wall; // horizontal walls: size N-1, each length N
vector<pair<int,int>> gv; // goals

// 壁判定: (x,y) から方向 d に動けるか
bool can_move(int x, int y, int d) {
    int n = N_global;
    int nx = x + dx4[d];
    int ny = y + dy4[d];
    if (nx < 0 || nx >= n || ny < 0 || ny >= n) return false;
    if (d == 0) { // up: (x,y)->(x-1,y)
        if (h_wall[nx][ny] == '1') return false;
    } else if (d == 1) { // right: (x,y)->(x,y+1)
        if (v_wall[x][y] == '1') return false;
    } else if (d == 2) { // down: (x,y)->(x+1,y)
        if (h_wall[x][y] == '1') return false;
    } else { // left: (x,y)->(x,y-1)
        if (v_wall[nx][ny] == '1') return false;
    }
    return true;
}

// 1区間 (sx,sy)->(gx,gy) の最短路 BFS。dir[x][y] に「そのマスから次に進む方向」を入れる。
// 到達不可なら path 全体が -1 のまま。
vector<vector<int>> bfs_build_path(int sx, int sy, int gx, int gy) {
    int n = N_global;
    const int INF = 1001001001;
    vector<vector<int>> dist(n, vector<int>(n, INF));
    vector<vector<pair<int,int>>> from(n, vector<pair<int,int>>(n, {-1, -1}));
    queue<pair<int,int>> q;
    dist[sx][sy] = 0;
    q.emplace(sx, sy);

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        if (x == gx && y == gy) break;
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx4[d];
            int ny = y + dy4[d];
            if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
            if (!can_move(x, y, d)) continue;
            if (dist[nx][ny] != INF) continue;
            dist[nx][ny] = dist[x][y] + 1;
            from[nx][ny] = {x, y};
            q.emplace(nx, ny);
        }
    }

    vector<vector<int>> path(n, vector<int>(n, -1));
    if (dist[gx][gy] == INF) {
        return path; // 到達不可
    }

    int x = gx, y = gy;
    while (!(x == sx && y == sy)) {
        auto [px, py] = from[x][y];
        if (px == -1 && py == -1) break;
        int ddx = x - px;
        int ddy = y - py;
        int used_d = -1;
        for (int d = 0; d < 4; ++d) {
            if (dx4[d] == ddx && dy4[d] == ddy) {
                used_d = d;
                break;
            }
        }
        if (used_d == -1) break;
        path[px][py] = used_d;
        x = px;
        y = py;
    }
    return path;
}

// dir2d から1次元の方向列と通過座標列を復元
bool build_linear_from_dir2d(int sx, int sy, int gx, int gy,
                             const vector<vector<int>>& dir2d,
                             vector<int>& dirs_out,
                             vector<pair<int,int>>& pos_out) {
    int n = N_global;
    dirs_out.clear();
    pos_out.clear();
    pos_out.emplace_back(sx, sy);
    int x = sx, y = sy;
    int safety = n * n * 10;
    while (!(x == gx && y == gy)) {
        if (safety-- <= 0) return false;
        int d = dir2d[x][y];
        if (d < 0 || d > 3) return false;
        int nx = x + dx4[d];
        int ny = y + dy4[d];
        if (nx < 0 || nx >= n || ny < 0 || ny >= n) return false;
        if (!can_move(x, y, d)) return false;
        dirs_out.push_back(d);
        x = nx; y = ny;
        pos_out.emplace_back(x, y);
    }
    return true;
}

// 同じ方向の2連続・3連続に基づくローカルスコア
int calc_local_score(const vector<int>& dirs) {
    int n = (int)dirs.size();
    int score = 0;
    for (int i = 1; i < n; ++i) {
        if (dirs[i] == dirs[i-1]) score += 1;            // 2連
        if (i >= 2 && dirs[i] == dirs[i-1] && dirs[i] == dirs[i-2]) score += 2; // 3連
    }
    return score;
}

// 最短経路から +2 の寄り道候補を1本作る
bool build_detour_candidate(const vector<int>& base_dirs,
                            const vector<pair<int,int>>& base_pos,
                            vector<int>& detour_dirs) {
    int len = (int)base_dirs.size();
    int n = N_global;
    for (int t = 0; t < len; ++t) {
        int x = base_pos[t].first;
        int y = base_pos[t].second;
        for (int d = 0; d < 4; ++d) {
            int nx = x + dx4[d];
            int ny = y + dy4[d];
            if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
            if (!can_move(x, y, d)) continue;
            int d2 = (d + 2) & 3;
            if (!can_move(nx, ny, d2)) continue;
            // 寄り道: d, d2 で元の位置に戻る
            detour_dirs.clear();
            detour_dirs.reserve(len + 2);
            for (int i = 0; i < t; ++i) detour_dirs.push_back(base_dirs[i]);
            detour_dirs.push_back(d);
            detour_dirs.push_back(d2);
            for (int i = t; i < len; ++i) detour_dirs.push_back(base_dirs[i]);
            return true;
        }
    }
    return false;
}

struct PathCand {
    vector<int> dirs;
};

struct Interval {
    int l, r;
    int w;
    int B;
    int pid;
};

struct PatternInfo {
    int B;
    int code;
};

void solve() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, K;
    if (!(cin >> n >> K >> T_global)) {
        return;
    }
    N_global = n;
    K_global = K;

    v_wall.assign(n, string());
    h_wall.assign(max(0, n-1), string());
    for (int i = 0; i < n; ++i) cin >> v_wall[i];
    for (int i = 0; i < n-1; ++i) cin >> h_wall[i];

    gv.assign(K, {0, 0});
    for (int i = 0; i < K; ++i) {
        cin >> gv[i].first >> gv[i].second;
    }

    int S = K - 1;
    if (S <= 0) {
        // 動かない解
        int C = 1;
        int Q = 1;
        int M = 0;
        cout << C << " " << Q << " " << M << "\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << 0 << (j+1==n ? '\n' : ' ');
            }
        }
        return;
    }

    // 1. 各区間の候補パス生成
    vector<vector<PathCand>> cand(S);
    vector<int> base_len(S, 0);
    bool ok_all = true;

    for (int seg = 0; seg < S; ++seg) {
        int sx = gv[seg].first;
        int sy = gv[seg].second;
        int gx = gv[seg+1].first;
        int gy = gv[seg+1].second;

        auto dir2d = bfs_build_path(sx, sy, gx, gy);
        vector<int> dirs0;
        vector<pair<int,int>> pos0;
        if (!build_linear_from_dir2d(sx, sy, gx, gy, dir2d, dirs0, pos0)) {
            ok_all = false;
            break;
        }
        PathCand c0;
        c0.dirs = dirs0;
        cand[seg].push_back(c0);
        base_len[seg] = (int)dirs0.size();

        // 寄り道候補 (+2) を1本だけ試す
        vector<int> det;
        if (build_detour_candidate(dirs0, pos0, det)) {
            PathCand c1;
            c1.dirs = det;
            cand[seg].push_back(c1);
        }
        // 他の候補は作らない（最大2本）
    }

    if (!ok_all) {
        // 到達できない区間があった場合の単純出力
        int C = 1;
        int Q = 1;
        int M = 0;
        cout << C << " " << Q << " " << M << "\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << 0 << (j+1==n ? '\n' : ' ');
            }
        }
        return;
    }

    // 2. slack DP で P を決める
    ll X = 0;
    for (int i = 0; i < S; ++i) X += base_len[i];

    ll reserve = 0; // v1 ではループのための特別な予約はしない
    ll slackLimit = max(0LL, T_global - X - reserve);
    slackLimit = min(slackLimit, 200LL); // 上限を絞る
    int slackMax = (int)slackLimit;
    if (slackMax < 0) slackMax = 0;

    vector<vector<ll>> dp(S+1, vector<ll>(slackMax+1, (ll)-4e18));
    struct Parent {
        int prev_s;
        int k;
        bool used;
    };
    vector<vector<Parent>> parent(S+1, vector<Parent>(slackMax+1, Parent{-1,-1,false}));

    dp[0][0] = 0;

    // 前計算: 各候補のローカルスコアと Δlen
    vector<vector<int>> local_score(S);
    vector<vector<int>> delta_len(S);
    for (int i = 0; i < S; ++i) {
        int m = (int)cand[i].size();
        local_score[i].assign(m, 0);
        delta_len[i].assign(m, 0);
        for (int k = 0; k < m; ++k) {
            local_score[i][k] = calc_local_score(cand[i][k].dirs);
            delta_len[i][k] = (int)cand[i][k].dirs.size() - base_len[i];
            if (delta_len[i][k] < 0) delta_len[i][k] = 0; // 念のため
        }
    }

    for (int i = 0; i < S; ++i) {
        for (int s = 0; s <= slackMax; ++s) {
            if (dp[i][s] <= (ll)-3e18) continue;
            int m = (int)cand[i].size();
            for (int k = 0; k < m; ++k) {
                int d = delta_len[i][k];
                if (s + d > slackMax) continue;
                ll val = dp[i][s] + local_score[i][k];
                if (val > dp[i+1][s+d]) {
                    dp[i+1][s+d] = val;
                    parent[i+1][s+d] = Parent{s, k, true};
                }
            }
        }
    }

    // 最終スコア最大の s を取り、その中で s が最大のものを選ぶ
    ll bestVal = (ll)-4e18;
    int best_s = 0;
    for (int s = 0; s <= slackMax; ++s) {
        if (dp[S][s] > bestVal) {
            bestVal = dp[S][s];
            best_s = s;
        } else if (dp[S][s] == bestVal && dp[S][s] > (ll)-3e18) {
            if (s > best_s) best_s = s;
        }
    }

    vector<int> choiceSeg(S, 0);
    if (bestVal <= (ll)-3e18) {
        // DP が全て無効なら、全区間候補0
        for (int i = 0; i < S; ++i) choiceSeg[i] = 0;
    } else {
        int s = best_s;
        for (int i = S; i > 0; --i) {
            Parent p = parent[i][s];
            if (!p.used) {
                // ありえないが保険で候補0
                choiceSeg[i-1] = 0;
            } else {
                choiceSeg[i-1] = p.k;
                s = p.prev_s;
            }
        }
    }

    // P 構築
    vector<int> P;
    P.reserve((size_t)(X + best_s + 10));
    for (int i = 0; i < S; ++i) {
        int k = choiceSeg[i];
        const auto& dirs = cand[i][k].dirs;
        P.insert(P.end(), dirs.begin(), dirs.end());
    }
    int L = (int)P.size();

    // 3. パターン抽出と区間DP
    vector<Interval> intervals;
    vector<PatternInfo> patterns;

    if (L >= 3) {
        const int B_min = 3;
        const int B_max = 5;
        for (int B = B_min; B <= B_max; ++B) {
            if (L < B) continue;
            unordered_map<int, vector<int>> posMap;
            posMap.reserve(L*2);

            // code は base-4 の整数
            int code = 0;
            int pow4 = 1;
            for (int i = 0; i < B; ++i) {
                pow4 *= 4;
            }

            // 最初の B 個
            code = 0;
            for (int i = 0; i < B; ++i) {
                code = code * 4 + P[i];
            }
            posMap[code].push_back(0);

            for (int i = 1; i <= L - B; ++i) {
                // ローリング: 前の上位桁を落として新しい1桁を追加
                // ただし pow4 = 4^B なので、code % (pow4/4) を使う
                int base = pow4 / 4;
                code = code % base;
                code = code * 4 + P[i + B - 1];
                posMap[code].push_back(i);
            }

            for (auto &kv : posMap) {
                const auto &vec = kv.second;
                int o = (int)vec.size();
                if (o < 3) continue; // 頻出パターンのみ
                int pid = (int)patterns.size();
                PatternInfo info;
                info.B = B;
                info.code = kv.first;
                patterns.push_back(info);
                int w = B - 2; // 1 occurrence あたりの近似 gain
                for (int pos : vec) {
                    Interval itv;
                    itv.l = pos;
                    itv.r = pos + B;
                    itv.w = w;
                    itv.B = B;
                    itv.pid = pid;
                    intervals.push_back(itv);
                }
            }
        }
    }

    vector<bool> usedInterval;
    if (!intervals.empty()) {
        // r 昇順でソート
        sort(intervals.begin(), intervals.end(),
             [](const Interval& a, const Interval& b) {
                 if (a.r != b.r) return a.r < b.r;
                 return a.l < b.l;
             });
        int M = (int)intervals.size();
        vector<int> lArr(M), rArr(M);
        for (int j = 0; j < M; ++j) {
            lArr[j] = intervals[j].l;
            rArr[j] = intervals[j].r;
        }
        // prev[j] 計算
        vector<int> prevIdx(M, -1);
        for (int j = 0; j < M; ++j) {
            int l = lArr[j];
            int i = upper_bound(rArr.begin(), rArr.begin() + j, l) - rArr.begin();
            prevIdx[j] = i - 1; // -1 の場合もあり
        }

        vector<ll> dp2(M+1, 0);
        vector<int> choose(M+1, 0); // 1: j-1 を選ぶ

        for (int j = 1; j <= M; ++j) {
            // interval index = j-1
            ll opt1 = dp2[j-1];
            int pj = prevIdx[j-1];
            ll opt2 = intervals[j-1].w + (pj >= 0 ? dp2[pj+1] : 0);
            if (opt2 > opt1) {
                dp2[j] = opt2;
                choose[j] = 1;
            } else {
                dp2[j] = opt1;
                choose[j] = 0;
            }
        }

        usedInterval.assign(M, false);
        int j = M;
        while (j > 0) {
            if (choose[j] == 1) {
                usedInterval[j-1] = true;
                int pj = prevIdx[j-1];
                j = (pj >= 0 ? pj+1 : 0);
            } else {
                --j;
            }
        }
    }

    // 4. 状態割り当て（v1: S のみ使用）
    // internalTag[t] = (pid, offset) if ブロック内部
    vector<int> internalPid(L+1, -1);
    vector<int> internalOff(L+1, 0);

    if (!intervals.empty()) {
        int M = (int)intervals.size();
        for (int j = 0; j < M; ++j) {
            if (!usedInterval[j]) continue;
            const Interval &inv = intervals[j];
            int B = inv.B;
            int pid = inv.pid;
            int l = inv.l;
            // 内部 t : l+1 .. l+B-1
            for (int off = 1; off <= B-1; ++off) {
                int t = l + off;
                if (t >= 0 && t <= L) {
                    // t==L は実際には来ない(B>=3, l<=L-B)
                    if (internalPid[t] != -1) {
                        // 非重なり DP のはずなので基本起こらない
                        // 起きたら共有は諦めて上書きせずスキップ
                        continue;
                    }
                    internalPid[t] = pid;
                    internalOff[t] = off;
                }
            }
        }
    }

    int numPatterns = (int)patterns.size();
    vector<vector<int>> sharedState(numPatterns); // sharedState[pid][off]

    vector<int> stateAtTime(L+1, -1);
    int nextQ = 0;

    // t=0 は必ず内部ではないので、0番状態にする
    stateAtTime[0] = nextQ++;
    for (int t = 1; t <= L; ++t) {
        if (internalPid[t] == -1) {
            // 個別状態
            stateAtTime[t] = nextQ++;
        } else {
            int pid = internalPid[t];
            int off = internalOff[t]; // 1..B-1
            if (pid < 0 || pid >= numPatterns) {
                stateAtTime[t] = nextQ++;
                continue;
            }
            int B = patterns[pid].B;
            if (off < 1 || off >= B) {
                stateAtTime[t] = nextQ++;
                continue;
            }
            if ((int)sharedState[pid].size() < B) {
                sharedState[pid].assign(B, -1);
            }
            if (sharedState[pid][off] == -1) {
                sharedState[pid][off] = nextQ++;
            }
            stateAtTime[t] = sharedState[pid][off];
        }
    }

    int Q_final = nextQ;
    int C_final = 1;

    // 5. 遷移規則構築（P 本体）
    vector<T5> rules;
    rules.reserve(L + 10);

    int c = 0; // 色は常に0
    int a = 0; // 書き換えなし

    for (int t = 0; t < L; ++t) {
        int q = stateAtTime[t];
        int s = stateAtTime[t+1];
        char dch = dc4[P[t]];
        rules.emplace_back(c, q, a, s, dch);
    }

    // 6. ループ構築（可能なら2ステップ往復）
    vector<pair<int,int>> pos(L+1);
    pos[0] = gv[0];
    bool pos_ok = true;
    for (int t = 0; t < L; ++t) {
        int x = pos[t].first;
        int y = pos[t].second;
        int d = P[t];
        if (!can_move(x, y, d)) {
            pos_ok = false;
            break;
        }
        int nx = x + dx4[d];
        int ny = y + dy4[d];
        pos[t+1] = {nx, ny};
    }
    if (!pos_ok) {
        // 位置の復元に失敗した場合、ループは諦める
    } else {
        int x_end = pos[L].first;
        int y_end = pos[L].second;
        int q_end = stateAtTime[L];
        int loopState = -1;
        for (int d = 0; d < 4; ++d) {
            if (!can_move(x_end, y_end, d)) continue;
            int nx = x_end + dx4[d];
            int ny = y_end + dy4[d];
            int d2 = (d + 2) & 3;
            if (!can_move(nx, ny, d2)) continue;
            loopState = nextQ++;
            Q_final = nextQ;
            char dch1 = dc4[d];
            char dch2 = dc4[d2];
            rules.emplace_back(c, q_end, a, loopState, dch1);
            rules.emplace_back(c, loopState, a, q_end, dch2);
            break;
        }
    }

    // ルール重複削除
    sort(rules.begin(), rules.end());
    rules.erase(unique(rules.begin(), rules.end()), rules.end());
    int M_final = (int)rules.size();

    // 出力
    cout << C_final << " " << Q_final << " " << M_final << "\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << 0 << (j+1==n ? '\n' : ' ');
        }
    }
    for (auto &tp : rules) {
        int qc, qq, qa, qs;
        char dch;
        tie(qc, qq, qa, qs, dch) = tp;
        cout << qc << " " << qq << " " << qa << " " << qs << " " << dch << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
