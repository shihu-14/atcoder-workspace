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

const int R = 10;
const int N = 10;
const int DEP_CAP = 15;
const int SID_CAP = 20;
const long long TIME_LIMIT_MS = 1880;

struct TimeKeeper {
    chrono::high_resolution_clock::time_point st;

    TimeKeeper() {
        st = chrono::high_resolution_clock::now();
    }

    long long elapsedMs() const {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now - st).count();
    }

    bool timeUp(long long marginMs = 0) const {
        return elapsedMs() >= TIME_LIMIT_MS - marginMs;
    }
};

struct Move {
    int type;
    int i;
    int j;
    int k;
};

struct Candidate {
    Move m;
    long long weight;
};

struct TurnNode {
    bool valid = false;
    long long weight = LLONG_MIN / 4;
    int kSum = 0;
    int cnt = 0;
    vector<Move> turn;
};

struct Param {
    long long ck = 1000000;
    long long coff = 0;
    long long cgroup = 0;
    long long cdiag = 0;
    long long cabs = 0;
    long long cnoise = 0;
    unsigned long long seed = 1;
};

struct Stats {
    int classifyTurns = 0;
    int classifyMoves = 0;
    int movedCars = 0;
    int singleTurns = 0;
    int k1Moves = 0;
    int kMaxMoves = 0;

    int offsetOk = 0;
    int offsetTurns = 0;
    int offsetMoves = 0;
    int offsetPlaced = 0;
    int offsetExposeSid = 0;
    int offsetExposeDep = 0;
    int offsetFlush = 0;
    int offsetDeadTurns = 0;
    int offsetMaxPlacedTurn = 0;
    int offsetAvgPlacedX100 = 0;
    int offsetMaxDirty = 0;

    int splitMask = -1;
    int splitPredTurns = INF;
    int splitPredMoves = INF;
    int splitBatchTurns1 = INF;
    int splitBatchTurns2 = INF;
};

struct Solution {
    bool ok = false;
    int trialId = -1;
    string method;

    vector<vector<Move>> turns;
    int totalMoves = 0;

    int maxDepLen = 10;
    int maxSidLen = 0;

    array<int, 5> phaseTurns{};
    array<int, 5> phaseMoves{};

    Stats stats;
    string validationMsg;
};

int groupOf(int x) {
    return x / 10;
}

int offsetOf(int x) {
    return x % 10;
}

unsigned long long splitmix64(unsigned long long x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

int findDeque(const deque<int> &dq, int target) {
    for (int i = 0; i < (int)dq.size(); ++i) {
        if (dq[i] == target) return i;
    }
    return -1;
}

bool validTurnMoves(const vector<Move> &turn, string *err = nullptr) {
    if (turn.empty() || (int)turn.size() > R) {
        if (err) *err = "invalid K";
        return false;
    }

    vector<int> usedDep(R, 0);
    vector<int> usedSid(R, 0);

    for (int a = 0; a < (int)turn.size(); ++a) {
        const Move &ma = turn[a];

        if (ma.type != 0 && ma.type != 1) {
            if (err) *err = "invalid type";
            return false;
        }

        if (ma.i < 0 || ma.i >= R || ma.j < 0 || ma.j >= R || ma.k <= 0) {
            if (err) *err = "invalid parameter";
            return false;
        }

        if (usedDep[ma.i] || usedSid[ma.j]) {
            if (err) *err = "line reused";
            return false;
        }

        usedDep[ma.i] = 1;
        usedSid[ma.j] = 1;

        for (int b = 0; b < a; ++b) {
            const Move &mb = turn[b];

            if (mb.i < ma.i && !(mb.j < ma.j)) {
                if (err) *err = "crossing";
                return false;
            }

            if (ma.i < mb.i && !(ma.j < mb.j)) {
                if (err) *err = "crossing";
                return false;
            }
        }
    }

    return true;
}

bool applyMoveState(vector<deque<int>> &dep, vector<deque<int>> &sid, const Move &m, string *err = nullptr) {
    if (m.type != 0 && m.type != 1) {
        if (err) *err = "invalid type";
        return false;
    }

    if (m.i < 0 || m.i >= R || m.j < 0 || m.j >= R || m.k <= 0) {
        if (err) *err = "invalid parameter";
        return false;
    }

    if (m.type == 0) {
        if ((int)dep[m.i].size() < m.k) {
            if (err) *err = "departure underflow";
            return false;
        }

        if ((int)sid[m.j].size() + m.k > SID_CAP) {
            if (err) *err = "siding overflow";
            return false;
        }

        vector<int> block(m.k);

        for (int p = m.k - 1; p >= 0; --p) {
            block[p] = dep[m.i].back();
            dep[m.i].pop_back();
        }

        for (int p = m.k - 1; p >= 0; --p) {
            sid[m.j].push_front(block[p]);
        }
    } else {
        if ((int)sid[m.j].size() < m.k) {
            if (err) *err = "siding underflow";
            return false;
        }

        if ((int)dep[m.i].size() + m.k > DEP_CAP) {
            if (err) *err = "departure overflow";
            return false;
        }

        vector<int> block(m.k);

        for (int p = 0; p < m.k; ++p) {
            block[p] = sid[m.j].front();
            sid[m.j].pop_front();
        }

        for (int p = 0; p < m.k; ++p) {
            dep[m.i].push_back(block[p]);
        }
    }

    return true;
}

vector<Move> sortTurn(vector<Move> turn) {
    sort(turn.begin(), turn.end(), [](const Move &a, const Move &b) {
        if (a.i != b.i) return a.i < b.i;
        return a.j < b.j;
    });
    return turn;
}

bool betterTurnNode(const TurnNode &a, const TurnNode &b) {
    if (!a.valid) return false;
    if (!b.valid) return true;
    if (a.weight != b.weight) return a.weight > b.weight;
    if (a.kSum != b.kSum) return a.kSum > b.kSum;
    if (a.cnt != b.cnt) return a.cnt > b.cnt;
    return false;
}

vector<Move> chooseBestTurn(const vector<Candidate> &cand) {
    vector<vector<Candidate>> byI(R);

    for (const Candidate &c : cand) {
        byI[c.m.i].push_back(c);
    }

    vector<TurnNode> dp(R + 1), ndp(R + 1);
    dp[0].valid = true;
    dp[0].weight = 0;

    for (int i = 0; i < R; ++i) {
        ndp = dp;

        for (int last = -1; last < R; ++last) {
            int idx = last + 1;

            if (!dp[idx].valid) continue;

            for (const Candidate &c : byI[i]) {
                if (c.m.j <= last) continue;

                TurnNode nx = dp[idx];
                nx.valid = true;
                nx.weight += c.weight;
                nx.kSum += c.m.k;
                nx.cnt += 1;
                nx.turn.push_back(c.m);

                int nidx = c.m.j + 1;

                if (betterTurnNode(nx, ndp[nidx])) {
                    ndp[nidx] = nx;
                }
            }
        }

        dp.swap(ndp);
    }

    TurnNode best;

    for (int idx = 0; idx <= R; ++idx) {
        if (!dp[idx].valid || dp[idx].turn.empty()) continue;

        if (betterTurnNode(dp[idx], best)) {
            best = dp[idx];
        }
    }

    if (!best.valid) return {};
    return sortTurn(best.turn);
}

bool validateTurns(const vector<deque<int>> &initial, const vector<vector<Move>> &turns, string &msg) {
    if ((int)initial.size() != R) {
        msg = "invalid initial size";
        return false;
    }

    vector<deque<int>> dep = initial;
    vector<deque<int>> sid(R);

    if ((int)turns.size() > 4000) {
        msg = "too many turns";
        return false;
    }

    for (int t = 0; t < (int)turns.size(); ++t) {
        string e;

        if (!validTurnMoves(turns[t], &e)) {
            msg = e + " at turn " + to_string(t);
            return false;
        }

        for (const Move &m : turns[t]) {
            if (!applyMoveState(dep, sid, m, &e)) {
                msg = e + " at turn " + to_string(t);
                return false;
            }
        }
    }

    for (int r = 0; r < R; ++r) {
        if ((int)dep[r].size() != N) {
            msg = "wrong final departure size";
            return false;
        }

        for (int c = 0; c < N; ++c) {
            if (dep[r][c] != N * r + c) {
                msg = "wrong final order";
                return false;
            }
        }

        if (!sid[r].empty()) {
            msg = "siding not empty";
            return false;
        }
    }

    msg = "ok";
    return true;
}

bool addTurn(
    vector<deque<int>> &dep,
    vector<deque<int>> &sid,
    Solution &res,
    vector<Move> turn,
    int phase
) {
    if (turn.empty()) return true;

    turn = sortTurn(turn);

    string e;

    if (!validTurnMoves(turn, &e)) {
        res.validationMsg = "invalid turn: " + e;
        return false;
    }

    for (const Move &m : turn) {
        if (!applyMoveState(dep, sid, m, &e)) {
            res.validationMsg = "apply failed: " + e;
            return false;
        }
    }

    res.turns.push_back(turn);
    res.phaseTurns[phase]++;
    res.phaseMoves[phase] += (int)turn.size();
    res.totalMoves += (int)turn.size();

    for (int i = 0; i < R; ++i) {
        chmax(res.maxDepLen, (int)dep[i].size());
        chmax(res.maxSidLen, (int)sid[i].size());
    }

    return true;
}

int dirtyLen(const vector<deque<int>> &dep, const vector<int> &placed, int i) {
    return max(0, (int)dep[i].size() - placed[i]);
}

int totalDirty(const vector<deque<int>> &dep, const vector<deque<int>> &sid, const vector<int> &placed) {
    int s = 0;

    for (int i = 0; i < R; ++i) {
        s += dirtyLen(dep, placed, i);
    }

    for (int j = 0; j < R; ++j) {
        s += (int)sid[j].size();
    }

    return s;
}

vector<Candidate> makeOffsetCandidates(
    const vector<deque<int>> &dep,
    const vector<deque<int>> &sid,
    const vector<int> &placed,
    int off
) {
    vector<Candidate> cand;

    for (int j = 0; j < R; ++j) {
        if (sid[j].empty()) continue;

        int x = sid[j].front();
        int g = groupOf(x);

        if (placed[g] == off &&
            offsetOf(x) == off &&
            x == N * g + off &&
            (int)dep[g].size() == placed[g]) {
            long long w = 9000000000000LL;
            w += (long long)(10 - abs(g - j)) * 10000000LL;
            cand.push_back({Move{1, g, j, 1}, w});
        }
    }

    for (int j = 0; j < R; ++j) {
        for (int pos = 1; pos < (int)sid[j].size(); ++pos) {
            int x = sid[j][pos];
            int g = groupOf(x);

            if (placed[g] != off) continue;
            if (x != N * g + off) continue;

            for (int t = 0; t < R; ++t) {
                if (t == g) continue;

                int cap = DEP_CAP - (int)dep[t].size();

                if (cap <= 0) continue;

                int k = min(pos, cap);

                if (k <= 0) continue;

                long long w = 4000000000000LL;
                w -= (long long)pos * 50000000LL;
                w += (long long)k * 10000000LL;
                w -= (long long)abs(t - j) * 1000000LL;

                cand.push_back({Move{1, t, j, k}, w});
            }

            break;
        }
    }

    for (int i = 0; i < R; ++i) {
        int dl = dirtyLen(dep, placed, i);

        if (dl <= 0) continue;

        int start = placed[i];

        for (int p = start; p < (int)dep[i].size(); ++p) {
            int x = dep[i][p];
            int g = groupOf(x);

            if (placed[g] != off) continue;
            if (x != N * g + off) continue;

            int above = (int)dep[i].size() - 1 - p;

            if (above == 0) {
                for (int j = 0; j < R; ++j) {
                    if ((int)sid[j].size() >= SID_CAP) continue;

                    long long w = 5000000000000LL;
                    w -= (long long)abs(i - j) * 1000000LL;

                    cand.push_back({Move{0, i, j, 1}, w});
                }
            } else {
                for (int j = 0; j < R; ++j) {
                    int cap = SID_CAP - (int)sid[j].size();

                    if (cap <= 0) continue;

                    int k = min(above, cap);

                    long long w = 2500000000000LL;
                    w += (long long)k * 50000000LL;
                    w -= (long long)abs(i - j) * 1000000LL;

                    cand.push_back({Move{0, i, j, k}, w});
                }
            }

            break;
        }
    }

    for (int i = 0; i < R; ++i) {
        int dl = dirtyLen(dep, placed, i);

        if (dl <= 0) continue;

        for (int j = 0; j < R; ++j) {
            int cap = SID_CAP - (int)sid[j].size();

            if (cap <= 0) continue;

            int k = min(dl, cap);

            long long w = 500000000000LL;
            w += (long long)k * 100000000LL;
            w -= (long long)abs(i - j) * 1000000LL;

            cand.push_back({Move{0, i, j, k}, w});
        }
    }

    return cand;
}

Solution buildOffsetPipeline(const vector<deque<int>> &initial) {
    Solution res;
    res.method = "offset_pipeline";

    vector<deque<int>> dep = initial;
    vector<deque<int>> sid(R);
    vector<int> placed(R, 0);

    {
        vector<Move> turn;

        for (int i = 0; i < R; ++i) {
            turn.push_back(Move{0, i, i, (int)dep[i].size()});
        }

        if (!addTurn(dep, sid, res, turn, 0)) {
            return res;
        }
    }

    for (int off = 0; off < N; ++off) {
        int guard = 0;

        while (true) {
            bool doneOff = true;

            for (int g = 0; g < R; ++g) {
                if (placed[g] <= off) {
                    doneOff = false;
                    break;
                }
            }

            if (doneOff) break;

            if (++guard > 500) {
                res.validationMsg = "offset guard exceeded";
                return res;
            }

            vector<deque<int>> beforeDep = dep;
            vector<deque<int>> beforeSid = sid;
            vector<int> beforePlaced = placed;

            vector<Candidate> cand = makeOffsetCandidates(dep, sid, placed, off);
            vector<Move> turn = chooseBestTurn(cand);

            if (turn.empty()) {
                res.stats.offsetDeadTurns++;
                res.validationMsg = "offset no candidate";
                return res;
            }

            int placedThisTurn = 0;
            int exposeSidThisTurn = 0;
            int exposeDepThisTurn = 0;
            int flushThisTurn = 0;

            for (const Move &m : turn) {
                if (m.type == 1 &&
                    m.k == 1 &&
                    beforePlaced[m.i] == off &&
                    (int)beforeDep[m.i].size() == beforePlaced[m.i] &&
                    !beforeSid[m.j].empty() &&
                    beforeSid[m.j].front() == N * m.i + off) {
                    placed[m.i]++;
                    placedThisTurn++;
                } else if (m.type == 1) {
                    exposeSidThisTurn++;
                } else {
                    if (!beforeDep[m.i].empty()) {
                        int x = beforeDep[m.i].back();
                        int g = groupOf(x);

                        if (beforePlaced[g] == off && x == N * g + off) {
                            exposeDepThisTurn++;
                        } else {
                            flushThisTurn++;
                        }
                    }
                }
            }

            if (!addTurn(dep, sid, res, turn, 1)) {
                return res;
            }

            res.stats.offsetPlaced += placedThisTurn;
            res.stats.offsetExposeSid += exposeSidThisTurn;
            res.stats.offsetExposeDep += exposeDepThisTurn;
            res.stats.offsetFlush += flushThisTurn;
            chmax(res.stats.offsetMaxPlacedTurn, placedThisTurn);
            chmax(res.stats.offsetMaxDirty, totalDirty(dep, sid, placed));
        }
    }

    string msg;

    if (!validateTurns(initial, res.turns, msg)) {
        res.ok = false;
        res.validationMsg = msg;
        return res;
    }

    res.ok = true;
    res.validationMsg = msg;
    res.stats.offsetOk = 1;
    res.stats.offsetTurns = (int)res.turns.size();
    res.stats.offsetMoves = res.totalMoves;

    if (!res.turns.empty()) {
        res.stats.offsetAvgPlacedX100 = (int)(100LL * res.stats.offsetPlaced / (int)res.turns.size());
    }

    return res;
}

int maxSameGroupBlockBack(const deque<int> &dq) {
    if (dq.empty()) return 0;

    int g = groupOf(dq.back());
    int len = 1;

    for (int i = (int)dq.size() - 2; i >= 0; --i) {
        if (groupOf(dq[i]) == g) ++len;
        else break;
    }

    return len;
}

vector<int> makeKCands(int maxBlock) {
    vector<int> v;

    for (int k : {maxBlock, 1, 2, 3}) {
        if (1 <= k && k <= maxBlock) {
            v.push_back(k);
        }
    }

    sort(rng(v));
    v.erase(unique(rng(v)), v.end());

    return v;
}

long long classifyWeight(
    const deque<int> &line,
    int d,
    int g,
    int k,
    const Param &p,
    unsigned long long inputHash
) {
    int offSum = 0;

    for (int idx = (int)line.size() - k; idx < (int)line.size(); ++idx) {
        offSum += offsetOf(line[idx]);
    }

    unsigned long long z = inputHash;
    z ^= p.seed;
    z ^= (unsigned long long)(d + 17) * 11995408973635179863ULL;
    z ^= (unsigned long long)(g + 31) * 10150724397891781847ULL;
    z ^= (unsigned long long)(k + 43) * 7259553151490588151ULL;
    z = splitmix64(z);

    long long noise = (long long)(z % 2001ULL) - 1000LL;

    long long w = 0;
    w += p.ck * k;
    w += p.coff * offSum;
    w += p.cgroup * g;
    w += p.cdiag * d;
    w += p.cabs * abs(d - g);
    w += p.cnoise * noise;

    return w;
}

vector<Candidate> makeClassifyCandidates(
    const vector<deque<int>> &dep,
    const Param &p,
    unsigned long long inputHash
) {
    vector<Candidate> cand;

    for (int d = 0; d < R; ++d) {
        if (dep[d].empty()) continue;

        int g = groupOf(dep[d].back());
        int maxBlock = maxSameGroupBlockBack(dep[d]);

        for (int k : makeKCands(maxBlock)) {
            Candidate c;
            c.m = Move{0, d, g, k};
            c.weight = classifyWeight(dep[d], d, g, k, p, inputHash);
            cand.push_back(c);
        }
    }

    return cand;
}

bool buildClassifyPhase(
    vector<deque<int>> &dep,
    vector<deque<int>> &sid,
    Solution &res,
    const Param &p,
    unsigned long long inputHash,
    const TimeKeeper &tk,
    bool useTimeLimit
) {
    int guard = 0;

    while (true) {
        bool empty = true;

        for (int i = 0; i < R; ++i) {
            if (!dep[i].empty()) empty = false;
        }

        if (empty) break;

        if (useTimeLimit && tk.timeUp(100)) return false;
        if (++guard > 300) return false;

        vector<Candidate> cand = makeClassifyCandidates(dep, p, inputHash);
        vector<Move> turn = chooseBestTurn(cand);

        if (turn.empty()) return false;

        if ((int)turn.size() == 1) {
            res.stats.singleTurns++;
        }

        for (const Move &m : turn) {
            if (m.type == 0) {
                res.stats.classifyMoves++;
                res.stats.movedCars += m.k;

                if (m.k == 1) res.stats.k1Moves++;
                if (m.k == maxSameGroupBlockBack(dep[m.i])) res.stats.kMaxMoves++;
            }

            string e;

            if (!applyMoveState(dep, sid, m, &e)) {
                res.validationMsg = "classify apply failed: " + e;
                return false;
            }
        }

        res.turns.push_back(turn);
        res.phaseTurns[2]++;
        res.phaseMoves[2] += (int)turn.size();
        res.totalMoves += (int)turn.size();
        res.stats.classifyTurns++;

        for (int r = 0; r < R; ++r) {
            chmax(res.maxDepLen, (int)dep[r].size());
            chmax(res.maxSidLen, (int)sid[r].size());
        }
    }

    return true;
}

vector<int> groupsFromMask(int mask) {
    vector<int> res;

    for (int g = 0; g < R; ++g) {
        if ((mask >> g) & 1) res.push_back(g);
    }

    return res;
}

vector<int> complementGroups(const vector<int> &a) {
    vector<int> used(R, 0);

    for (int x : a) used[x] = 1;

    vector<int> b;

    for (int i = 0; i < R; ++i) {
        if (!used[i]) b.push_back(i);
    }

    return b;
}

bool applyScheduled(vector<deque<int>> &dep, vector<deque<int>> &sid, Solution *res, vector<Move> moves, int phase) {
    while (!moves.empty()) {
        vector<Candidate> cand;

        for (const Move &m : moves) {
            cand.push_back(Candidate{m, 1000000LL + m.k});
        }

        vector<Move> turn = chooseBestTurn(cand);

        if (turn.empty()) return false;

        for (const Move &m : turn) {
            string e;

            if (!applyMoveState(dep, sid, m, &e)) {
                return false;
            }
        }

        if (res) {
            res->turns.push_back(turn);
            res->phaseTurns[phase]++;
            res->phaseMoves[phase] += (int)turn.size();
            res->totalMoves += (int)turn.size();

            for (int r = 0; r < R; ++r) {
                chmax(res->maxDepLen, (int)dep[r].size());
                chmax(res->maxSidLen, (int)sid[r].size());
            }
        }

        vector<int> used((int)moves.size(), 0);

        for (const Move &sel : turn) {
            for (int i = 0; i < (int)moves.size(); ++i) {
                if (used[i]) continue;

                const Move &m = moves[i];

                if (m.type == sel.type &&
                    m.i == sel.i &&
                    m.j == sel.j &&
                    m.k == sel.k) {
                    used[i] = 1;
                    break;
                }
            }
        }

        vector<Move> nxt;

        for (int i = 0; i < (int)moves.size(); ++i) {
            if (!used[i]) nxt.push_back(moves[i]);
        }

        moves.swap(nxt);
    }

    return true;
}

bool runSplitBatch(
    vector<deque<int>> &dep,
    vector<deque<int>> &sid,
    Solution *res,
    const vector<int> &groups,
    const vector<int> &temps,
    int phase
) {
    if (groups.size() != temps.size()) return false;

    auto done = [&]() -> bool {
        for (int g : groups) {
            if ((int)dep[g].size() != N) return false;
        }

        for (int t : temps) {
            if (!dep[t].empty()) return false;
        }

        return true;
    };

    for (int guard = 0; guard < 500 && !done(); ++guard) {
        vector<Candidate> cand;

        for (int idx = 0; idx < (int)groups.size(); ++idx) {
            int g = groups[idx];
            int t = temps[idx];

            if ((int)dep[g].size() >= N) {
                if (!dep[t].empty()) {
                    cand.push_back(Candidate{Move{0, t, g, (int)dep[t].size()}, 50000});
                }

                continue;
            }

            int target = N * g + (int)dep[g].size();

            if (!dep[t].empty()) {
                if (dep[t].back() == target) {
                    cand.push_back(Candidate{Move{0, t, g, 1}, 950000});
                } else {
                    cand.push_back(Candidate{Move{0, t, g, (int)dep[t].size()}, 40000});
                }
            }

            int pos = findDeque(sid[g], target);

            if (pos < 0) {
                if (dep[t].empty()) return false;
                continue;
            }

            if (pos == 0) {
                cand.push_back(Candidate{Move{1, g, g, 1}, 1000000});
            } else {
                if ((int)dep[t].size() + pos <= DEP_CAP) {
                    cand.push_back(Candidate{Move{1, t, g, pos}, 200000 + pos * 1000});
                } else if (!dep[t].empty()) {
                    cand.push_back(Candidate{Move{0, t, g, (int)dep[t].size()}, 30000});
                }
            }
        }

        vector<Move> turn = chooseBestTurn(cand);

        if (turn.empty()) return false;

        for (const Move &m : turn) {
            string e;

            if (!applyMoveState(dep, sid, m, &e)) {
                return false;
            }
        }

        if (res) {
            res->turns.push_back(turn);
            res->phaseTurns[3]++;
            res->phaseMoves[3] += (int)turn.size();
            res->totalMoves += (int)turn.size();

            for (int r = 0; r < R; ++r) {
                chmax(res->maxDepLen, (int)dep[r].size());
                chmax(res->maxSidLen, (int)sid[r].size());
            }
        }
    }

    return done();
}

struct SplitSim {
    bool ok = false;
    int turns = INF;
    int moves = INF;
    int firstTurns = INF;
    int secondTurns = INF;
};

SplitSim simulateSplitFinish(const vector<deque<int>> &dep0, const vector<deque<int>> &sid0, int mask) {
    SplitSim sim;

    vector<int> first = groupsFromMask(mask);
    vector<int> second = complementGroups(first);

    vector<deque<int>> dep = dep0;
    vector<deque<int>> sid = sid0;
    Solution tmp;
    tmp.ok = true;

    if (!runSplitBatch(dep, sid, &tmp, first, second, 3)) return sim;

    vector<Move> store;

    for (int g : first) {
        store.push_back(Move{0, g, g, N});
    }

    if (!applyScheduled(dep, sid, &tmp, store, 3)) return sim;
    if (!runSplitBatch(dep, sid, &tmp, second, first, 4)) return sim;

    vector<Move> restore;

    for (int g : first) {
        restore.push_back(Move{1, g, g, N});
    }

    if (!applyScheduled(dep, sid, &tmp, restore, 4)) return sim;

    bool finalOk = true;

    for (int g = 0; g < R; ++g) {
        if ((int)dep[g].size() != N) {
            finalOk = false;
            break;
        }

        for (int c = 0; c < N; ++c) {
            if (dep[g][c] != N * g + c) {
                finalOk = false;
                break;
            }
        }

        if (!sid[g].empty()) {
            finalOk = false;
            break;
        }
    }

    if (!finalOk) return sim;

    sim.ok = true;
    sim.turns = (int)tmp.turns.size();
    sim.moves = tmp.totalMoves;
    sim.firstTurns = tmp.phaseTurns[3];
    sim.secondTurns = tmp.phaseTurns[4];

    return sim;
}

bool finishSplit(vector<deque<int>> dep0, vector<deque<int>> sid0, Solution base, Solution &out) {
    SplitSim bestSim;
    int bestMask = -1;

    for (int mask = 0; mask < (1 << R); ++mask) {
        if (popcount(mask) != 5) continue;

        SplitSim sim = simulateSplitFinish(dep0, sid0, mask);

        if (!sim.ok) continue;

        if (sim.turns < bestSim.turns ||
            (sim.turns == bestSim.turns && sim.moves < bestSim.moves)) {
            bestSim = sim;
            bestMask = mask;
        }
    }

    if (bestMask < 0) return false;

    out = base;
    out.method += "+split_finish";
    out.stats.splitMask = bestMask;
    out.stats.splitPredTurns = bestSim.turns;
    out.stats.splitPredMoves = bestSim.moves;
    out.stats.splitBatchTurns1 = bestSim.firstTurns;
    out.stats.splitBatchTurns2 = bestSim.secondTurns;

    vector<int> first = groupsFromMask(bestMask);
    vector<int> second = complementGroups(first);

    vector<deque<int>> dep = dep0;
    vector<deque<int>> sid = sid0;

    if (!runSplitBatch(dep, sid, &out, first, second, 3)) return false;

    vector<Move> store;

    for (int g : first) {
        store.push_back(Move{0, g, g, N});
    }

    if (!applyScheduled(dep, sid, &out, store, 3)) return false;
    if (!runSplitBatch(dep, sid, &out, second, first, 4)) return false;

    vector<Move> restore;

    for (int g : first) {
        restore.push_back(Move{1, g, g, N});
    }

    if (!applyScheduled(dep, sid, &out, restore, 4)) return false;

    out.ok = true;
    return true;
}

struct ClassifiedResult {
    bool ok = false;
    vector<deque<int>> dep;
    vector<deque<int>> sid;
    Solution sol;
};

ClassifiedResult buildClassified(
    const vector<deque<int>> &initial,
    int trialId,
    const Param &p,
    unsigned long long inputHash,
    const TimeKeeper &tk,
    bool useTimeLimit
) {
    ClassifiedResult cr;
    cr.dep = initial;
    cr.sid.assign(R, deque<int>());
    cr.sol.trialId = trialId;
    cr.sol.method = "classify";

    if (!buildClassifyPhase(cr.dep, cr.sid, cr.sol, p, inputHash, tk, useTimeLimit)) {
        if (cr.sol.validationMsg.empty()) {
            cr.sol.validationMsg = "classify failed";
        }
        return cr;
    }

    for (int g = 0; g < R; ++g) {
        if (!cr.dep[g].empty()) {
            cr.sol.validationMsg = "dep not empty after classify";
            return cr;
        }

        if ((int)cr.sid[g].size() != N) {
            cr.sol.validationMsg = "sid size invalid after classify";
            return cr;
        }

        for (int x : cr.sid[g]) {
            if (groupOf(x) != g) {
                cr.sol.validationMsg = "sid group invalid after classify";
                return cr;
            }
        }
    }

    cr.ok = true;
    return cr;
}

void finalizeSolution(const vector<deque<int>> &initial, Solution &sol) {
    string msg;

    if (!validateTurns(initial, sol.turns, msg)) {
        sol.ok = false;
        sol.validationMsg = msg;
        return;
    }

    sol.ok = true;
    sol.validationMsg = msg;
}

bool betterSolution(const Solution &a, const Solution &b) {
    if (!a.ok) return false;
    if (!b.ok) return true;

    if ((int)a.turns.size() != (int)b.turns.size()) {
        return (int)a.turns.size() < (int)b.turns.size();
    }

    if (a.method == "offset_pipeline" && b.method != "offset_pipeline") {
        return true;
    }

    if (a.totalMoves != b.totalMoves) {
        return a.totalMoves < b.totalMoves;
    }

    return a.trialId < b.trialId;
}

vector<Param> makeParams(unsigned long long inputHash) {
    vector<Param> ps;

    auto add = [&](long long ck, long long coff, long long cgroup, long long cdiag,
                   long long cabs, long long cnoise, unsigned long long seed) {
        Param p;
        p.ck = ck;
        p.coff = coff;
        p.cgroup = cgroup;
        p.cdiag = cdiag;
        p.cabs = cabs;
        p.cnoise = cnoise;
        p.seed = splitmix64(inputHash ^ seed);
        ps.push_back(p);
    };

    add(1000000, 10000, 0, 1, 0, 0, 0);
    add(1000000, -10000, 0, 1, 0, 0, 1);
    add(10000, 1000000, 0, 1, 0, 0, 2);
    add(10000, -1000000, 0, 1, 0, 0, 3);
    add(1000000, 100, 10000, 1, -10000, 1000, 4);
    add(1000000, 100, -10000, 1, 10000, 1000, 5);
    add(1000000, 100, 0, -10000, 0, 1000, 6);
    add(1000000, 100, 0, 10000, 0, 1000, 7);
    add(500000, 500000, 0, 0, 0, 1000, 8);
    add(500000, -500000, 0, 0, 0, 1000, 9);

    return ps;
}

Param randomParam(mt19937_64 &mt) {
    vector<long long> v = {-1000000, -500000, -100000, 0, 100000, 500000, 1000000};
    vector<long long> k = {100000, 500000, 1000000, 1500000, 2000000};

    auto pick = [&](const vector<long long> &a) {
        return a[(int)(mt() % a.size())];
    };

    Param p;
    p.ck = pick(k);
    p.coff = pick(v);
    p.cgroup = pick(v);
    p.cdiag = pick(v);
    p.cabs = pick(v);
    p.cnoise = (long long)(mt() % 5000);
    p.seed = mt();

    return p;
}

void printUpdate(const Solution &s, int trial, long long elapsed) {
    cerr << "best_update"
         << " trial=" << trial
         << " elapsed_ms=" << elapsed
         << " method=" << s.method
         << " turns=" << s.turns.size()
         << " moves=" << s.totalMoves
         << " phase_turns="
         << s.phaseTurns[0] << ","
         << s.phaseTurns[1] << ","
         << s.phaseTurns[2] << ","
         << s.phaseTurns[3] << ","
         << s.phaseTurns[4]
         << " phase_moves="
         << s.phaseMoves[0] << ","
         << s.phaseMoves[1] << ","
         << s.phaseMoves[2] << ","
         << s.phaseMoves[3] << ","
         << s.phaseMoves[4]
         << " offset_ok=" << s.stats.offsetOk
         << " offset_turns=" << s.stats.offsetTurns
         << " offset_moves=" << s.stats.offsetMoves
         << " offset_placed=" << s.stats.offsetPlaced
         << " offset_expose_sid=" << s.stats.offsetExposeSid
         << " offset_expose_dep=" << s.stats.offsetExposeDep
         << " offset_flush=" << s.stats.offsetFlush
         << " offset_dead_turns=" << s.stats.offsetDeadTurns
         << " offset_max_place_turn=" << s.stats.offsetMaxPlacedTurn
         << " offset_avg_place_x100=" << s.stats.offsetAvgPlacedX100
         << " offset_max_dirty=" << s.stats.offsetMaxDirty
         << " classify_turns=" << s.stats.classifyTurns
         << " classify_moves=" << s.stats.classifyMoves
         << " moved_cars=" << s.stats.movedCars
         << " k1_moves=" << s.stats.k1Moves
         << " kmax_moves=" << s.stats.kMaxMoves
         << " single_turns=" << s.stats.singleTurns
         << " split_mask=" << s.stats.splitMask
         << " split_pred_turns=" << s.stats.splitPredTurns
         << " split_pred_moves=" << s.stats.splitPredMoves
         << " split_batch_turns=" << s.stats.splitBatchTurns1 << "," << s.stats.splitBatchTurns2
         << " max_departure_len=" << s.maxDepLen
         << " max_siding_len=" << s.maxSidLen
         << " validation=" << s.validationMsg
         << '\n';
}

void solve()
{
    TimeKeeper tk;

    int inputR;
    cin >> inputR;

    vector<deque<int>> initial(inputR);
    unsigned long long inputHash = 1469598103934665603ULL;

    rep(r, inputR) {
        rep(c, N) {
            int x;
            cin >> x;
            initial[r].push_back(x);
            inputHash ^= (unsigned long long)(x + 1009 * r + 9176 * c + 1);
            inputHash *= 1099511628211ULL;
        }
    }

    if (inputR != R) {
        cerr << "unsupported R=" << inputR << '\n';
        exit(1);
    }

    Solution best;
    int trial = 0;
    int validTrials = 0;
    int offsetTrials = 0;
    int fallbackTrials = 0;
    int bestUpdates = 0;

    {
        Solution cur = buildOffsetPipeline(initial);
        cur.trialId = trial;
        offsetTrials++;

        if (cur.ok) {
            validTrials++;
        }

        if (betterSolution(cur, best)) {
            best = cur;
            bestUpdates++;
            printUpdate(best, trial, tk.elapsedMs());
        } else {
            cerr << "offset_summary"
                 << " ok=" << cur.ok
                 << " turns=" << cur.turns.size()
                 << " moves=" << cur.totalMoves
                 << " placed=" << cur.stats.offsetPlaced
                 << " expose_sid=" << cur.stats.offsetExposeSid
                 << " expose_dep=" << cur.stats.offsetExposeDep
                 << " flush=" << cur.stats.offsetFlush
                 << " max_place_turn=" << cur.stats.offsetMaxPlacedTurn
                 << " avg_place_x100=" << cur.stats.offsetAvgPlacedX100
                 << " max_dirty=" << cur.stats.offsetMaxDirty
                 << " validation=" << cur.validationMsg
                 << '\n';
        }

        trial++;
    }

    vector<Param> params = makeParams(inputHash);
    mt19937_64 mt(inputHash ^ 0x9e3779b97f4a7c15ULL);

    auto tryFallback = [&](const Param &p) {
        if (tk.timeUp(100)) return;

        ClassifiedResult cr = buildClassified(initial, trial, p, inputHash, tk, true);
        fallbackTrials++;

        if (!cr.ok) {
            trial++;
            return;
        }

        Solution cand;

        if (finishSplit(cr.dep, cr.sid, cr.sol, cand)) {
            cand.trialId = trial;
            finalizeSolution(initial, cand);

            if (cand.ok) {
                validTrials++;

                if (betterSolution(cand, best)) {
                    best = cand;
                    bestUpdates++;
                    printUpdate(best, trial, tk.elapsedMs());
                }
            }
        }

        trial++;
    };

    for (int i = 0; i < (int)params.size(); ++i) {
        if (tk.timeUp(450)) break;
        tryFallback(params[i]);
    }

    while (!tk.timeUp(100)) {
        Param p = randomParam(mt);
        tryFallback(p);
    }

    if (!best.ok) {
        cerr << "no valid timed solution, building emergency fallback" << '\n';

        Param fallback = makeParams(inputHash)[0];
        ClassifiedResult cr = buildClassified(initial, trial, fallback, inputHash, tk, false);

        if (!cr.ok) {
            cerr << "emergency classify failed: " << cr.sol.validationMsg << '\n';
            exit(1);
        }

        Solution cand;

        if (!finishSplit(cr.dep, cr.sid, cr.sol, cand)) {
            cerr << "emergency finish failed" << '\n';
            exit(1);
        }

        finalizeSolution(initial, cand);

        if (!cand.ok) {
            cerr << "emergency validation failed: " << cand.validationMsg << '\n';
            exit(1);
        }

        best = cand;
    }

    cerr << "final_summary"
         << " trials=" << trial
         << " valid_trials=" << validTrials
         << " offset_trials=" << offsetTrials
         << " fallback_trials=" << fallbackTrials
         << " best_updates=" << bestUpdates
         << " elapsed_ms=" << tk.elapsedMs()
         << " selected_trial=" << best.trialId
         << " method=" << best.method
         << " turns=" << best.turns.size()
         << " moves=" << best.totalMoves
         << " phase_turns="
         << best.phaseTurns[0] << ","
         << best.phaseTurns[1] << ","
         << best.phaseTurns[2] << ","
         << best.phaseTurns[3] << ","
         << best.phaseTurns[4]
         << " phase_moves="
         << best.phaseMoves[0] << ","
         << best.phaseMoves[1] << ","
         << best.phaseMoves[2] << ","
         << best.phaseMoves[3] << ","
         << best.phaseMoves[4]
         << " offset_ok=" << best.stats.offsetOk
         << " offset_turns=" << best.stats.offsetTurns
         << " offset_moves=" << best.stats.offsetMoves
         << " offset_placed=" << best.stats.offsetPlaced
         << " offset_expose_sid=" << best.stats.offsetExposeSid
         << " offset_expose_dep=" << best.stats.offsetExposeDep
         << " offset_flush=" << best.stats.offsetFlush
         << " offset_dead_turns=" << best.stats.offsetDeadTurns
         << " offset_max_place_turn=" << best.stats.offsetMaxPlacedTurn
         << " offset_avg_place_x100=" << best.stats.offsetAvgPlacedX100
         << " offset_max_dirty=" << best.stats.offsetMaxDirty
         << " classify_turns=" << best.stats.classifyTurns
         << " classify_moves=" << best.stats.classifyMoves
         << " moved_cars=" << best.stats.movedCars
         << " k1_moves=" << best.stats.k1Moves
         << " kmax_moves=" << best.stats.kMaxMoves
         << " single_turns=" << best.stats.singleTurns
         << " split_mask=" << best.stats.splitMask
         << " split_pred_turns=" << best.stats.splitPredTurns
         << " split_pred_moves=" << best.stats.splitPredMoves
         << " split_batch_turns=" << best.stats.splitBatchTurns1 << "," << best.stats.splitBatchTurns2
         << " max_departure_len=" << best.maxDepLen
         << " max_siding_len=" << best.maxSidLen
         << " validation=" << best.validationMsg
         << '\n';

    cout << best.turns.size() << '\n';

    for (const auto &turn : best.turns) {
        cout << turn.size() << '\n';

        for (const Move &m : turn) {
            cout << m.type << ' ' << m.i << ' ' << m.j << ' ' << m.k << '\n';
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
    return 0;
}