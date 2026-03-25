#include <bits/stdc++.h>
using namespace std;

// 方向: 0:U, 1:D, 2:L, 3:R
const int DX[4] = {-1, 1, 0, 0};
const int DY[4] = {0, 0, -1, 1};
const char DIR_CHAR[4] = {'U', 'D', 'L', 'R'};

struct Pos {
    int x, y;
};

struct EvalResult {
    double score;
    int turns;
    int segments;
    int blockRepeats;
    int L;
};

// メイン
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    long long T;
    if (!(cin >> N >> K >> T)) {
        return 0;
    }

    // v[i][j]: (i,j) - (i,j+1) の間の壁 (0:なし, 1:あり)
    vector<string> v(N);
    for (int i = 0; i < N; ++i) cin >> v[i];

    // h[i][j]: (i,j) - (i+1,j) の間の壁 (0:なし, 1:あり)
    vector<string> h(N - 1);
    for (int i = 0; i < N - 1; ++i) cin >> h[i];

    vector<Pos> dest(K);
    for (int i = 0; i < K; ++i) {
        cin >> dest[i].x >> dest[i].y;
    }

    auto in_bounds = [&](int x, int y) -> bool {
        return 0 <= x && x < N && 0 <= y && y < N;
    };

    auto can_move = [&](int x, int y, int dir) -> bool {
        int nx = x + DX[dir];
        int ny = y + DY[dir];
        if (!in_bounds(nx, ny)) return false;
        if (dir == 0) { // U
            if (x - 1 < 0) return false;
            return h[x - 1][y] == '0';
        } else if (dir == 1) { // D
            if (x >= N - 1) return false;
            return h[x][y] == '0';
        } else if (dir == 2) { // L
            if (y - 1 < 0) return false;
            return v[x][y - 1] == '0';
        } else { // R
            if (y >= N - 1) return false;
            return v[x][y] == '0';
        }
    };

    // s -> g の BFS で方向列を返す
    auto bfs_path = [&](Pos s, Pos g) -> vector<int> {
        int SZ = N * N;
        const int INF = (int)1e9;
        vector<int> dist(SZ, INF);
        vector<int> prevDir(SZ, -1);
        vector<int> prevIdx(SZ, -1);
        queue<int> que;

        int sId = s.x * N + s.y;
        int gId = g.x * N + g.y;
        dist[sId] = 0;
        que.push(sId);

        while (!que.empty()) {
            int id = que.front();
            que.pop();
            if (id == gId) break;
            int x = id / N;
            int y = id % N;
            for (int d = 0; d < 4; ++d) {
                if (!can_move(x, y, d)) continue;
                int nx = x + DX[d];
                int ny = y + DY[d];
                int nid = nx * N + ny;
                if (dist[nid] > dist[id] + 1) {
                    dist[nid] = dist[id] + 1;
                    prevDir[nid] = d;
                    prevIdx[nid] = id;
                    que.push(nid);
                }
            }
        }

        vector<int> res;
        if (dist[gId] == INF) {
            cerr << "WARNING: BFS unreachable from (" << s.x << "," << s.y
                 << ") to (" << g.x << "," << g.y << ")\n";
            return res;
        }
        int cur = gId;
        while (cur != sId) {
            int d = prevDir[cur];
            res.push_back(d);
            cur = prevIdx[cur];
        }
        reverse(res.begin(), res.end());
        return res;
    };

    // --- 通行帯スコアの計算 ---
    struct LineScore {
        int idx;
        long long score;
    };
    vector<LineScore> rowScore, colScore;
    rowScore.reserve(N);
    colScore.reserve(N);

    // 行方向
    {
        double alpha = 3.0, beta = 1.0, gamma = 2.0;
        for (int i = 0; i < N; ++i) {
            int longest = 0, sumLen = 0, comps = 0;
            int curLen = 0;
            for (int j = 0; j < N - 1; ++j) {
                if (v[i][j] == '0') {
                    curLen++;
                } else {
                    if (curLen > 0) {
                        comps++;
                        sumLen += curLen;
                        longest = max(longest, curLen);
                        curLen = 0;
                    }
                }
            }
            if (curLen > 0) {
                comps++;
                sumLen += curLen;
                longest = max(longest, curLen);
            }
            long long sc = (long long)llround(alpha * longest + beta * sumLen - gamma * comps);
            rowScore.push_back({i, sc});
        }
    }
    // 列方向
    {
        double alpha = 3.0, beta = 1.0, gamma = 2.0;
        for (int j = 0; j < N; ++j) {
            int longest = 0, sumLen = 0, comps = 0;
            int curLen = 0;
            for (int i = 0; i < N - 1; ++i) {
                if (h[i][j] == '0') {
                    curLen++;
                } else {
                    if (curLen > 0) {
                        comps++;
                        sumLen += curLen;
                        longest = max(longest, curLen);
                        curLen = 0;
                    }
                }
            }
            if (curLen > 0) {
                comps++;
                sumLen += curLen;
                longest = max(longest, curLen);
            }
            long long sc = (long long)llround(alpha * longest + beta * sumLen - gamma * comps);
            colScore.push_back({j, sc});
        }
    }

    sort(rowScore.begin(), rowScore.end(),
         [](const LineScore &a, const LineScore &b) { return a.score > b.score; });
    sort(colScore.begin(), colScore.end(),
         [](const LineScore &a, const LineScore &b) { return a.score > b.score; });

    int R_row = min(3, N);
    int R_col = min(3, N);
    vector<int> corridorRows, corridorCols;
    for (int k = 0; k < R_row; ++k) {
        if (rowScore[k].score > 0) corridorRows.push_back(rowScore[k].idx);
    }
    for (int k = 0; k < R_col; ++k) {
        if (colScore[k].score > 0) corridorCols.push_back(colScore[k].idx);
    }

    cerr << "N=" << N << " K=" << K << " T=" << T << "\n";
    cerr << "Selected corridor rows: ";
    for (int r : corridorRows) cerr << r << " ";
    cerr << "\nSelected corridor cols: ";
    for (int c : corridorCols) cerr << c << " ";
    cerr << "\n";

    // --- 各セグメントの最短距離 dist_i と X を計算 ---
    int segCount = K - 1;
    vector<int> distSeg(segCount, 0);
    long long X = 0;
    for (int i = 0; i < segCount; ++i) {
        vector<int> p = bfs_path(dest[i], dest[i + 1]);
        int dlen = (int)p.size();
        distSeg[i] = dlen;
        X += dlen;
    }
    if (X > T) {
        // 問題文上ありえないが、安全のため
        cerr << "WARNING: X > T, X=" << X << " T=" << T << "\n";
    }
    long long slackTotal = max(0LL, T - X);
    cerr << "X=" << X << " slackTotal=" << slackTotal << "\n";

    // --- 各セグメントの候補経路生成 ---
    const int MAX_CAND = 5;
    vector<vector<vector<int>>> cand(segCount); // cand[i][k] = k番目候補の方向列

    for (int i = 0; i < segCount; ++i) {
        Pos s = dest[i];
        Pos g = dest[i + 1];
        int base = distSeg[i];

        vector<vector<int>> cands;

        // A: 純最短
        vector<int> shortest = bfs_path(s, g);
        if (shortest.empty() && !(s.x == g.x && s.y == g.y)) {
            cerr << "ERROR: empty shortest path at segment " << i << "\n";
        } else {
            cands.push_back(shortest);
        }

        // B: 通行帯1本経由 (row)
        for (int r : corridorRows) {
            if ((int)cands.size() >= MAX_CAND) break;
            Pos w = {r, g.y};
            vector<int> p1 = bfs_path(s, w);
            vector<int> p2 = bfs_path(w, g);
            if (p1.empty() && !(s.x == w.x && s.y == w.y)) continue;
            if (p2.empty() && !(w.x == g.x && w.y == g.y)) continue;
            vector<int> merged;
            merged.reserve(p1.size() + p2.size());
            merged.insert(merged.end(), p1.begin(), p1.end());
            merged.insert(merged.end(), p2.begin(), p2.end());
            int len = (int)merged.size();
            if (len <= base) { // 最短長以内に制限
                cands.push_back(merged);
            }
        }

        // B: 通行帯1本経由 (col)
        for (int c : corridorCols) {
            if ((int)cands.size() >= MAX_CAND) break;
            Pos w = {g.x, c};
            vector<int> p1 = bfs_path(s, w);
            vector<int> p2 = bfs_path(w, g);
            if (p1.empty() && !(s.x == w.x && s.y == w.y)) continue;
            if (p2.empty() && !(w.x == g.x && w.y == g.y)) continue;
            vector<int> merged;
            merged.reserve(p1.size() + p2.size());
            merged.insert(merged.end(), p1.begin(), p1.end());
            merged.insert(merged.end(), p2.begin(), p2.end());
            int len = (int)merged.size();
            if (len <= base) {
                cands.push_back(merged);
            }
        }

        if (cands.empty()) {
            cands.push_back(shortest);
        }
        if ((int)cands.size() > MAX_CAND) {
            cands.resize(MAX_CAND);
        }
        cand[i] = cands;
        cerr << "Segment " << i << ": dist=" << distSeg[i]
             << " cand_count=" << (int)cands.size() << "\n";
    }

    // --- 評価関数: ブロック重複を含む ---
    auto eval_path = [&](const vector<int> &dirs) -> EvalResult {
        EvalResult res;
        res.L = (int)dirs.size();
        if ((long long)res.L > T) {
            res.score = 1e18;
            res.turns = 0;
            res.segments = 0;
            res.blockRepeats = 0;
            return res;
        }

        int L = res.L;

        // 曲がり回数
        int turns = 0;
        for (int i = 0; i + 1 < L; ++i) {
            if (dirs[i] != dirs[i + 1]) turns++;
        }

        // 直線区間数
        int segments = 0;
        if (L > 0) {
            segments = 1;
            for (int i = 1; i < L; ++i) {
                if (dirs[i] != dirs[i - 1]) segments++;
            }
        }

        // ブロックパターン（長さ4）の重複カウント
        const int B = 4;
        static int freq[256];
        memset(freq, 0, sizeof(freq));
        for (int i = 0; i + B <= L; ++i) {
            int id = 0;
            // 4進数で encode
            for (int k = 0; k < B; ++k) {
                int d = dirs[i + k];
                if (d < 0 || d >= 4) d = 0;
                id = id * 4 + d;
            }
            freq[id]++;
        }
        int blockRepeats = 0;
        int blockKinds = 0;
        for (int id = 0; id < 256; ++id) {
            if (freq[id] > 0) {
                blockKinds++;
                if (freq[id] > 1) {
                    blockRepeats += (freq[id] - 1);
                }
            }
        }

        // スコア: 小さいほど良い
        double a = 1.0;
        double b = 0.1;
        double lambda = 0.5; // ブロック重複ボーナス
        double score = a * (double)turns + b * (double)segments - lambda * (double)blockRepeats;

        res.score = score;
        res.turns = turns;
        res.segments = segments;
        res.blockRepeats = blockRepeats;

        // デバッグ用推定 Q_est (L+1 からブロックで少し減ると仮定)
        int Q_naive = L + 1;
        int Q_est = Q_naive - blockRepeats; // 非厳密だが目安
        cerr << "[eval] L=" << L
             << " turns=" << turns
             << " segments=" << segments
             << " blockKinds=" << blockKinds
             << " blockRepeats=" << blockRepeats
             << " Q_naive=" << Q_naive
             << " Q_est~" << Q_est
             << " score=" << score << "\n";

        return res;
    };

    // --- 初期解 (各セグメント最短候補) ---
    vector<int> choice(segCount, 0);
    vector<int> P;
    P.reserve((size_t)T);
    for (int i = 0; i < segCount; ++i) {
        const auto &dirs = cand[i][choice[i]];
        P.insert(P.end(), dirs.begin(), dirs.end());
    }
    if ((long long)P.size() > T) {
        P.resize((size_t)T);
    }
    EvalResult currEval = eval_path(P);
    vector<int> bestChoice = choice;
    vector<int> bestP = P;
    EvalResult bestEval = currEval;

    cerr << "Initial path length L=" << P.size()
         << " score=" << currEval.score
         << " turns=" << currEval.turns
         << " segments=" << currEval.segments
         << " blockRepeats=" << currEval.blockRepeats
         << "\n";

    // --- 山登り法 ---
    mt19937_64 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());
    const double timeLimit = 1.8; // 秒目安
    auto startTime = chrono::steady_clock::now();

    int ITER_MAX = 5000;
    for (int it = 0; it < ITER_MAX; ++it) {
        if (it % 128 == 0) {
            auto now = chrono::steady_clock::now();
            double elapsed = chrono::duration<double>(now - startTime).count();
            if (elapsed > timeLimit) break;
        }
        // cand数>1 のセグメントからランダム選択
        int tries = 0;
        int segIdx = -1;
        while (tries < 50) {
            int i = (int)(rng() % max(1, segCount));
            if ((int)cand[i].size() > 1) {
                segIdx = i;
                break;
            }
            tries++;
        }
        if (segIdx == -1) continue;

        int oldChoice = choice[segIdx];
        int newChoice = oldChoice;
        int candCount = (int)cand[segIdx].size();
        if (candCount <= 1) continue;

        for (int loop = 0; loop < 5; ++loop) {
            int cidx = (int)(rng() % candCount);
            if (cidx != oldChoice) {
                newChoice = cidx;
                break;
            }
        }
        if (newChoice == oldChoice) continue;

        // 新しい解の構築（全体を再構築）
        vector<int> newChoiceVec = choice;
        newChoiceVec[segIdx] = newChoice;
        vector<int> newP;
        newP.reserve((size_t)T);
        for (int i = 0; i < segCount; ++i) {
            const auto &dirs = cand[i][newChoiceVec[i]];
            newP.insert(newP.end(), dirs.begin(), dirs.end());
        }
        if ((long long)newP.size() > T) {
            // 今の設計上起きないはずだが保険
            continue;
        }
        EvalResult newEval = eval_path(newP);
        if (newEval.score < currEval.score) {
            choice.swap(newChoiceVec);
            P.swap(newP);
            currEval = newEval;
            if (currEval.score < bestEval.score) {
                bestEval = currEval;
                bestChoice = choice;
                bestP = P;
            }
        }
    }

    cerr << "Final best length L=" << bestP.size()
         << " bestScore=" << bestEval.score
         << " turns=" << bestEval.turns
         << " segments=" << bestEval.segments
         << " blockRepeats=" << bestEval.blockRepeats
         << "\n";

    // --- オートマトン出力 (まだベースライン: 1色 + 1ステップ1状態) ---
    long long L = (long long)bestP.size();
    int C = 1;
    long long Q_ll = L + 1;
    long long Q_max = 1LL * N * N * N * N;
    if (Q_ll > Q_max) Q_ll = Q_max;
    int Q = (int)Q_ll;
    int M = (int)min(L, (long long)Q - 1);

    cout << C << " " << Q << " " << M << "\n";
    // 初期盤面色: 全マス0
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << 0 << (j + 1 == N ? '\n' : ' ');
        }
    }
    // 遷移規則: (c=0, q=i) -> (c=0, q=i+1, dir=bestP[i])
    for (int i = 0; i < M; ++i) {
        int c = 0;
        int qstate = i;
        int newColor = 0;
        int newState = i + 1;
        char dch = DIR_CHAR[bestP[i]];
        cout << c << " " << qstate << " " << newColor << " " << newState << " " << dch << "\n";
    }

    return 0;
}
