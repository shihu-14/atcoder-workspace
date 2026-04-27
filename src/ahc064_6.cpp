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

struct ParentInfo
{
    unsigned long long prevKey;
    vector<Move> turn;
};

struct BeamState
{
    array<int, 10> pos;
    unsigned long long key;
    long long score;
};

struct BeamCandidate
{
    array<int, 10> pos;
    unsigned long long key;
    long long score;
    unsigned long long prevKey;
    vector<Move> turn;
};

const int CAR_PER_LINE = 10;
const int DEP_CAP = 15;
const int SID_CAP = 20;

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
    long long bestWeight = -1;
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

unsigned long long encodePos(const array<int, 10> &pos, int R)
{
    unsigned long long key = 0;
    unsigned long long base = 1;

    for (int i = 0; i < R; ++i)
    {
        key += (unsigned long long)pos[i] * base;
        base *= 11ULL;
    }

    return key;
}

void computeNeedGroup(const vector<vector<int>> &orders, const array<int, 10> &pos, int R, vector<int> &need)
{
    need.assign(R, R - 1);

    for (int g = 0; g < R; ++g)
    {
        for (int t = 0; t < pos[g]; ++t)
        {
            int offset = orders[g][t] % CAR_PER_LINE;
            --need[offset];
        }
    }
}

vector<pair<int, int>> getPhase2Pairs(const vector<vector<int>> &orders, const array<int, 10> &pos, int R)
{
    vector<int> need;
    computeNeedGroup(orders, pos, R, need);

    vector<pair<int, int>> pairs;

    for (int g = 0; g < R; ++g)
    {
        if (pos[g] >= R) continue;

        int x = orders[g][pos[g]];
        int offset = x % CAR_PER_LINE;

        if (need[offset] == g)
        {
            pairs.push_back({offset, g});
        }
    }

    return pairs;
}

vector<vector<Move>> enumerateValidPhase2Turns(const vector<pair<int, int>> &pairs, int R)
{
    int n = (int)pairs.size();
    vector<vector<Move>> turns;

    for (int mask = 1; mask < (1 << n); ++mask)
    {
        vector<Move> turn;

        for (int b = 0; b < n; ++b)
        {
            if ((mask >> b) & 1)
            {
                int offset = pairs[b].first;
                int g = pairs[b].second;
                turn.push_back(Move{1, offset, g, 1});
            }
        }

        string err;
        if (!validTurnMoves(turn, R, &err)) continue;

        sort(turn.begin(), turn.end(), [](const Move &a, const Move &b)
        {
            if (a.i != b.i) return a.i < b.i;
            return a.j < b.j;
        });

        turns.push_back(turn);
    }

    sort(turns.begin(), turns.end(), [](const vector<Move> &a, const vector<Move> &b)
    {
        if (a.size() != b.size()) return a.size() > b.size();

        int sa = 0;
        int sb = 0;

        for (const Move &m : a) sa += m.i + m.j;
        for (const Move &m : b) sb += m.i + m.j;

        return sa > sb;
    });

    return turns;
}

long long evaluatePhase2State(const vector<vector<int>> &orders, const array<int, 10> &pos, int R)
{
    int progress = 0;
    int remainingGroups = 0;

    for (int g = 0; g < R; ++g)
    {
        progress += pos[g];
        if (pos[g] < R) ++remainingGroups;
    }

    vector<pair<int, int>> pairs = getPhase2Pairs(orders, pos, R);
    vector<vector<Move>> validTurns = enumerateValidPhase2Turns(pairs, R);

    int candidateCount = (int)pairs.size();
    int bestTurnSize = validTurns.empty() ? 0 : (int)validTurns[0].size();
    int blocking = remainingGroups - candidateCount;

    long long score = 0;
    score += (long long)progress * 1000000000000LL;
    score += (long long)bestTurnSize * 10000000000LL;
    score += (long long)candidateCount * 100000000LL;
    score -= (long long)blocking * 1000000LL;

    for (int g = 0; g < R; ++g)
    {
        score += (long long)pos[g] * (1000 + g);
    }

    return score;
}

bool reconstructBeamPath(
    unsigned long long startKey,
    unsigned long long goalKey,
    const unordered_map<unsigned long long, ParentInfo> &parent,
    vector<vector<Move>> &result
)
{
    vector<vector<Move>> rev;
    unsigned long long cur = goalKey;

    while (cur != startKey)
    {
        auto it = parent.find(cur);

        if (it == parent.end())
        {
            return false;
        }

        rev.push_back(it->second.turn);
        cur = it->second.prevKey;
    }

    reverse(rev.begin(), rev.end());
    result = rev;
    return true;
}

vector<vector<Move>> buildPhase2GreedyTurns(const vector<deque<int>> &sidStart, int R, bool &ok)
{
    vector<vector<int>> orders(R);

    for (int g = 0; g < R; ++g)
    {
        for (int x : sidStart[g])
        {
            orders[g].push_back(x);
        }

        if ((int)orders[g].size() != R)
        {
            ok = false;
            return {};
        }
    }

    array<int, 10> pos{};
    pos.fill(0);

    vector<vector<Move>> turns;
    int guard = 0;

    while (true)
    {
        int progress = 0;

        for (int g = 0; g < R; ++g)
        {
            progress += pos[g];
        }

        if (progress == R * R)
        {
            ok = true;
            return turns;
        }

        if (++guard > 500)
        {
            ok = false;
            return {};
        }

        vector<pair<int, int>> pairs = getPhase2Pairs(orders, pos, R);
        vector<vector<Move>> candTurns = enumerateValidPhase2Turns(pairs, R);

        if (candTurns.empty())
        {
            ok = false;
            return {};
        }

        long long bestScore = LLONG_MIN;
        vector<Move> bestTurn;

        for (const vector<Move> &turn : candTurns)
        {
            array<int, 10> nextPos = pos;

            for (const Move &m : turn)
            {
                ++nextPos[m.j];
            }

            long long score = 0;
            score += (long long)turn.size() * 1000000000000000LL;
            score += evaluatePhase2State(orders, nextPos, R);

            if (score > bestScore)
            {
                bestScore = score;
                bestTurn = turn;
            }
        }

        for (const Move &m : bestTurn)
        {
            ++pos[m.j];
        }

        turns.push_back(bestTurn);
    }
}

vector<vector<Move>> buildPhase2BeamTurns(
    const vector<deque<int>> &sidStart,
    int R,
    int beamWidth,
    bool &ok,
    int &expandedStates,
    int &generatedStates,
    int &maxBeamSize
)
{
    ok = false;
    expandedStates = 0;
    generatedStates = 0;
    maxBeamSize = 0;

    vector<vector<int>> orders(R);

    for (int g = 0; g < R; ++g)
    {
        for (int x : sidStart[g])
        {
            orders[g].push_back(x);
        }

        if ((int)orders[g].size() != R)
        {
            return {};
        }
    }

    array<int, 10> startPos{};
    startPos.fill(0);

    array<int, 10> goalPos{};
    goalPos.fill(R);

    unsigned long long startKey = encodePos(startPos, R);
    unsigned long long goalKey = encodePos(goalPos, R);

    vector<BeamState> beam;
    beam.push_back(BeamState{startPos, startKey, evaluatePhase2State(orders, startPos, R)});

    unordered_map<unsigned long long, ParentInfo> parent;
    unordered_set<unsigned long long> visited;
    parent.reserve(20000);
    visited.reserve(20000);
    visited.insert(startKey);

    maxBeamSize = 1;

    for (int depth = 0; depth <= 120; ++depth)
    {
        unordered_map<unsigned long long, BeamCandidate> nextMap;
        nextMap.reserve((size_t)beamWidth * 64);

        for (const BeamState &st : beam)
        {
            ++expandedStates;

            vector<pair<int, int>> pairs = getPhase2Pairs(orders, st.pos, R);
            vector<vector<Move>> candTurns = enumerateValidPhase2Turns(pairs, R);

            for (const vector<Move> &turn : candTurns)
            {
                array<int, 10> nextPos = st.pos;

                for (const Move &m : turn)
                {
                    ++nextPos[m.j];
                }

                unsigned long long key = encodePos(nextPos, R);

                if (visited.count(key)) continue;

                long long score = evaluatePhase2State(orders, nextPos, R);

                ++generatedStates;

                if (key == goalKey)
                {
                    parent[key] = ParentInfo{st.key, turn};

                    vector<vector<Move>> result;
                    bool recOk = reconstructBeamPath(startKey, goalKey, parent, result);

                    if (recOk)
                    {
                        ok = true;
                        return result;
                    }

                    return {};
                }

                auto it = nextMap.find(key);

                if (it == nextMap.end() || score > it->second.score)
                {
                    BeamCandidate bc;
                    bc.pos = nextPos;
                    bc.key = key;
                    bc.score = score;
                    bc.prevKey = st.key;
                    bc.turn = turn;
                    nextMap[key] = bc;
                }
            }
        }

        if (nextMap.empty())
        {
            break;
        }

        vector<BeamCandidate> nextList;
        nextList.reserve(nextMap.size());

        for (auto &kv : nextMap)
        {
            nextList.push_back(kv.second);
        }

        sort(nextList.begin(), nextList.end(), [](const BeamCandidate &a, const BeamCandidate &b)
        {
            if (a.score != b.score) return a.score > b.score;
            return a.key < b.key;
        });

        if ((int)nextList.size() > beamWidth)
        {
            nextList.resize(beamWidth);
        }

        beam.clear();

        for (const BeamCandidate &bc : nextList)
        {
            visited.insert(bc.key);
            parent[bc.key] = ParentInfo{bc.prevKey, bc.turn};
            beam.push_back(BeamState{bc.pos, bc.key, bc.score});
        }

        chmax(maxBeamSize, (int)beam.size());
    }

    return {};
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

void solve()
{
    int R;
    cin >> R;

    vector<deque<int>> initial(R);

    rep(r, R)
    {
        rep(c, CAR_PER_LINE)
        {
            int x;
            cin >> x;
            initial[r].push_back(x);
        }
    }

    if (R != CAR_PER_LINE)
    {
        cerr << "unsupported R=" << R << '\n';
        exit(1);
    }

    vector<deque<int>> dep = initial;
    vector<deque<int>> sid(R);
    vector<vector<Move>> turns;

    int maxDepLen = CAR_PER_LINE;
    int maxSidLen = 0;
    int totalMoves = 0;
    array<int, 5> phaseTurns{};
    array<int, 5> phaseMoves{};

    auto refreshMaxLen = [&]()
    {
        for (int r = 0; r < R; ++r)
        {
            chmax(maxDepLen, (int)dep[r].size());
            chmax(maxSidLen, (int)sid[r].size());
        }
    };

    auto applyTurn = [&](const vector<Move> &turn, int phase)
    {
        if (turn.empty())
        {
            cerr << "empty turn in phase " << phase << '\n';
            exit(1);
        }

        string turnErr;

        if (!validTurnMoves(turn, R, &turnErr))
        {
            cerr << "invalid generated turn: " << turnErr
                 << " phase=" << phase << '\n';
            exit(1);
        }

        for (const Move &m : turn)
        {
            string err;
            bool ok = applyMoveState(dep, sid, m, &err);

            if (!ok)
            {
                cerr << "internal move error: " << err
                     << " phase=" << phase
                     << " type=" << m.type
                     << " i=" << m.i
                     << " j=" << m.j
                     << " k=" << m.k << '\n';
                exit(1);
            }
        }

        turns.push_back(turn);
        phaseTurns[phase]++;
        phaseMoves[phase] += (int)turn.size();
        totalMoves += (int)turn.size();
        refreshMaxLen();
    };

    /*
        Phase 1:
        ID / 10 によって目的グループへ分類する。
        ただし、Phase 2 で詰まりにくくするため、同じ k なら offset が大きい車両をやや優先する。
        待避線は先頭追加なので、早めに大きい offset を入れるほど、
        後から小さい offset が前に来やすくなる。
    */
    while (true)
    {
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

            int offsetSum = 0;

            for (int p = (int)dep[d].size() - k; p < (int)dep[d].size(); ++p)
            {
                offsetSum += dep[d][p] % CAR_PER_LINE;
            }

            Candidate c;
            c.m = Move{0, d, g, k};
            c.weight = (long long)k * 1000000LL
                     + (long long)offsetSum * 10000LL
                     + d;

            cand.push_back(c);
        }

        if (cand.empty()) break;

        vector<Move> turn = chooseBestTurn(cand, R);

        if (turn.empty())
        {
            cerr << "phase1 deadlock" << '\n';
            exit(1);
        }

        applyTurn(turn, 1);
    }

    /*
        Phase 2:
        最大ボトルネックなので、ここだけビームサーチでターン列を作る。
        状態は「各待避線から何両取り出したか」だけで表現する。
        失敗した場合は安全な貪欲法にフォールバックする。
    */
    const int BEAM_WIDTH = 120;

    bool phase2BeamOk = false;
    int beamExpanded = 0;
    int beamGenerated = 0;
    int maxBeamSize = 0;

    vector<vector<Move>> phase2Turns = buildPhase2BeamTurns(
        sid,
        R,
        BEAM_WIDTH,
        phase2BeamOk,
        beamExpanded,
        beamGenerated,
        maxBeamSize
    );

    string phase2Method = "beam";

    if (!phase2BeamOk)
    {
        bool greedyOk = false;
        phase2Turns = buildPhase2GreedyTurns(sid, R, greedyOk);
        phase2Method = "greedy_fallback";

        if (!greedyOk)
        {
            cerr << "phase2 failed both beam and greedy" << '\n';
            exit(1);
        }
    }

    for (const vector<Move> &turn : phase2Turns)
    {
        applyTurn(turn, 2);
    }

    /*
        Phase 3:
        出発線 offset の末尾から待避線 g へ戻し、
        待避線上に完成形 10g,10g+1,...,10g+9 を作る。
    */
    vector<int> needOffset(R, R - 1);

    while (true)
    {
        vector<Candidate> cand;
        int remaining = 0;

        for (int offset = 0; offset < R; ++offset)
        {
            if (dep[offset].empty()) continue;

            ++remaining;

            int x = dep[offset].back();
            int g = x / CAR_PER_LINE;
            int off = x % CAR_PER_LINE;

            if (off != offset)
            {
                cerr << "phase3 invalid offset line"
                     << " line=" << offset
                     << " value=" << x << '\n';
                exit(1);
            }

            if (needOffset[g] == offset)
            {
                Candidate c;
                c.m = Move{0, offset, g, 1};
                c.weight = 1000000LL + (long long)(R - 1 - g) * 1000LL + offset;
                cand.push_back(c);
            }
        }

        if (remaining == 0) break;

        vector<Move> turn = chooseBestTurn(cand, R);

        if (turn.empty())
        {
            cerr << "phase3 deadlock" << '\n';
            exit(1);
        }

        applyTurn(turn, 3);

        for (const Move &m : turn)
        {
            --needOffset[m.j];
        }
    }

    for (int g = 0; g < R; ++g)
    {
        if (needOffset[g] != -1)
        {
            cerr << "phase3 incomplete group=" << g
                 << " needOffset=" << needOffset[g] << '\n';
            exit(1);
        }
    }

    /*
        Phase 4:
        各待避線には完成形があるため、全て同時に戻す。
    */
    vector<Move> finalTurn;

    for (int g = 0; g < R; ++g)
    {
        if ((int)sid[g].size() != CAR_PER_LINE)
        {
            cerr << "phase4 invalid siding size"
                 << " g=" << g
                 << " size=" << sid[g].size() << '\n';
            exit(1);
        }

        for (int c = 0; c < CAR_PER_LINE; ++c)
        {
            if (sid[g][c] != CAR_PER_LINE * g + c)
            {
                cerr << "phase4 invalid siding order"
                     << " g=" << g
                     << " c=" << c
                     << " value=" << sid[g][c] << '\n';
                exit(1);
            }
        }

        finalTurn.push_back(Move{1, g, g, CAR_PER_LINE});
    }

    applyTurn(finalTurn, 4);

    string msg;
    bool ok = validateTurns(initial, turns, msg);

    cerr << "turns=" << turns.size()
         << " moves=" << totalMoves
         << " phase_turns="
         << phaseTurns[1] << ','
         << phaseTurns[2] << ','
         << phaseTurns[3] << ','
         << phaseTurns[4]
         << " phase_moves="
         << phaseMoves[1] << ','
         << phaseMoves[2] << ','
         << phaseMoves[3] << ','
         << phaseMoves[4]
         << " phase2_method=" << phase2Method
         << " beam_width=" << BEAM_WIDTH
         << " beam_expanded=" << beamExpanded
         << " beam_generated=" << beamGenerated
         << " max_beam_size=" << maxBeamSize
         << " max_departure_len=" << maxDepLen
         << " max_siding_len=" << maxSidLen
         << " validation=" << msg << '\n';

    if (!ok)
    {
        cerr << "validation failed: " << msg << '\n';
        exit(1);
    }

    if ((int)turns.size() > 4000)
    {
        cerr << "too many turns after validation" << '\n';
        exit(1);
    }

    cout << turns.size() << '\n';

    for (const vector<Move> &turn : turns)
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