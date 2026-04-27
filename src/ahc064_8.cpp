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
// #define rng(a) a.begin(),a.end()
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

const int CAR_PER_LINE = 10;
const int DEP_CAP = 15;
const int SID_CAP = 20;
const long long TIME_LIMIT_MS = 1980;

struct TimeKeeper
{
    chrono::high_resolution_clock::time_point st;

    TimeKeeper()
    {
        st = chrono::high_resolution_clock::now();
    }

    long long elapsedMs() const
    {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now - st).count();
    }

    bool timeUp(long long marginMs = 0) const
    {
        return elapsedMs() >= TIME_LIMIT_MS - marginMs;
    }
};

struct Move
{
    int type;
    int i;
    int j;
    int k;
};

struct Candidate
{
    Move m;
    long long weight;
};

struct Phase1Param
{
    long long ck = 0;
    long long co = 0;
    long long cg = 0;
    long long cd = 0;
    long long cabs = 0;
    long long cminv = 0;
    long long cmaxv = 0;
    long long cnoise = 0;
    unsigned long long seed = 0;
};

struct SolutionResult
{
    bool ok = false;
    int trialId = -1;
    string method;
    Phase1Param param;

    vector<vector<Move>> turns;
    int totalMoves = 0;
    array<int, 5> phaseTurns{};
    array<int, 5> phaseMoves{};

    int maxDepLen = 0;
    int maxSidLen = 0;

    string validationMsg;
};

unsigned long long splitmix64(unsigned long long x)
{
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

bool applyMoveState(vector<deque<int>> &dep, vector<deque<int>> &sid, const Move &m, string *err = nullptr)
{
    int R = (int)dep.size();

    if (m.type != 0 && m.type != 1)
    {
        if (err) *err = "invalid type";
        return false;
    }

    if (m.i < 0 || m.i >= R || m.j < 0 || m.j >= R || m.k <= 0)
    {
        if (err) *err = "invalid parameter";
        return false;
    }

    if (m.type == 0)
    {
        if ((int)dep[m.i].size() < m.k)
        {
            if (err) *err = "departure underflow";
            return false;
        }

        if ((int)sid[m.j].size() + m.k > SID_CAP)
        {
            if (err) *err = "siding overflow";
            return false;
        }

        vector<int> block(m.k);

        for (int p = m.k - 1; p >= 0; --p)
        {
            block[p] = dep[m.i].back();
            dep[m.i].pop_back();
        }

        for (int p = m.k - 1; p >= 0; --p)
        {
            sid[m.j].push_front(block[p]);
        }
    }
    else
    {
        if ((int)sid[m.j].size() < m.k)
        {
            if (err) *err = "siding underflow";
            return false;
        }

        if ((int)dep[m.i].size() + m.k > DEP_CAP)
        {
            if (err) *err = "departure overflow";
            return false;
        }

        vector<int> block(m.k);

        for (int p = 0; p < m.k; ++p)
        {
            block[p] = sid[m.j].front();
            sid[m.j].pop_front();
        }

        for (int p = 0; p < m.k; ++p)
        {
            dep[m.i].push_back(block[p]);
        }
    }

    return true;
}

bool validTurnMoves(const vector<Move> &turn, int R, string *err = nullptr)
{
    if (turn.empty() || (int)turn.size() > R)
    {
        if (err) *err = "invalid K";
        return false;
    }

    vector<int> usedDep(R, 0);
    vector<int> usedSid(R, 0);

    for (int a = 0; a < (int)turn.size(); ++a)
    {
        const Move &ma = turn[a];

        if (ma.type != 0 && ma.type != 1)
        {
            if (err) *err = "invalid type";
            return false;
        }

        if (ma.i < 0 || ma.i >= R || ma.j < 0 || ma.j >= R || ma.k <= 0)
        {
            if (err) *err = "invalid parameter";
            return false;
        }

        if (usedDep[ma.i] || usedSid[ma.j])
        {
            if (err) *err = "line reused";
            return false;
        }

        usedDep[ma.i] = 1;
        usedSid[ma.j] = 1;

        for (int b = 0; b < a; ++b)
        {
            const Move &mb = turn[b];

            if (mb.i < ma.i && !(mb.j < ma.j))
            {
                if (err) *err = "crossing";
                return false;
            }

            if (ma.i < mb.i && !(ma.j < mb.j))
            {
                if (err) *err = "crossing";
                return false;
            }
        }
    }

    return true;
}

vector<Move> chooseBestTurn(const vector<Candidate> &cand, int R)
{
    int n = (int)cand.size();
    int bestMask = 0;
    long long bestWeight = LLONG_MIN;
    int bestKSum = -1;
    int bestCnt = -1;

    for (int mask = 1; mask < (1 << n); ++mask)
    {
        vector<Move> turn;
        long long weight = 0;
        int kSum = 0;

        for (int b = 0; b < n; ++b)
        {
            if ((mask >> b) & 1)
            {
                turn.push_back(cand[b].m);
                weight += cand[b].weight;
                kSum += cand[b].m.k;
            }
        }

        string err;
        if (!validTurnMoves(turn, R, &err)) continue;

        int cnt = (int)turn.size();

        if (weight > bestWeight ||
            (weight == bestWeight && kSum > bestKSum) ||
            (weight == bestWeight && kSum == bestKSum && cnt > bestCnt))
        {
            bestWeight = weight;
            bestKSum = kSum;
            bestCnt = cnt;
            bestMask = mask;
        }
    }

    vector<Move> res;

    for (int b = 0; b < n; ++b)
    {
        if ((bestMask >> b) & 1)
        {
            res.push_back(cand[b].m);
        }
    }

    sort(res.begin(), res.end(), [](const Move &a, const Move &b)
    {
        if (a.i != b.i) return a.i < b.i;
        return a.j < b.j;
    });

    return res;
}

bool validateTurns(const vector<deque<int>> &initial, const vector<vector<Move>> &turns, string &msg)
{
    int R = (int)initial.size();

    if ((int)turns.size() > 4000)
    {
        msg = "too many turns";
        return false;
    }

    vector<deque<int>> dep = initial;
    vector<deque<int>> sid(R);

    for (int t = 0; t < (int)turns.size(); ++t)
    {
        string turnErr;

        if (!validTurnMoves(turns[t], R, &turnErr))
        {
            msg = turnErr + " at turn " + to_string(t);
            return false;
        }

        for (const Move &m : turns[t])
        {
            string err;

            if (!applyMoveState(dep, sid, m, &err))
            {
                msg = err + " at turn " + to_string(t);
                return false;
            }
        }
    }

    for (int r = 0; r < R; ++r)
    {
        if ((int)dep[r].size() != CAR_PER_LINE)
        {
            msg = "wrong final departure size";
            return false;
        }

        for (int c = 0; c < CAR_PER_LINE; ++c)
        {
            if (dep[r][c] != CAR_PER_LINE * r + c)
            {
                msg = "wrong final order";
                return false;
            }
        }

        if (!sid[r].empty())
        {
            msg = "siding not empty";
            return false;
        }
    }

    msg = "ok";
    return true;
}

long long phase1WeightByParam(
    const deque<int> &line,
    int d,
    int g,
    int k,
    const Phase1Param &p,
    unsigned long long inputHash
)
{
    int offsetSum = 0;
    int offsetMin = INF;
    int offsetMax = -INF;

    for (int idx = (int)line.size() - k; idx < (int)line.size(); ++idx)
    {
        int off = line[idx] % CAR_PER_LINE;
        offsetSum += off;
        chmin(offsetMin, off);
        chmax(offsetMax, off);
    }

    unsigned long long z = inputHash;
    z ^= p.seed;
    z ^= (unsigned long long)(d + 11) * 10150724397891781847ULL;
    z ^= (unsigned long long)(g + 17) * 7259553151490588151ULL;
    z ^= (unsigned long long)(k + 23) * 2870177450012600261ULL;
    z = splitmix64(z);

    long long noise = (long long)(z % 2001ULL) - 1000LL;

    long long w = 0;
    w += p.ck * (long long)k;
    w += p.co * (long long)offsetSum;
    w += p.cg * (long long)g;
    w += p.cd * (long long)d;
    w += p.cabs * (long long)abs(d - g);
    w += p.cminv * (long long)offsetMin;
    w += p.cmaxv * (long long)offsetMax;
    w += p.cnoise * noise;

    return w;
}

bool betterSolution(const SolutionResult &a, const SolutionResult &b)
{
    if (!a.ok) return false;
    if (!b.ok) return true;
    if (a.turns.size() != b.turns.size()) return a.turns.size() < b.turns.size();
    if (a.phaseTurns[2] != b.phaseTurns[2]) return a.phaseTurns[2] < b.phaseTurns[2];
    if (a.phaseTurns[1] != b.phaseTurns[1]) return a.phaseTurns[1] < b.phaseTurns[1];
    if (a.totalMoves != b.totalMoves) return a.totalMoves < b.totalMoves;
    return a.trialId < b.trialId;
}

SolutionResult buildDirectBatchSolution(
    const vector<deque<int>> &initial,
    int trialId,
    const Phase1Param &param,
    unsigned long long inputHash,
    const TimeKeeper &tk,
    bool useTimeLimit
)
{
    int R = (int)initial.size();

    SolutionResult res;
    res.trialId = trialId;
    res.method = "direct_batch_sort";
    res.param = param;
    res.maxDepLen = CAR_PER_LINE;
    res.maxSidLen = 0;

    vector<deque<int>> dep = initial;
    vector<deque<int>> sid(R);

    auto refreshMaxLen = [&]()
    {
        for (int r = 0; r < R; ++r)
        {
            chmax(res.maxDepLen, (int)dep[r].size());
            chmax(res.maxSidLen, (int)sid[r].size());
        }
    };

    auto applyTurn = [&](const vector<Move> &turn, int phase) -> bool
    {
        if (turn.empty())
        {
            return true;
        }

        string turnErr;

        if (!validTurnMoves(turn, R, &turnErr))
        {
            res.validationMsg = "invalid generated turn: " + turnErr;
            return false;
        }

        for (const Move &m : turn)
        {
            string err;
            bool ok = applyMoveState(dep, sid, m, &err);

            if (!ok)
            {
                res.validationMsg = "internal move error: " + err;
                return false;
            }
        }

        res.turns.push_back(turn);
        res.phaseTurns[phase]++;
        res.phaseMoves[phase] += (int)turn.size();
        res.totalMoves += (int)turn.size();
        refreshMaxLen();
        return true;
    };

    /*
        Phase 1:
        目的グループ ID / 10 ごとに待避線へ分類する。
        この後、各待避線を直接ソートする。
    */
    while (true)
    {
        if (useTimeLimit && tk.timeUp(3))
        {
            res.validationMsg = "timeout in phase1";
            return res;
        }

        vector<Candidate> cand;

        for (int d = 0; d < R; ++d)
        {
            if (dep[d].empty()) continue;

            int g = dep[d].back() / CAR_PER_LINE;
            int k = 1;

            for (int idx = (int)dep[d].size() - 2; idx >= 0; --idx)
            {
                if (dep[d][idx] / CAR_PER_LINE == g)
                {
                    ++k;
                }
                else
                {
                    break;
                }
            }

            Candidate c;
            c.m = Move{0, d, g, k};
            c.weight = phase1WeightByParam(dep[d], d, g, k, param, inputHash);
            cand.push_back(c);
        }

        if (cand.empty()) break;

        vector<Move> turn = chooseBestTurn(cand, R);

        if (turn.empty())
        {
            res.validationMsg = "phase1 deadlock";
            return res;
        }

        if (!applyTurn(turn, 1)) return res;
    }

    for (int g = 0; g < R; ++g)
    {
        if ((int)dep[g].size() != 0)
        {
            res.validationMsg = "phase1 departure not empty";
            return res;
        }

        if ((int)sid[g].size() != CAR_PER_LINE)
        {
            res.validationMsg = "phase1 invalid siding size";
            return res;
        }

        for (int x : sid[g])
        {
            if (x / CAR_PER_LINE != g)
            {
                res.validationMsg = "phase1 invalid group";
                return res;
            }
        }
    }

    auto findInSiding = [&](int g, int target) -> int
    {
        for (int i = 0; i < (int)sid[g].size(); ++i)
        {
            if (sid[g][i] == target) return i;
        }

        return -1;
    };

    /*
        5本ずつ直接ソートする。

        前半:
            group 0..4 を dep 0..4 に完成させる。
            temp として dep 5..9 を使う。

        後半:
            dep 0..4 の完成列を一時的に sid 0..4 へ退避する。
            group 5..9 を dep 5..9 に完成させる。
            最後に sid 0..4 を dep 0..4 へ戻す。

        各 group g の sid[g] から target=10g+off を探し、
        target より前の邪魔な車両を temp dep に一時退避し、
        target を dep[g] へ追加し、
        邪魔な車両を sid[g] へ戻す。
    */
    auto sortBatch = [&](const vector<int> &groups, const vector<int> &temps, int phase) -> bool
    {
        int B = (int)groups.size();

        for (int off = 0; off < CAR_PER_LINE; ++off)
        {
            if (useTimeLimit && tk.timeUp(3))
            {
                res.validationMsg = "timeout in batch sort";
                return false;
            }

            vector<int> pos(B, 0);

            for (int b = 0; b < B; ++b)
            {
                int g = groups[b];
                int target = CAR_PER_LINE * g + off;
                pos[b] = findInSiding(g, target);

                if (pos[b] < 0)
                {
                    res.validationMsg = "target not found in batch sort";
                    return false;
                }
            }

            vector<Move> toTemp;

            for (int b = 0; b < B; ++b)
            {
                int g = groups[b];
                int t = temps[b];

                if (pos[b] > 0)
                {
                    toTemp.push_back(Move{1, t, g, pos[b]});
                }
            }

            sort(toTemp.begin(), toTemp.end(), [](const Move &a, const Move &b)
            {
                if (a.i != b.i) return a.i < b.i;
                return a.j < b.j;
            });

            if (!applyTurn(toTemp, phase)) return false;

            vector<Move> takeTarget;

            for (int b = 0; b < B; ++b)
            {
                int g = groups[b];
                int target = CAR_PER_LINE * g + off;

                if (sid[g].empty() || sid[g].front() != target)
                {
                    res.validationMsg = "front is not target after temp move";
                    return false;
                }

                takeTarget.push_back(Move{1, g, g, 1});
            }

            sort(takeTarget.begin(), takeTarget.end(), [](const Move &a, const Move &b)
            {
                if (a.i != b.i) return a.i < b.i;
                return a.j < b.j;
            });

            if (!applyTurn(takeTarget, phase)) return false;

            vector<Move> backTemp;

            for (int b = 0; b < B; ++b)
            {
                int g = groups[b];
                int t = temps[b];

                if (pos[b] > 0)
                {
                    backTemp.push_back(Move{0, t, g, pos[b]});
                }
            }

            sort(backTemp.begin(), backTemp.end(), [](const Move &a, const Move &b)
            {
                if (a.i != b.i) return a.i < b.i;
                return a.j < b.j;
            });

            if (!applyTurn(backTemp, phase)) return false;
        }

        return true;
    };

    vector<int> groupA = {0, 1, 2, 3, 4};
    vector<int> tempA  = {5, 6, 7, 8, 9};

    if (!sortBatch(groupA, tempA, 2)) return res;

    vector<Move> storeA;

    for (int g = 0; g < 5; ++g)
    {
        if ((int)dep[g].size() != CAR_PER_LINE)
        {
            res.validationMsg = "groupA output size invalid";
            return res;
        }

        for (int c = 0; c < CAR_PER_LINE; ++c)
        {
            if (dep[g][c] != CAR_PER_LINE * g + c)
            {
                res.validationMsg = "groupA output order invalid";
                return res;
            }
        }

        storeA.push_back(Move{0, g, g, CAR_PER_LINE});
    }

    if (!applyTurn(storeA, 2)) return res;

    vector<int> groupB = {5, 6, 7, 8, 9};
    vector<int> tempB  = {0, 1, 2, 3, 4};

    if (!sortBatch(groupB, tempB, 3)) return res;

    vector<Move> restoreA;

    for (int g = 0; g < 5; ++g)
    {
        if ((int)sid[g].size() != CAR_PER_LINE)
        {
            res.validationMsg = "stored groupA size invalid";
            return res;
        }

        for (int c = 0; c < CAR_PER_LINE; ++c)
        {
            if (sid[g][c] != CAR_PER_LINE * g + c)
            {
                res.validationMsg = "stored groupA order invalid";
                return res;
            }
        }

        restoreA.push_back(Move{1, g, g, CAR_PER_LINE});
    }

    if (!applyTurn(restoreA, 3)) return res;

    string msg;
    bool ok = validateTurns(initial, res.turns, msg);
    res.validationMsg = msg;
    res.ok = ok && ((int)res.turns.size() <= 4000);

    return res;
}

vector<Phase1Param> makeBaseParams(unsigned long long inputHash)
{
    vector<Phase1Param> ps;

    auto add = [&](long long ck, long long co, long long cg, long long cd, long long cabs,
                   long long cminv, long long cmaxv, long long cnoise, unsigned long long seed)
    {
        Phase1Param p;
        p.ck = ck;
        p.co = co;
        p.cg = cg;
        p.cd = cd;
        p.cabs = cabs;
        p.cminv = cminv;
        p.cmaxv = cmaxv;
        p.cnoise = cnoise;
        p.seed = splitmix64(inputHash ^ seed);
        ps.push_back(p);
    };

    add(1000000, 10000, 0, 1, 0, 0, 0, 0, 0);
    add(1000000, -10000, 0, 1, 0, 0, 0, 0, 1);
    add(10000, 1000000, 0, 1, 0, 0, 0, 0, 2);
    add(10000, -1000000, 0, 1, 0, 0, 0, 0, 3);
    add(1000000, 100, 10000, 1, 0, 0, 0, 0, 4);
    add(1000000, 100, -10000, 1, 0, 0, 0, 0, 5);
    add(1000000, 100, 0, 1, -10000, 0, 0, 0, 6);
    add(1000000, 100, 0, 1, 10000, 0, 0, 0, 7);
    add(10000, 0, 100, 1, 0, 0, 1000000, 0, 8);
    add(10000, 0, 100, 1, 0, -1000000, 0, 0, 9);
    add(1000000, 100, 0, -10000, 0, 0, 0, 0, 10);
    add(1000000, 100, 0, 10000, 0, 0, 0, 0, 11);
    add(1000000, 10, 0, 1, 0, 0, 0, 1000, 12);
    add(100, 1000000, 0, 1, 0, 0, 0, 1000, 13);
    add(100, -1000000, 0, 1, 0, 0, 0, 1000, 14);
    add(10000, 100, 0, 1, 0, 0, 0, 1000000, 15);
    add(0, 1000000, 0, 0, 0, 0, 0, 100000, 16);
    add(0, -1000000, 0, 0, 0, 0, 0, 100000, 17);
    add(500000, 500000, 20000, 0, -10000, 0, 0, 5000, 18);
    add(500000, -500000, -20000, 0, 10000, 0, 0, 5000, 19);
    add(2000000, 0, 0, 0, 0, -200000, 200000, 10000, 20);
    add(200000, 0, 500000, 500000, -500000, 0, 0, 10000, 21);
    add(200000, 0, -500000, -500000, 500000, 0, 0, 10000, 22);
    add(100000, 200000, 200000, -200000, -200000, -100000, 100000, 20000, 23);

    return ps;
}

Phase1Param randomParam(mt19937_64 &rng)
{
    vector<long long> big = {
        -3000000, -2000000, -1000000, -500000, -200000, -100000, -50000,
        0,
        50000, 100000, 200000, 500000, 1000000, 2000000, 3000000
    };

    vector<long long> kcoef = {
        -300000, -100000, 0, 100000, 200000, 500000, 1000000, 2000000, 3000000
    };

    vector<long long> ncoef = {
        0, 1000, 5000, 10000, 50000, 100000, 300000, 1000000
    };

    auto pick = [&](const vector<long long> &v) -> long long
    {
        return v[(int)(rng() % v.size())];
    };

    Phase1Param p;
    p.ck = pick(kcoef);
    p.co = pick(big);
    p.cg = pick(big);
    p.cd = pick(big);
    p.cabs = pick(big);
    p.cminv = pick(big);
    p.cmaxv = pick(big);
    p.cnoise = pick(ncoef);
    p.seed = rng();

    return p;
}

void printBestUpdate(const SolutionResult &best, int trial, long long elapsed)
{
    cerr << "best_update"
         << " trial=" << trial
         << " elapsed_ms=" << elapsed
         << " method=" << best.method
         << " turns=" << best.turns.size()
         << " moves=" << best.totalMoves
         << " phase_turns="
         << best.phaseTurns[1] << ','
         << best.phaseTurns[2] << ','
         << best.phaseTurns[3] << ','
         << best.phaseTurns[4]
         << " phase_moves="
         << best.phaseMoves[1] << ','
         << best.phaseMoves[2] << ','
         << best.phaseMoves[3] << ','
         << best.phaseMoves[4]
         << " max_departure_len=" << best.maxDepLen
         << " max_siding_len=" << best.maxSidLen
         << " validation=" << best.validationMsg
         << '\n';
}

void solve()
{
    TimeKeeper tk;

    int R;
    cin >> R;

    vector<deque<int>> initial(R);
    unsigned long long inputHash = 1469598103934665603ULL;

    rep(r, R)
    {
        rep(c, CAR_PER_LINE)
        {
            int x;
            cin >> x;
            initial[r].push_back(x);
            inputHash ^= (unsigned long long)(x + 1009 * r + 9176 * c + 1);
            inputHash *= 1099511628211ULL;
        }
    }

    if (R != CAR_PER_LINE)
    {
        cerr << "unsupported R=" << R << '\n';
        exit(1);
    }

    SolutionResult best;
    vector<Phase1Param> baseParams = makeBaseParams(inputHash);
    mt19937_64 rng(inputHash ^ 0x9e3779b97f4a7c15ULL);

    int trial = 0;
    int validTrials = 0;
    int bestUpdates = 0;
    long long totalTrialMs = 0;

    for (const Phase1Param &p : baseParams)
    {
        if (tk.timeUp(20)) break;

        long long st = tk.elapsedMs();
        SolutionResult cur = buildDirectBatchSolution(initial, trial, p, inputHash, tk, true);
        long long ed = tk.elapsedMs();
        totalTrialMs += max(1LL, ed - st);

        if (cur.ok) ++validTrials;

        if (betterSolution(cur, best))
        {
            best = cur;
            ++bestUpdates;
            printBestUpdate(best, trial, tk.elapsedMs());
        }

        ++trial;
    }

    while (!tk.timeUp(12))
    {
        long long avgTrialMs = (trial > 0 ? max(1LL, totalTrialMs / trial) : 1LL);

        if (tk.elapsedMs() + avgTrialMs + 8 >= TIME_LIMIT_MS)
        {
            break;
        }

        Phase1Param p = randomParam(rng);

        long long st = tk.elapsedMs();
        SolutionResult cur = buildDirectBatchSolution(initial, trial, p, inputHash, tk, true);
        long long ed = tk.elapsedMs();
        totalTrialMs += max(1LL, ed - st);

        if (cur.ok) ++validTrials;

        if (betterSolution(cur, best))
        {
            best = cur;
            ++bestUpdates;
            printBestUpdate(best, trial, tk.elapsedMs());
        }

        ++trial;
    }

    if (!best.ok)
    {
        cerr << "no valid timed solution, building emergency fallback" << '\n';

        Phase1Param fallback = makeBaseParams(inputHash)[0];
        best = buildDirectBatchSolution(initial, trial, fallback, inputHash, tk, false);

        if (!best.ok)
        {
            cerr << "emergency fallback failed: " << best.validationMsg << '\n';
            exit(1);
        }
    }

    cerr << "final_summary"
         << " trials=" << trial
         << " valid_trials=" << validTrials
         << " best_updates=" << bestUpdates
         << " elapsed_ms=" << tk.elapsedMs()
         << " selected_trial=" << best.trialId
         << " method=" << best.method
         << " turns=" << best.turns.size()
         << " moves=" << best.totalMoves
         << " phase_turns="
         << best.phaseTurns[1] << ','
         << best.phaseTurns[2] << ','
         << best.phaseTurns[3] << ','
         << best.phaseTurns[4]
         << " phase_moves="
         << best.phaseMoves[1] << ','
         << best.phaseMoves[2] << ','
         << best.phaseMoves[3] << ','
         << best.phaseMoves[4]
         << " max_departure_len=" << best.maxDepLen
         << " max_siding_len=" << best.maxSidLen
         << " validation=" << best.validationMsg
         << '\n';

    cout << best.turns.size() << '\n';

    for (const vector<Move> &turn : best.turns)
    {
        cout << turn.size() << '\n';

        for (const Move &m : turn)
        {
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