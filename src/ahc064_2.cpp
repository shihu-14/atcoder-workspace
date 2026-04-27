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

bool canAddToTurn(const vector<Move> &turn, const Move &m, int R)
{
    if ((int)turn.size() >= R) return false;

    for (const Move &a : turn)
    {
        if (a.i == m.i) return false;
        if (a.j == m.j) return false;

        if (a.i < m.i && !(a.j < m.j)) return false;
        if (m.i < a.i && !(m.j < a.j)) return false;
    }

    return true;
}

vector<vector<Move>> packMoves(const vector<Move> &raw, int R)
{
    vector<vector<Move>> turns;
    vector<Move> cur;

    for (const Move &m : raw)
    {
        if (canAddToTurn(cur, m, R))
        {
            cur.push_back(m);
        }
        else
        {
            if (!cur.empty()) turns.push_back(cur);
            cur.clear();
            cur.push_back(m);
        }
    }

    if (!cur.empty()) turns.push_back(cur);

    return turns;
}

vector<vector<Move>> makeSingleTurns(const vector<Move> &raw)
{
    vector<vector<Move>> turns;

    for (const Move &m : raw)
    {
        turns.push_back(vector<Move>{m});
    }

    return turns;
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
        const vector<Move> &turn = turns[t];

        if (turn.empty() || (int)turn.size() > R)
        {
            msg = "invalid K at turn " + to_string(t);
            return false;
        }

        vector<int> usedDep(R, 0);
        vector<int> usedSid(R, 0);

        for (int a = 0; a < (int)turn.size(); ++a)
        {
            const Move &ma = turn[a];

            if (ma.type != 0 && ma.type != 1)
            {
                msg = "invalid type at turn " + to_string(t);
                return false;
            }

            if (ma.i < 0 || ma.i >= R || ma.j < 0 || ma.j >= R || ma.k <= 0)
            {
                msg = "invalid parameter at turn " + to_string(t);
                return false;
            }

            if (usedDep[ma.i] || usedSid[ma.j])
            {
                msg = "line reused at turn " + to_string(t);
                return false;
            }

            usedDep[ma.i] = 1;
            usedSid[ma.j] = 1;

            for (int b = 0; b < a; ++b)
            {
                const Move &mb = turn[b];

                if (mb.i < ma.i && !(mb.j < ma.j))
                {
                    msg = "crossing at turn " + to_string(t);
                    return false;
                }

                if (ma.i < mb.i && !(ma.j < mb.j))
                {
                    msg = "crossing at turn " + to_string(t);
                    return false;
                }
            }
        }

        for (const Move &m : turn)
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
    vector<Move> raw;

    int maxDepLen = CAR_PER_LINE;
    int maxSidLen = 0;

    auto refreshMaxLen = [&]()
    {
        for (int r = 0; r < R; ++r)
        {
            chmax(maxDepLen, (int)dep[r].size());
            chmax(maxSidLen, (int)sid[r].size());
        }
    };

    auto addMove = [&](Move m)
    {
        string err;

        bool ok = applyMoveState(dep, sid, m, &err);

        if (!ok)
        {
            cerr << "internal move error: " << err
                 << " type=" << m.type
                 << " i=" << m.i
                 << " j=" << m.j
                 << " k=" << m.k << '\n';
            exit(1);
        }

        raw.push_back(m);
        refreshMaxLen();
    };

    /*
        Phase 1:
        出発線の末尾から車両を取り出し、
        最終的に行く出発線 id / 10 ごとの待避線へ分類する。

        同じ分類が末尾側に連続している場合は k > 1 でまとめて移動する。
    */
    int beforePhase1 = (int)raw.size();

    for (int d = 0; d < R; ++d)
    {
        while (!dep[d].empty())
        {
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

            addMove(Move{0, d, g, k});
        }
    }

    int phase1Ops = (int)raw.size() - beforePhase1;

    /*
        Phase 2:
        待避線 g から取り出し、id % 10 ごとの出発線へ置く。

        g = 9,8,...,0 の順で処理する。
        これにより、各出発線 offset の末尾側から
        g = 0,1,2,... の順に取り出せる状態になる。
    */
    int beforePhase2 = (int)raw.size();

    for (int g = R - 1; g >= 0; --g)
    {
        while (!sid[g].empty())
        {
            int x = sid[g].front();
            int offset = x % CAR_PER_LINE;

            addMove(Move{1, offset, g, 1});
        }
    }

    int phase2Ops = (int)raw.size() - beforePhase2;

    /*
        Phase 3:
        出発線 offset の末尾には 10*g + offset がある。

        offset = 9,8,...,0 の順に待避線 g の先頭へ入れると、
        待避線 g は 10*g,10*g+1,...,10*g+9 の昇順になる。
    */
    int beforePhase3 = (int)raw.size();

    for (int g = 0; g < R; ++g)
    {
        for (int offset = CAR_PER_LINE - 1; offset >= 0; --offset)
        {
            if (dep[offset].empty() || dep[offset].back() != CAR_PER_LINE * g + offset)
            {
                cerr << "unexpected phase3 state"
                     << " g=" << g
                     << " offset=" << offset;

                if (!dep[offset].empty())
                {
                    cerr << " back=" << dep[offset].back();
                }

                cerr << '\n';
                exit(1);
            }

            addMove(Move{0, offset, g, 1});
        }
    }

    int phase3Ops = (int)raw.size() - beforePhase3;

    /*
        Phase 4:
        待避線 g はすでに正しい昇順なので、
        出発線 g へ 10 両まとめて戻す。
    */
    int beforePhase4 = (int)raw.size();

    for (int g = 0; g < R; ++g)
    {
        if ((int)sid[g].size() != CAR_PER_LINE)
        {
            cerr << "unexpected phase4 siding size"
                 << " g=" << g
                 << " size=" << sid[g].size() << '\n';
            exit(1);
        }

        for (int c = 0; c < CAR_PER_LINE; ++c)
        {
            if (sid[g][c] != CAR_PER_LINE * g + c)
            {
                cerr << "unexpected phase4 order"
                     << " g=" << g
                     << " c=" << c
                     << " value=" << sid[g][c] << '\n';
                exit(1);
            }
        }

        addMove(Move{1, g, g, CAR_PER_LINE});
    }

    int phase4Ops = (int)raw.size() - beforePhase4;

    vector<vector<Move>> turns = packMoves(raw, R);

    string msg;
    bool ok = validateTurns(initial, turns, msg);

    if (!ok)
    {
        cerr << "packed validation failed: " << msg << '\n';

        turns = makeSingleTurns(raw);
        ok = validateTurns(initial, turns, msg);
    }

    cerr << "raw_ops=" << raw.size()
         << " turns=" << turns.size()
         << " phase_ops="
         << phase1Ops << ','
         << phase2Ops << ','
         << phase3Ops << ','
         << phase4Ops
         << " max_departure_len=" << maxDepLen
         << " max_siding_len=" << maxSidLen
         << " validation=" << msg << '\n';

    if (!ok)
    {
        cerr << "raw validation failed: " << msg << '\n';
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