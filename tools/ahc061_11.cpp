#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <array>
#include <cstdint>

using namespace std;

// -----------------------------
// RNG (Xorshift64)
// -----------------------------
struct Xorshift {
    uint64_t x;
    Xorshift(uint64_t seed=123456789ULL) : x(seed ? seed : 123456789ULL) {}
    inline uint64_t next_u64() {
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        return x;
    }
    inline int next_int(int n) {
        if (n <= 0) return 0;
        return (int)(next_u64() % (uint64_t)n);
    }
    inline double next_double() {
        // [0,1)
        return (next_u64() >> 11) * (1.0 / 9007199254740992.0);
    }
};

static inline uint64_t splitmix64(uint64_t &x) {
    uint64_t z = (x += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

constexpr int MAX_N = 10;
constexpr int MAX_M = 10;
constexpr int MAX_CAND = 24;

int N, M, T, U;
int V[MAX_N][MAX_N];

// -----------------------------
// Timer
// -----------------------------
struct Timer {
    chrono::high_resolution_clock::time_point start;
    Timer() { reset(); }
    void reset() { start = chrono::high_resolution_clock::now(); }
    double get_sec() const {
        auto now = chrono::high_resolution_clock::now();
        return chrono::duration_cast<chrono::milliseconds>(now - start).count() / 1000.0;
    }
} global_timer;

// -----------------------------
// FastQueue (fixed capacity)
// -----------------------------
template<typename T, int CAP>
struct FastQueue {
    T data[CAP];
    int head = 0, tail = 0;
    inline void clear() { head = 0; tail = 0; }
    inline void push(const T &v) { data[tail++] = v; }
    inline T pop() { return data[head++]; }
    inline bool empty() const { return head == tail; }
};

// -----------------------------
// State
// -----------------------------
struct State {
    int pos_r[MAX_M];
    int pos_c[MAX_M];
    int owner[MAX_N][MAX_N];
    int level[MAX_N][MAX_N];

    void init_empty() {
        for (int r=0; r<N; ++r) {
            for (int c=0; c<N; ++c) {
                owner[r][c] = -1;
                level[r][c] = 0;
            }
        }
    }

    inline void compute_scores(long long scores_out[MAX_M]) const {
        for (int i=0; i<MAX_M; ++i) scores_out[i] = 0;
        for (int r=0; r<N; ++r) {
            for (int c=0; c<N; ++c) {
                int o = owner[r][c];
                if (o != -1) scores_out[o] += 1LL * V[r][c] * level[r][c];
            }
        }
    }

    double get_score_objective() const {
        long long scores[MAX_M];
        compute_scores(scores);

        int territory_count0 = 0;
        int level_sum0 = 0;
        for (int r=0; r<N; ++r) {
            for (int c=0; c<N; ++c) {
                if (owner[r][c] == 0) {
                    territory_count0++;
                    level_sum0 += level[r][c];
                }
            }
        }

        long long s0 = scores[0];
        long long max_other = 1;
        for (int i=1; i<M; ++i) max_other = max(max_other, scores[i]);

        double score = log((double)s0 + 1.0) - log((double)max_other + 1.0);
        score += territory_count0 * 0.0001;
        score += level_sum0 * 0.00001;
        return score;
    }
};

struct Move { int r, c; };

// -----------------------------
// Valid moves
// -----------------------------
static uint32_t visited_token[MAX_N][MAX_N];
static uint32_t current_token = 1;
static uint32_t added_token[MAX_N][MAX_N];
static uint32_t current_added_token = 1;

static FastQueue<pair<int,int>, MAX_N*MAX_N> bfs_q;
static vector<pair<int,int>> reachable_territory;

static inline void token_bump(uint32_t &tok, uint32_t grid[MAX_N][MAX_N]) {
    tok++;
    if (tok == 0) { // overflow wrap
        memset(grid, 0, sizeof(uint32_t)*MAX_N*MAX_N);
        tok = 1;
    }
}

static inline void get_valid_moves(const State& s, int p_id, vector<Move>& out_moves) {
    out_moves.clear();

    token_bump(current_token, visited_token);
    const uint32_t tok = current_token;

    int start_r = s.pos_r[p_id];
    int start_c = s.pos_c[p_id];
    if (start_r < 0 || start_r >= N || start_c < 0 || start_c >= N) {
        out_moves.push_back({0,0});
        return;
    }

    bool is_occupied[MAX_N][MAX_N];
    memset(is_occupied, 0, sizeof(is_occupied));
    for (int i=0; i<M; ++i) {
        if (i == p_id) continue;
        int r = s.pos_r[i], c = s.pos_c[i];
        if (0 <= r && r < N && 0 <= c && c < N) is_occupied[r][c] = true;
    }

    static const int dr[4] = {0, 0, 1, -1};
    static const int dc[4] = {1, -1, 0, 0};

    bfs_q.clear();
    bfs_q.push({start_r, start_c});
    visited_token[start_r][start_c] = tok;

    reachable_territory.clear();
    reachable_territory.push_back({start_r, start_c});

    while (!bfs_q.empty()) {
        auto cur = bfs_q.pop();
        for (int k=0; k<4; ++k) {
            int nr = cur.first + dr[k];
            int nc = cur.second + dc[k];
            if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                if (visited_token[nr][nc] != tok && s.owner[nr][nc] == p_id) {
                    visited_token[nr][nc] = tok;
                    reachable_territory.push_back({nr, nc});
                    bfs_q.push({nr, nc});
                }
            }
        }
    }

    token_bump(current_added_token, added_token);
    const uint32_t atok = current_added_token;

    for (auto &t : reachable_territory) {
        int r = t.first, c = t.second;
        if (added_token[r][c] != atok && !is_occupied[r][c]) {
            added_token[r][c] = atok;
            out_moves.push_back({r,c});
        }
        for (int k=0; k<4; ++k) {
            int nr = r + dr[k];
            int nc = c + dc[k];
            if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                if (added_token[nr][nc] != atok && !is_occupied[nr][nc]) {
                    added_token[nr][nc] = atok;
                    out_moves.push_back({nr,nc});
                }
            }
        }
    }

    if (out_moves.empty()) out_moves.push_back({start_r, start_c});
}

// -----------------------------
// Exact advance (rules-correct)
// -----------------------------
static inline void advance_exact(State& s, const array<Move, MAX_M>& moves) {
    // Save start positions for "collected pieces return"
    int start_r[MAX_M], start_c[MAX_M];
    for (int p=0; p<M; ++p) {
        start_r[p] = s.pos_r[p];
        start_c[p] = s.pos_c[p];
    }

    // target lists
    static int target_count[MAX_N][MAX_N];
    static int target_list[MAX_N][MAX_N][MAX_M];
    for (int r=0; r<N; ++r) {
        memset(target_count[r], 0, sizeof(int)*N);
    }

    for (int p=0; p<M; ++p) {
        int r = moves[p].r;
        int c = moves[p].c;
        target_list[r][c][target_count[r][c]++] = p;
    }

    bool collected[MAX_M];
    memset(collected, 0, sizeof(collected));

    // conflict resolution and tentative move
    int new_r[MAX_M], new_c[MAX_M];
    for (int p=0; p<M; ++p) {
        new_r[p] = start_r[p];
        new_c[p] = start_c[p];
    }

    for (int r=0; r<N; ++r) {
        for (int c=0; c<N; ++c) {
            int cnt = target_count[r][c];
            if (cnt == 0) continue;

            int cell_owner = s.owner[r][c];
            int winner = -1;

            if (cnt == 1) {
                winner = target_list[r][c][0];
            } else {
                if (cell_owner != -1) {
                    for (int k=0; k<cnt; ++k) {
                        int p = target_list[r][c][k];
                        if (p == cell_owner) { winner = p; break; }
                    }
                }
                // else winner stays -1 (all collected)
            }

            if (winner == -1) {
                for (int k=0; k<cnt; ++k) collected[target_list[r][c][k]] = true;
            } else {
                for (int k=0; k<cnt; ++k) {
                    int p = target_list[r][c][k];
                    if (p != winner) collected[p] = true;
                }
                new_r[winner] = r;
                new_c[winner] = c;
            }
        }
    }

    // apply territory update for non-collected pieces
    for (int p=0; p<M; ++p) {
        if (collected[p]) continue;

        int r = new_r[p];
        int c = new_c[p];

        int o = s.owner[r][c];
        if (o == -1) {
            // occupy
            s.owner[r][c] = p;
            s.level[r][c] = 1;
        } else if (o == p) {
            // strengthen
            if (s.level[r][c] < U) s.level[r][c]++;
        } else {
            // attack
            s.level[r][c]--;
            if (s.level[r][c] == 0) {
                s.owner[r][c] = p;
                s.level[r][c] = 1;
            } else {
                // attacker collected (returns to start position)
                collected[p] = true;
            }
        }
    }

    // finalize positions: collected return to start, others stay at new
    for (int p=0; p<M; ++p) {
        if (collected[p]) {
            s.pos_r[p] = start_r[p];
            s.pos_c[p] = start_c[p];
        } else {
            s.pos_r[p] = new_r[p];
            s.pos_c[p] = new_c[p];
        }
    }
}

// -----------------------------
// AI model (particle filter)
// -----------------------------
struct AIParam {
    double wa, wb, wc, wd, eps;
};

struct AIParticle {
    AIParam par;
    double w;
};

struct AIModel {
    static constexpr int P = 24;
    array<AIParticle, P> ps;

    void init(Xorshift &rng) {
        double invP = 1.0 / P;
        for (int i=0; i<P; ++i) {
            ps[i].par.wa  = 0.3 + rng.next_double() * 0.7;
            ps[i].par.wb  = 0.3 + rng.next_double() * 0.7;
            ps[i].par.wc  = 0.3 + rng.next_double() * 0.7;
            ps[i].par.wd  = 0.3 + rng.next_double() * 0.7;
            ps[i].par.eps = 0.1 + rng.next_double() * 0.4;
            ps[i].w = invP;
        }
    }

    static inline int cell_type(const State &s, int p, int r, int c) {
        int o = s.owner[r][c];
        if (o == -1) return 0; // empty
        if (o == p) {
            if (s.level[r][c] < U) return 1; // own < U
            return 2; // own == U
        }
        // other
        if (s.level[r][c] == 1) return 3;
        return 4;
    }

    static inline double eval_cell(const AIParam &par, const State &s, int p, int r, int c) {
        int t = cell_type(s, p, r, c);
        if (t == 2) return 0.0; // own==U
        double mult = 0.0;
        if (t == 0) mult = par.wa;
        else if (t == 1) mult = par.wb;
        else if (t == 3) mult = par.wc;
        else mult = par.wd;
        return (double)V[r][c] * mult;
    }

    void observe(const State &turn_start, int p, int obs_r, int obs_c, const vector<Move> &B) {
        if (B.empty()) return;

        // check if observed move is in B
        bool inB = false;
        for (auto &m : B) {
            if (m.r == obs_r && m.c == obs_c) { inB = true; break; }
        }
        if (!inB) return;

        // update weights by likelihood
        double wsum = 0.0;
        const double invB = 1.0 / (double)B.size();

        for (int i=0; i<P; ++i) {
            const AIParam &par = ps[i].par;

            // greedy argmax set
            double best = -1e100;
            int argcnt = 0;
            bool obs_is_arg = false;

            for (auto &m : B) {
                double e = eval_cell(par, turn_start, p, m.r, m.c);
                if (e > best + 1e-12) {
                    best = e;
                    argcnt = 1;
                    obs_is_arg = (m.r == obs_r && m.c == obs_c);
                } else if (fabs(e - best) <= 1e-12) {
                    argcnt++;
                    if (m.r == obs_r && m.c == obs_c) obs_is_arg = true;
                }
            }

            double p_greedy = 0.0;
            if (obs_is_arg && argcnt > 0) p_greedy = 1.0 / (double)argcnt;

            double prob = par.eps * invB + (1.0 - par.eps) * p_greedy;
            if (prob < 1e-15) prob = 1e-15;
            ps[i].w *= prob;
            wsum += ps[i].w;
        }

        if (wsum <= 0.0) {
            // reset
            double invP = 1.0 / P;
            for (int i=0; i<P; ++i) ps[i].w = invP;
            return;
        }

        for (int i=0; i<P; ++i) ps[i].w /= wsum;

        // resample if ESS too small
        double ess_inv = 0.0;
        for (int i=0; i<P; ++i) ess_inv += ps[i].w * ps[i].w;
        double ess = 1.0 / max(1e-18, ess_inv);
        if (ess >= P * 0.5) return;

        // systematic resampling
        array<AIParticle, P> newps;
        double step = 1.0 / P;

        // deterministic offset based on observed move (stable)
        uint64_t seed = 1469598103934665603ULL;
        seed ^= (uint64_t)(obs_r * 1315423911u + obs_c * 2654435761u + p * 97531u);
        seed *= 1099511628211ULL;
        uint64_t sm = seed;
        double u0 = (splitmix64(sm) >> 11) * (1.0 / 9007199254740992.0) * step;

        double cdf = ps[0].w;
        int idx = 0;
        for (int i=0; i<P; ++i) {
            double u = u0 + i * step;
            while (u > cdf && idx + 1 < P) {
                idx++;
                cdf += ps[idx].w;
            }
            newps[i].par = ps[idx].par;
            newps[i].w = step;
        }
        ps = newps;
    }

    AIParam best_param() const {
        int best = 0;
        for (int i=1; i<P; ++i) if (ps[i].w > ps[best].w) best = i;
        return ps[best].par;
    }
};

// -----------------------------
// Move selection in playout
// -----------------------------
static inline Move choose_ai_move(const State &s, int p_id, const AIParam &par, Xorshift &rng, vector<Move> &tmp) {
    get_valid_moves(s, p_id, tmp);
    if (tmp.empty()) return {s.pos_r[p_id], s.pos_c[p_id]};
    if ((int)tmp.size() == 1) return tmp[0];

    double r = rng.next_double();
    if (r < par.eps) {
        return tmp[rng.next_int((int)tmp.size())];
    }

    double best = -1e100;
    int best_cnt = 0;
    Move best_moves[100];

    for (auto &m : tmp) {
        double e = AIModel::eval_cell(par, s, p_id, m.r, m.c);
        if (e > best + 1e-12) {
            best = e;
            best_cnt = 1;
            best_moves[0] = m;
        } else if (fabs(e - best) <= 1e-12) {
            best_moves[best_cnt++] = m;
        }
    }
    return best_moves[rng.next_int(best_cnt)];
}

static inline Move choose_my_rollout_move(const State &s, int leader, Xorshift &rng, vector<Move> &tmp) {
    get_valid_moves(s, 0, tmp);
    if (tmp.empty()) return {s.pos_r[0], s.pos_c[0]};
    if ((int)tmp.size() == 1) return tmp[0];

    // Weighted random with objective-aware weights
    double weights[100];
    double sum = 0.0;
    static const int dr[4] = {0, 0, 1, -1};
    static const int dc[4] = {1, -1, 0, 0};

    for (int i=0; i<(int)tmp.size(); ++i) {
        int r = tmp[i].r, c = tmp[i].c;
        double w = (double)V[r][c];

        int o = s.owner[r][c];
        if (o == -1) {
            w *= 4.158041517441582;
        } else if (o == 0) {
            bool threatened = false;
            for (int k=0; k<4; ++k) {
                int nr = r + dr[k], nc = c + dc[k];
                if (0 <= nr && nr < N && 0 <= nc && nc < N) {
                    int no = s.owner[nr][nc];
                    if (no != -1 && no != 0) { threatened = true; break; }
                }
            }
            if (s.level[r][c] < U) w *= threatened ? 6.520436877618656 : 0.6;
            else w *= threatened ? 0.8 : 0.0450417279607806;
        } else {
            // enemy
            if (s.level[r][c] == 1) {
                w *= (o == leader ? 6.703571361159733 : 6.0);
            } else {
                // attacking level>=2 often wastes the piece (it gets collected)
                w *= (o == leader ? 1.2 : 0.8);
            }
        }

        if (w < 0.01) w = 0.01;
        weights[i] = w;
        sum += w;
    }

    double rv = rng.next_double() * sum;
    double cur = 0.0;
    for (int i=0; i<(int)tmp.size(); ++i) {
        cur += weights[i];
        if (rv <= cur) return tmp[i];
    }
    return tmp.back();
}

// -----------------------------
// Simulation
// -----------------------------
static inline int compute_leader_ai(const State &s) {
    long long scores[MAX_M];
    s.compute_scores(scores);
    int leader = 1;
    for (int p=2; p<M; ++p) if (scores[p] > scores[leader]) leader = p;
    return leader;
}

static inline double run_simulation(State s, const Move my_first, int depth,
                                    const array<AIParam, MAX_M> &ai_params,
                                    uint64_t seed) {
    Xorshift rng(seed);

    array<Move, MAX_M> moves;
    vector<Move> tmp; tmp.reserve(128);

    int leader = compute_leader_ai(s);

    moves[0] = my_first;
    for (int p=1; p<M; ++p) moves[p] = choose_ai_move(s, p, ai_params[p], rng, tmp);
    advance_exact(s, moves);

    for (int d=0; d<depth; ++d) {
        leader = compute_leader_ai(s);
        moves[0] = choose_my_rollout_move(s, leader, rng, tmp);
        for (int p=1; p<M; ++p) moves[p] = choose_ai_move(s, p, ai_params[p], rng, tmp);
        advance_exact(s, moves);
    }

    return s.get_score_objective();
}

// -----------------------------
// Candidate pruning heuristic
// -----------------------------
static inline double fast_eval_candidate(const State &s, const Move &m, int leader) {
    int r = m.r, c = m.c;
    double base = (double)V[r][c];
    int o = s.owner[r][c];
    if (o == -1) return base * 3.0;
    if (o == 0) {
        if (s.level[r][c] < U) return base * 1.0;
        return base * 0.06123170494435577;
    }
    if (s.level[r][c] == 1) return base * (o == leader ? 6.915140204095886 : 6.0);
    return base * (o == leader ? 0.9 : 0.6);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> N >> M >> T >> U)) return 0;
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) cin >> V[i][j];

    State state;
    state.init_empty();
    for (int p=0; p<M; ++p) {
        cin >> state.pos_r[p] >> state.pos_c[p];
        state.owner[state.pos_r[p]][state.pos_c[p]] = p;
        state.level[state.pos_r[p]][state.pos_c[p]] = 1;
    }

    reachable_territory.reserve(MAX_N * MAX_N);

    // init AI models
    array<AIModel, MAX_M> ai_models;
    {
        uint64_t seed = 123456789ULL;
        // stable init from board values
        for (int r=0; r<N; ++r) for (int c=0; c<N; ++c) seed = seed * 1315423911ULL + (uint64_t)V[r][c];
        Xorshift init_rng(seed);
        for (int p=1; p<M; ++p) ai_models[p].init(init_rng);
    }

    const double TIME_LIMIT = 1.94; // seconds

    vector<Move> candidates;
    candidates.reserve(128);

    for (int t=0; t<T; ++t) {
        const double cur_time = global_timer.get_sec();
        double remain = TIME_LIMIT - cur_time;
        if (remain < 0.0) remain = 0.0;
        double turn_time = remain / max(1, (T - t));
        double end_time = cur_time + max(0.005, turn_time);

        State turn_start = state;

        // Build best-guess AI parameters (best particle) for playout
        array<AIParam, MAX_M> ai_params;
        for (int p=0; p<M; ++p) ai_params[p] = {0.6,0.6,0.6,0.6,0.25};
        for (int p=1; p<M; ++p) ai_params[p] = ai_models[p].best_param();

        int leader = compute_leader_ai(turn_start);

        get_valid_moves(turn_start, 0, candidates);
        if (candidates.empty()) candidates.push_back({turn_start.pos_r[0], turn_start.pos_c[0]});

        // Prune candidates to top-K by fast heuristic
        struct ScoredMove { double s; Move m; };
        static vector<ScoredMove> scored;
        scored.clear();
        scored.reserve(candidates.size());
        for (auto &m : candidates) {
            scored.push_back({fast_eval_candidate(turn_start, m, leader), m});
        }
        sort(scored.begin(), scored.end(), [](const ScoredMove &a, const ScoredMove &b){ return a.s > b.s; });

        // ensure current position included
        Move stay{turn_start.pos_r[0], turn_start.pos_c[0]};

        array<Move, MAX_CAND> top;
        int K = 0;
        bool has_stay = false;
        for (auto &sm : scored) {
            if (K >= MAX_CAND) break;
            top[K++] = sm.m;
            if (sm.m.r == stay.r && sm.m.c == stay.c) has_stay = true;
        }
        if (!has_stay) {
            if (K < MAX_CAND) top[K++] = stay;
            else top[K-1] = stay;
        }

        // Choose depth
        int depth;
        if (t < 30) depth = 7;
        else if (t < 70) depth = 10;
        else depth = 11;
        depth = min(depth, T - t - 1);
        if (depth < 0) depth = 0;

        // UCB1 over top-K
        array<double, MAX_CAND> sum;
        array<int, MAX_CAND> cnt;
        for (int i=0; i<K; ++i) { sum[i] = 0.0; cnt[i] = 0; }

        // base seed per turn (stable)
        uint64_t base = 88172645463393265ULL;
        base ^= (uint64_t)t * 0x9e3779b97f4a7c15ULL;
        base ^= (uint64_t)(turn_start.pos_r[0] * 31 + turn_start.pos_c[0] * 131);

        int total = 0;
        for (int i=0; i<K; ++i) {
            uint64_t sseed = base;
            sseed ^= (uint64_t)i * 0xBF58476D1CE4E5B9ULL;
            sseed ^= (uint64_t)(total + 1) * 0x94D049BB133111EBULL;
            double sc = run_simulation(turn_start, top[i], depth, ai_params, sseed);
            sum[i] += sc;
            cnt[i]++;
            total++;
        }

        const double C = 0.20738909720843646;
        int loop = 0;
        while (true) {
            if ((loop & 63) == 0) {
                if (global_timer.get_sec() > end_time) break;
            }

            int best_i = 0;
            double best_ucb = -1e100;
            double log_total = log((double)total);

            for (int i=0; i<K; ++i) {
                double avg = sum[i] / (double)cnt[i];
                double ucb = avg + C * sqrt(log_total / (double)cnt[i]);
                if (ucb > best_ucb) {
                    best_ucb = ucb;
                    best_i = i;
                }
            }

            uint64_t sseed = base;
            sseed ^= (uint64_t)best_i * 0xBF58476D1CE4E5B9ULL;
            sseed ^= (uint64_t)(total + 1) * 0x94D049BB133111EBULL;
            double sc = run_simulation(turn_start, top[best_i], depth, ai_params, sseed);
            sum[best_i] += sc;
            cnt[best_i]++;
            total++;
            loop++;
        }

        int best_move_idx = 0;
        double best_avg = -1e100;
        for (int i=0; i<K; ++i) {
            double avg = sum[i] / (double)cnt[i];
            if (avg > best_avg) {
                best_avg = avg;
                best_move_idx = i;
            }
        }
        Move best_move = top[best_move_idx];

        cout << best_move.r << ' ' << best_move.c << '\n' << flush;

        // Read turn feedback
        int tx_r[MAX_M], tx_c[MAX_M];
        for (int p=0; p<M; ++p) cin >> tx_r[p] >> tx_c[p];

        // Update AI models using turn_start state and observed tx
        for (int p=1; p<M; ++p) {
            vector<Move> B;
            B.reserve(128);
            get_valid_moves(turn_start, p, B);
            ai_models[p].observe(turn_start, p, tx_r[p], tx_c[p], B);
        }

        // end positions
        for (int p=0; p<M; ++p) cin >> state.pos_r[p] >> state.pos_c[p];
        for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) cin >> state.owner[i][j];
        for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) cin >> state.level[i][j];
    }

    return 0;
}
