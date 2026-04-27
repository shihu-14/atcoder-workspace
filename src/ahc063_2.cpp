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

namespace {
const int MAX_CELL = 256;
const int TURN_LIMIT = 100000;
const char DIRC[4] = {'U','R','D','L'};

enum PhaseType { PHASE_EXACT = 0, PHASE_GROWTH = 1, PHASE_REPAIR = 2 };

struct DebugStats {
    long long exact_searches = 0, growth_searches = 0, repair_searches = 0;
    long long exact_success = 0, growth_success = 0, repair_success = 0;
    long long fallback_moves = 0;
    long long expanded = 0, generated = 0;
    long long eaten = 0, bites = 0;
    int max_len = 5;
    int best_pen = INF;
};

struct State {
    array<unsigned char, MAX_CELL> board{};
    array<unsigned char, MAX_CELL> snake{};
    array<unsigned char, MAX_CELL> color{};
    array<unsigned short, 8> food_cnt{};
    int len = 0;
    int mis = 0;
    int pref = 0;
    int pen = 0;
};

struct Node {
    array<unsigned char, MAX_CELL> board{};
    array<unsigned char, MAX_CELL> snake{};
    array<unsigned char, MAX_CELL> color{};
    array<unsigned short, 8> food_cnt{};
    int len = 0;
    int mis = 0;
    int pref = 0;
    int pen = 0;
    int depth = 0;
    int prev = -1;
    char mv = '?';
    int dist_next = INF;
    int dist_any = INF;
    int excess = 0;
    int bites = 0;
    ull hash = 0;
};

struct SearchResult {
    vector<char> path;
    bool goal = false;
    int pen = INF;
    int len = 0;
    int pref = 0;
};

int N_, M_, C_;
vector<int> desired_;
DebugStats dbg_;
chrono::steady_clock::time_point start_clock_;
}

inline int id_of(int r, int c){ return r * N_ + c; }
inline int row_of(int id){ return id / N_; }
inline int col_of(int id){ return id % N_; }
inline int dir_from_char(char ch){ if(ch=='U') return 0; if(ch=='R') return 1; if(ch=='D') return 2; return 3; }
inline double elapsed_ms(){ return chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_clock_).count(); }
inline int next_target_color(int len){ return len < M_ ? desired_[len] : 0; }

template<class T> inline void hash_combine_u64(ull& seed, T v) {
    ull x = (ull)v + 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x ^= (x >> 31);
    seed ^= x + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
}

inline ull signature_of(const array<unsigned char, MAX_CELL>& snake,
                        const array<unsigned char, MAX_CELL>& color,
                        const array<unsigned short, 8>& food_cnt,
                        int len, int pref, int pen){
    ull h = 1469598103934665603ULL;
    hash_combine_u64(h, len);
    hash_combine_u64(h, pref);
    hash_combine_u64(h, pen);
    int lims = min(len, 10);
    for(int i = 0; i < lims; ++i) hash_combine_u64(h, (int)snake[i]);
    int limc = min(len, 16);
    for(int i = 0; i < limc; ++i) hash_combine_u64(h, (int)color[i]);
    for(int c = 1; c <= C_; ++c) hash_combine_u64(h, (int)food_cnt[c]);
    if(len > 0) hash_combine_u64(h, (int)snake[len - 1]);
    return h;
}

inline void calc_metrics(const array<unsigned char, MAX_CELL>& color, int len, int& mis, int& pref, int& pen){
    mis = 0;
    pref = 0;
    bool broken = false;
    for(int i = 0; i < len; ++i){
        if((int)color[i] != desired_[i]){
            ++mis;
            broken = true;
        } else if(!broken) {
            ++pref;
        }
    }
    pen = mis + 2 * (M_ - len);
}

inline int count_all_food_on_board(const array<unsigned short, 8>& food_cnt){
    int s = 0;
    for(int c = 1; c <= C_; ++c) s += food_cnt[c];
    return s;
}

inline int dist_to_color(const array<unsigned char, MAX_CELL>& board,
                         const array<unsigned short, 8>& food_cnt,
                         const array<unsigned char, MAX_CELL>& snake,
                         int col){
    if(col <= 0) return 0;
    if(food_cnt[col] == 0) return INF;
    int h = snake[0];
    int hr = row_of(h), hc = col_of(h);
    int best = INF;
    for(int i = 0; i < N_ * N_; ++i){
        if((int)board[i] == col) chmin(best, abs(hr - row_of(i)) + abs(hc - col_of(i)));
    }
    return best;
}

inline int dist_to_any_food(const array<unsigned char, MAX_CELL>& board,
                            const array<unsigned short, 8>& food_cnt,
                            const array<unsigned char, MAX_CELL>& snake){
    if(count_all_food_on_board(food_cnt) == 0) return INF;
    int h = snake[0];
    int hr = row_of(h), hc = col_of(h);
    int best = INF;
    for(int i = 0; i < N_ * N_; ++i){
        if(board[i] != 0) chmin(best, abs(hr - row_of(i)) + abs(hc - col_of(i)));
    }
    return best;
}

bool apply_move_core(const array<unsigned char, MAX_CELL>& src_board,
                     const array<unsigned char, MAX_CELL>& src_snake,
                     const array<unsigned char, MAX_CELL>& src_color,
                     const array<unsigned short, 8>& src_food_cnt,
                     int src_len,
                     int dir,
                     array<unsigned char, MAX_CELL>& dst_board,
                     array<unsigned char, MAX_CELL>& dst_snake,
                     array<unsigned char, MAX_CELL>& dst_color,
                     array<unsigned short, 8>& dst_food_cnt,
                     int& dst_len,
                     int& dst_mis,
                     int& dst_pref,
                     int& dst_pen,
                     bool& ate,
                     int& eaten_color,
                     bool& bit,
                     bool count_debug){
    int head = src_snake[0];
    int nr = row_of(head) + (int)dx[dir];
    int nc = col_of(head) + (int)dy[dir];
    if(nr < 0 || nr >= N_ || nc < 0 || nc >= N_) return false;
    int nh = id_of(nr, nc);
    if(src_len >= 2 && nh == (int)src_snake[1]) return false;

    dst_board = src_board;
    dst_snake = src_snake;
    dst_color = src_color;
    dst_food_cnt = src_food_cnt;
    dst_len = src_len;
    ate = false;
    eaten_color = 0;
    bit = false;

    dst_snake[0] = (unsigned char)nh;
    for(int i = 1; i < src_len; ++i) dst_snake[i] = src_snake[i - 1];

    unsigned char food = src_board[nh];
    if(food != 0){
        if(src_len >= M_) return false;
        ate = true;
        eaten_color = (int)food;
        dst_board[nh] = 0;
        if(dst_food_cnt[food] == 0) return false;
        --dst_food_cnt[food];
        dst_snake[src_len] = src_snake[src_len - 1];
        dst_color[src_len] = food;
        dst_len = src_len + 1;
        if(count_debug) ++dbg_.eaten;
    }

    if(!ate){
        int h = -1;
        for(int i = 1; i <= src_len - 2; ++i){
            if(dst_snake[i] == (unsigned char)nh){
                h = i;
                break;
            }
        }
        if(h != -1){
            bit = true;
            for(int p = h + 1; p < src_len; ++p){
                int cell = dst_snake[p];
                unsigned char col = dst_color[p];
                dst_board[cell] = col;
                ++dst_food_cnt[col];
            }
            dst_len = h + 1;
            if(count_debug) ++dbg_.bites;
        }
    }

    calc_metrics(dst_color, dst_len, dst_mis, dst_pref, dst_pen);
    if(count_debug){
        chmax(dbg_.max_len, dst_len);
        chmin(dbg_.best_pen, dst_pen);
    }
    return true;
}

bool apply_move_state(const State& src, int dir, State& dst, bool count_debug){
    bool ate, bit;
    int eaten_color;
    return apply_move_core(src.board, src.snake, src.color, src.food_cnt, src.len, dir,
                           dst.board, dst.snake, dst.color, dst.food_cnt,
                           dst.len, dst.mis, dst.pref, dst.pen,
                           ate, eaten_color, bit, count_debug);
}

Node make_node(const array<unsigned char, MAX_CELL>& board,
               const array<unsigned char, MAX_CELL>& snake,
               const array<unsigned char, MAX_CELL>& color,
               const array<unsigned short, 8>& food_cnt,
               int len, int mis, int pref, int pen,
               int depth, int prev, char mv, int bites){
    Node nd;
    nd.board = board;
    nd.snake = snake;
    nd.color = color;
    nd.food_cnt = food_cnt;
    nd.len = len;
    nd.mis = mis;
    nd.pref = pref;
    nd.pen = pen;
    nd.depth = depth;
    nd.prev = prev;
    nd.mv = mv;
    nd.bites = bites;
    nd.excess = len - pref;
    nd.dist_next = dist_to_color(board, food_cnt, snake, next_target_color(len));
    nd.dist_any = dist_to_any_food(board, food_cnt, snake);
    nd.hash = signature_of(snake, color, food_cnt, len, pref, pen);
    return nd;
}

bool better_exact(const Node& A, const Node& B){
    if(A.pref != B.pref) return A.pref > B.pref;
    if(A.excess != B.excess) return A.excess < B.excess;
    if(A.pen != B.pen) return A.pen < B.pen;
    if(A.len != B.len) return A.len > B.len;
    if(A.dist_next != B.dist_next) return A.dist_next < B.dist_next;
    if(A.bites != B.bites) return A.bites < B.bites;
    return A.depth < B.depth;
}

bool better_growth(const Node& A, const Node& B){
    if(A.pen != B.pen) return A.pen < B.pen;
    if(A.len != B.len) return A.len > B.len;
    if(A.mis != B.mis) return A.mis < B.mis;
    if(A.bites != B.bites) return A.bites < B.bites;
    if(A.dist_any != B.dist_any) return A.dist_any < B.dist_any;
    if(A.dist_next != B.dist_next) return A.dist_next < B.dist_next;
    if(A.pref != B.pref) return A.pref > B.pref;
    return A.depth < B.depth;
}

bool better_repair(const Node& A, const Node& B){
    if(A.pen != B.pen) return A.pen < B.pen;
    if(A.mis != B.mis) return A.mis < B.mis;
    if(A.pref != B.pref) return A.pref > B.pref;
    if(A.len != B.len) return A.len > B.len;
    if(A.bites != B.bites) return A.bites < B.bites;
    if(A.dist_next != B.dist_next) return A.dist_next < B.dist_next;
    return A.depth < B.depth;
}

vector<char> reconstruct_path(const vector<Node>& nodes, int idx){
    vector<char> path;
    while(idx > 0){
        path.push_back(nodes[idx].mv);
        idx = nodes[idx].prev;
    }
    reverse(rng(path));
    return path;
}

SearchResult beam_search(const State& root, PhaseType phase, int turn_remain){
    SearchResult ret;
    ret.pen = root.pen;
    ret.len = root.len;
    ret.pref = root.pref;

    if(phase == PHASE_EXACT) ++dbg_.exact_searches;
    else if(phase == PHASE_GROWTH) ++dbg_.growth_searches;
    else ++dbg_.repair_searches;

    int depth_limit = 18;
    int beam_width = 180;
    if(phase == PHASE_EXACT){ depth_limit = 26; beam_width = 260; }
    if(phase == PHASE_GROWTH){ depth_limit = 20; beam_width = 240; }
    if(phase == PHASE_REPAIR){ depth_limit = 22; beam_width = 220; }
    if(N_ <= 10){ depth_limit += 8; beam_width += 140; }
    if(phase == PHASE_GROWTH && root.len <= 14){ depth_limit += 8; beam_width += 140; }
    if(phase == PHASE_EXACT && root.len <= 16){ depth_limit += 8; beam_width += 120; }
    if(root.pen <= 12) depth_limit += 4;
    depth_limit = min(depth_limit, turn_remain);
    if(depth_limit <= 0) return ret;

    auto better = [&](const Node& A, const Node& B){
        if(phase == PHASE_EXACT) return better_exact(A, B);
        if(phase == PHASE_GROWTH) return better_growth(A, B);
        return better_repair(A, B);
    };

    vector<Node> nodes;
    nodes.reserve(1 + depth_limit * beam_width * 4 + 16);
    nodes.push_back(make_node(root.board, root.snake, root.color, root.food_cnt,
                              root.len, root.mis, root.pref, root.pen, 0, -1, '?', 0));

    vector<int> beam{0};
    int best_goal = -1;
    int best_any = 0;

    for(int dep = 0; dep < depth_limit; ++dep){
        if(elapsed_ms() > 1850.0) break;
        vector<int> nxt;
        nxt.reserve((int)beam.size() * 4);

        for(int idx : beam){
            ++dbg_.expanded;
            const Node& cur = nodes[idx];
            for(int dir = 0; dir < 4; ++dir){
                array<unsigned char, MAX_CELL> nb, ns, nc;
                array<unsigned short, 8> ncnt;
                int nl, nm, np, npen, eaten_color;
                bool ate, bit;
                if(!apply_move_core(cur.board, cur.snake, cur.color, cur.food_cnt, cur.len, dir,
                                    nb, ns, nc, ncnt, nl, nm, np, npen, ate, eaten_color, bit, false)) continue;
                Node child = make_node(nb, ns, nc, ncnt, nl, nm, np, npen,
                                       cur.depth + 1, idx, DIRC[dir], cur.bites + (bit ? 1 : 0));
                nodes.push_back(child);
                int nid = (int)nodes.size() - 1;
                nxt.push_back(nid);
                ++dbg_.generated;
                if(better(nodes[nid], nodes[best_any])) best_any = nid;

                bool goal = false;
                if(phase == PHASE_EXACT) goal = (child.pref > root.pref);
                else if(phase == PHASE_GROWTH) goal = (child.len > root.len || child.pen < root.pen);
                else goal = (child.pen < root.pen);
                if(goal){
                    if(best_goal == -1 || better(nodes[nid], nodes[best_goal])) best_goal = nid;
                }
            }
        }

        if(nxt.empty()) break;
        if(best_goal != -1 && dep >= 1) break;

        sort(rng(nxt), [&](int a, int b){ return better(nodes[a], nodes[b]); });

        vector<int> picked;
        picked.reserve(beam_width);
        unordered_set<ull> seen;
        seen.reserve(beam_width * 3 + 16);
        unordered_map<int,int> head_cap;
        head_cap.reserve(beam_width * 2 + 16);

        for(int idx : nxt){
            ull h = nodes[idx].hash;
            if(seen.find(h) != seen.end()) continue;
            int head = nodes[idx].snake[0];
            int lim = max(2, beam_width / 40);
            if(phase == PHASE_EXACT) lim = max(3, beam_width / 30);
            if(head_cap[head] >= lim) continue;
            seen.insert(h);
            head_cap[head]++;
            picked.push_back(idx);
            if((int)picked.size() >= beam_width) break;
        }
        if(picked.empty()) break;
        beam.swap(picked);
    }

    int chosen = -1;
    if(best_goal != -1){
        chosen = best_goal;
        ret.goal = true;
        if(phase == PHASE_EXACT) ++dbg_.exact_success;
        else if(phase == PHASE_GROWTH) ++dbg_.growth_success;
        else ++dbg_.repair_success;
    } else if(best_any != 0) {
        const Node& root_node = nodes[0];
        const Node& cand = nodes[best_any];
        bool take = false;
        if(phase == PHASE_EXACT) take = better(cand, root_node);
        else if(phase == PHASE_GROWTH) {
            if(cand.pen < root.pen) take = true;
            else if(cand.len > root.len) take = true;
            else if(cand.dist_any < root_node.dist_any) take = true;
            else if(cand.dist_next < root_node.dist_next) take = true;
        } else {
            take = better(cand, root_node);
        }
        if(take) chosen = best_any;
    }

    if(chosen <= 0) return ret;
    ret.path = reconstruct_path(nodes, chosen);
    ret.pen = nodes[chosen].pen;
    ret.len = nodes[chosen].len;
    ret.pref = nodes[chosen].pref;
    return ret;
}

int choose_fallback_dir(const State& cur, PhaseType phase){
    bool found = false;
    Node best;
    int best_dir = -1;
    for(int dir = 0; dir < 4; ++dir){
        array<unsigned char, MAX_CELL> nb, ns, nc;
        array<unsigned short, 8> ncnt;
        int nl, nm, np, npen, eaten_color;
        bool ate, bit;
        if(!apply_move_core(cur.board, cur.snake, cur.color, cur.food_cnt, cur.len, dir,
                            nb, ns, nc, ncnt, nl, nm, np, npen, ate, eaten_color, bit, false)) continue;
        Node cand = make_node(nb, ns, nc, ncnt, nl, nm, np, npen, 1, -1, DIRC[dir], bit ? 1 : 0);
        if(!found){
            found = true;
            best = cand;
            best_dir = dir;
        } else {
            bool take = false;
            if(phase == PHASE_EXACT) take = better_exact(cand, best);
            else if(phase == PHASE_GROWTH) take = better_growth(cand, best);
            else take = better_repair(cand, best);
            if(take){
                best = cand;
                best_dir = dir;
            }
        }
    }
    return best_dir;
}

void update_best_snapshot(const string& answer, const State& cur,
                          ll& best_score, int& best_turn,
                          int& best_len_snapshot, int& best_pen_snapshot){
    ll cur_score = (ll)answer.size() + 10000LL * (ll)cur.pen;
    if(cur_score < best_score){
        best_score = cur_score;
        best_turn = (int)answer.size();
        best_len_snapshot = cur.len;
        best_pen_snapshot = cur.pen;
    }
}

void execute_path(const vector<char>& path, State& cur, string& answer, PhaseType phase,
                  ll& best_score, int& best_turn, int& best_len_snapshot, int& best_pen_snapshot){
    int old_pen = cur.pen;
    int old_len = cur.len;
    int old_pref = cur.pref;
    int old_mis = cur.mis;

    int max_step = 8;
    if(phase == PHASE_EXACT) max_step = 14;
    else if(phase == PHASE_GROWTH) max_step = 8;
    else max_step = 8;

    int step = 0;
    for(char ch : path){
        if((int)answer.size() >= TURN_LIMIT) break;
        if(elapsed_ms() > 1850.0) break;
        int dir = dir_from_char(ch);
        State nxt;
        if(!apply_move_state(cur, dir, nxt, true)) break;
        answer.push_back(ch);
        cur = nxt;
        ++step;
        update_best_snapshot(answer, cur, best_score, best_turn, best_len_snapshot, best_pen_snapshot);

        if(phase == PHASE_EXACT){
            if(cur.pref > old_pref) break;
        } else if(phase == PHASE_GROWTH){
            if(cur.len > old_len || cur.pen < old_pen) break;
        } else {
            if(cur.pen < old_pen || cur.mis < old_mis) break;
        }
        if(step >= max_step) break;
    }
}

void solve()
{
    cin >> N_ >> M_ >> C_;
    desired_.assign(M_, 0);
    rep(i, M_) cin >> desired_[i];
    start_clock_ = chrono::steady_clock::now();

    State cur;
    rep(i, MAX_CELL){
        cur.board[i] = 0;
        cur.snake[i] = 0;
        cur.color[i] = 0;
    }
    for(int c = 0; c < 8; ++c) cur.food_cnt[c] = 0;

    rep(i, N_) rep(j, N_){
        int x; cin >> x;
        cur.board[id_of((int)i, (int)j)] = (unsigned char)x;
        if(x != 0) ++cur.food_cnt[x];
    }

    cur.len = 5;
    cur.snake[0] = id_of(4, 0);
    cur.snake[1] = id_of(3, 0);
    cur.snake[2] = id_of(2, 0);
    cur.snake[3] = id_of(1, 0);
    cur.snake[4] = id_of(0, 0);
    rep(i, 5) cur.color[i] = 1;

    int init_cells[5] = {id_of(4,0), id_of(3,0), id_of(2,0), id_of(1,0), id_of(0,0)};
    for(int t = 0; t < 5; ++t){
        int cell = init_cells[t];
        int x = cur.board[cell];
        if(x != 0){
            --cur.food_cnt[x];
            cur.board[cell] = 0;
        }
    }

    calc_metrics(cur.color, cur.len, cur.mis, cur.pref, cur.pen);
    dbg_.best_pen = cur.pen;
    dbg_.max_len = cur.len;

    string answer;
    answer.reserve(TURN_LIMIT);

    ll best_score = 10000LL * (ll)cur.pen;
    int best_turn = 0;
    int best_len_snapshot = cur.len;
    int best_pen_snapshot = cur.pen;

    int exact_fail_streak = 0;
    int repair_no_improve = 0;

    while((int)answer.size() < TURN_LIMIT){
        if(elapsed_ms() > 1950.0) break;
        if(cur.pen == 0) break;

        PhaseType phase = PHASE_GROWTH;
        if(cur.pref == cur.len && cur.len < M_ && exact_fail_streak < 4 && elapsed_ms() < 1300.0){
            phase = PHASE_EXACT;
        } else if(cur.len >= M_ - 2 && elapsed_ms() > 900.0){
            phase = PHASE_REPAIR;
        } else if(cur.len >= M_ && elapsed_ms() > 700.0){
            phase = PHASE_REPAIR;
        } else {
            phase = PHASE_GROWTH;
        }

        SearchResult sr = beam_search(cur, phase, TURN_LIMIT - (int)answer.size());
        if(!sr.path.empty()){
            int old_pen = cur.pen;
            int old_pref = cur.pref;
            execute_path(sr.path, cur, answer, phase,
                         best_score, best_turn, best_len_snapshot, best_pen_snapshot);
            if(phase == PHASE_EXACT){
                if(cur.pref > old_pref) exact_fail_streak = 0;
                else ++exact_fail_streak;
            } else if(phase == PHASE_REPAIR){
                if(cur.pen < old_pen) repair_no_improve = 0;
                else ++repair_no_improve;
            } else {
                if(cur.pref == cur.len) exact_fail_streak = 0;
            }
        } else {
            int dir = choose_fallback_dir(cur, phase);
            if(dir == -1) break;
            vector<char> one(1, DIRC[dir]);
            int old_pen = cur.pen;
            int old_pref = cur.pref;
            execute_path(one, cur, answer, phase,
                         best_score, best_turn, best_len_snapshot, best_pen_snapshot);
            ++dbg_.fallback_moves;
            if(phase == PHASE_EXACT){
                if(cur.pref > old_pref) exact_fail_streak = 0;
                else ++exact_fail_streak;
            } else if(phase == PHASE_REPAIR){
                if(cur.pen < old_pen) repair_no_improve = 0;
                else ++repair_no_improve;
            }
        }

        if(cur.len < M_ && count_all_food_on_board(cur.food_cnt) == 0) break;
        if(phase == PHASE_REPAIR && repair_no_improve >= 25) break;
    }

#ifdef LOCAL
    cerr << "turns=" << answer.size()
         << " len=" << cur.len
         << " mis=" << cur.mis
         << " pref=" << cur.pref
         << " pen=" << cur.pen
         << " board_food=" << count_all_food_on_board(cur.food_cnt)
         << " exact_searches=" << dbg_.exact_searches
         << " growth_searches=" << dbg_.growth_searches
         << " repair_searches=" << dbg_.repair_searches
         << " exact_success=" << dbg_.exact_success
         << " growth_success=" << dbg_.growth_success
         << " repair_success=" << dbg_.repair_success
         << " fallback_moves=" << dbg_.fallback_moves
         << " expanded=" << dbg_.expanded
         << " generated=" << dbg_.generated
         << " eaten=" << dbg_.eaten
         << " bites=" << dbg_.bites
         << " max_len=" << dbg_.max_len
         << " best_pen=" << dbg_.best_pen
         << " best_turn=" << best_turn
         << " best_len=" << best_len_snapshot
         << " best_pen_snapshot=" << best_pen_snapshot
         << " best_score=" << best_score
         << " elapsed_ms=" << elapsed_ms() << '\n';
#endif

    for(int i = 0; i < best_turn; ++i) cout << answer[i] << '\n';
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