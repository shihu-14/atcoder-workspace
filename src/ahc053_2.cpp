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
template<class T>
using G = vector<vector<T>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
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

static inline ll clamp_ll(ll x, ll lo, ll hi){ if(x<lo) return lo; if(x>hi) return hi; return x; }
static inline ll iabs(ll x){ return x>=0? x : -x; }

void solve(){
    // 1) 読み込み（前半は N,M,L,U のみ）
    ll N, M, L, U; 
    if(!(cin>>N>>M>>L>>U)) return;

    // 2) 事前設計：2アンカー + Kビットの“コイン”を M 枚ずつ
    //    K は 2M + K*M <= N を満たす最大、かつ安全な上限（50）に丸める
    ll K = 0;
    if(M>0){
        ll can = (ll)(N / M) - 2;
        if(can > 50) can = 50;
        if(can < 0) can = 0;
        K = can;
    }

    // pow2K，s の計算（K=0 の場合はコイン無し）
    auto ceil_div = [](unsigned __int128 a, unsigned __int128 b)->unsigned __int128{
        return (a + b - 1) / b;
    };

    ull pow2K = (K==0 ? 1ull : (1ull<<K)); // K<=50 を保証
    ll s = 0;
    if(K>0){
        // Δceil = ceil((U-L)/M)
        unsigned __int128 dUL = (unsigned __int128)(U - L);
        unsigned __int128 Delta_ceil = ceil_div(dUL, (unsigned __int128)M);
        unsigned __int128 num = dUL + Delta_ceil;
        unsigned __int128 den = (unsigned __int128)pow2K - 1u;
        unsigned __int128 s128 = ceil_div(num, den);
        if(s128 == 0) s128 = 1;
        // c_{K-1} = s * 2^{K-1} が U を超えるなら K を縮めて再計算
        // 余裕を持って調整
        while(K>0){
            pow2K = (1ull<<K);
            den = (unsigned __int128)pow2K - 1u;
            s128 = ceil_div(num, den);
            if(s128 == 0) s128 = 1;
            unsigned __int128 top = s128 * (1ull<<(K-1));
            if(top <= (unsigned __int128)U) break;
            --K;
        }
        s = (ll)s128;
    }

    // 3) A_i を構築して出力
    vector<ll> A; A.reserve(N);
    vector<int> typ; typ.reserve(N); // 0:anchor-, 1:anchor+, 2+k: coin bit k, 100: extra
    vector<int> par; par.reserve(N); // 補助（アンカーは順位 j，コインは bit k）
    // 期待順位点 μ_j
    auto mu_of = [&](ll j)->ll{
        // μ_j = L + ((2j-1)/(2M))*(U-L) を整数で
        __int128 twoj_1 = ( (__int128)2*j - 1 );
        __int128 num = twoj_1 * (__int128)(U - L);
        __int128 den = ( (__int128)2 * M );
        ll add = (ll)( num / den );
        ll mu = L + add;
        return mu;
    };

    unsigned __int128 C = 0, halfC = 0;
    if(K>0){
        C = (unsigned __int128)s * ( (unsigned __int128)pow2K - 1u );
        halfC = C / 2u;
    }

    // アンカー 2M 枚
    vector<pair<ll,int>> anchors; anchors.reserve(2*M);
    rep(j, M){
        ll muj = mu_of(j+1);
        ll low = (K>0)? clamp_ll( (ll)((__int128)muj - ( __int128)halfC ), 1LL, U) : clamp_ll(muj, 1LL, U);
        A.push_back(low); typ.push_back(0); par.push_back((int)j);
        anchors.emplace_back(low, (int)A.size()-1);
    }
    rep(j, M){
        ll muj = mu_of(j+1);
        ll high = (K>0)? clamp_ll( (ll)((__int128)muj + ( __int128)halfC ), 1LL, U) : clamp_ll(muj, 1LL, U);
        A.push_back(high); typ.push_back(1); par.push_back((int)j);
        anchors.emplace_back(high, (int)A.size()-1);
    }

    // コイン K 種 × 各 M 枚
    vector<vector<int>> coin_ids; coin_ids.resize(K);
    vector<ll> coin_val(K, 0);
    rep(k, K){
        unsigned __int128 v128 = (unsigned __int128)s * (1ull<<k);
        ll v = (ll) v128;
        if(v < 1) v = 1;
        if(v > U) v = U;
        coin_val[k] = v;
        rep(j, M){
            A.push_back(v); typ.push_back(2 + (int)k); par.push_back((int)k);
            coin_ids[k].push_back((int)A.size()-1);
        }
    }

    // 余りは捨て札（1）
    while((ll)A.size() < N){
        A.push_back(1); typ.push_back(100); par.push_back(-1);
    }

    // 出力（A1..AN）＋ flush
    rep(i, N){
        if(i) cout << ' ';
        cout << A[i];
    }
    cout << '\n' << flush;

    // 4) B を読み込み
    vector<ll> B(M);
    rep(i, M) cin >> B[i];

    // 5) 初期割当：アンカーを B に一つずつ対応
    //    「<= Bj の最大」を優先。なければ最小の未使用。
    sort(rng(anchors)); // by value asc
    vector<int> used_anchor(anchors.size(), 0);
    vector<int> anchor_of_pile(M, -1);
    vector<pair<ll,int>> Bs; Bs.reserve(M);
    rep(j, M) Bs.emplace_back(B[j], (int)j);
    sort(rng(Bs)); // asc

    vector<int> avail; avail.reserve(2*M);
    int p = 0;
    rep(ti, M){
        ll bj = Bs[ti].first;
        int j = Bs[ti].second;
        while(p < (int)anchors.size() && anchors[p].first <= bj){
            avail.push_back(p);
            ++p;
        }
        int pick = -1;
        while(!avail.empty()){
            int idx = avail.back(); avail.pop_back();
            if(!used_anchor[idx]){ pick = idx; break; }
        }
        if(pick == -1){
            int q = p;
            while(q < (int)anchors.size() && used_anchor[q]) ++q;
            if(q == (int)anchors.size()){
                q = 0;
                while(q < (int)anchors.size() && used_anchor[q]) ++q;
            }
            pick = q;
        }
        used_anchor[pick] = 1;
        int card_id = anchors[pick].second;
        anchor_of_pile[j] = card_id;
    }

    // 6) コインを貪欲に配布（高ビット→低ビット，R>=c_k）
    vector<ll> S(M, 0);
    vector<int> X(N, 0); // 0=捨て，1..M=山
    rep(j, M){
        int cid = anchor_of_pile[j];
        if(cid>=0){
            X[cid] = j+1;
            S[j] += A[cid];
        }
    }
    rep(k, K){
        // 供給は M 枚。二進性から「各山 at most 1」で十分。
        // 高ビット優先のため k を降順で回す
    }
    for(int k = (int)K-1; k >= 0; --k){
        ll v = coin_val[k];
        int ptr = 0; // coin_ids[k] の使用位置
        // 各山に最大1枚
        rep(j, M){
            if(ptr >= (int)coin_ids[k].size()) break;
            ll need = B[j] - S[j];
            if(need >= v){
                int cid = coin_ids[k][ptr++];
                X[cid] = j+1;
                S[j] += v;
            }
        }
        // 余りのコインは未使用のまま（捨て）
    }

    // 7) 局所改良（2-opt の簡易版：コイン一枚移動）
    //    時間制限内で改善する移動のみ採用（ΔE<0）
    vector<int> coin_card_ids; coin_card_ids.reserve(K*M);
    rep(k, K) for(int id: coin_ids[k]) coin_card_ids.push_back(id);

    // coin -> current pile map（-1=未使用）
    vector<int> coin_to_pile(N, -1);
    for(int id: coin_card_ids){
        if(X[id] > 0) coin_to_pile[id] = X[id]-1;
    }

    auto cur_E = [&]()->long double{
        long double e = 0;
        rep(j, M) e += (long double) iabs(S[j]-B[j]);
        return e;
    };

    // 改善ループ
    auto t0 = chrono::steady_clock::now();
    mt19937_64 rng64(chrono::steady_clock::now().time_since_epoch().count());
    const double TIME_LIMIT_MS = 120.0; // 軽め
    ll iter = 0, best_accept = 0;
    while(true){
        ++iter;
        if(iter % 256 == 0){
            auto now = chrono::steady_clock::now();
            double ms = chrono::duration<double, std::milli>(now - t0).count();
            if(ms > TIME_LIMIT_MS) break;
        }
        if(coin_card_ids.empty()) break;
        int cid = coin_card_ids[rng64() % coin_card_ids.size()];
        int tp = typ[cid];
        if(tp < 2 || tp >= 2+(int)K) continue; // 念のため
        int k = par[cid];
        ll v = coin_val[k];
        int pcur = coin_to_pile[cid]; // -1 未使用

        // 現在の E のうち、関係する山だけ差分評価
        // 候補：未使用(-1)および全 M 山
        ll best_gain = LLONG_MIN;
        int best_to = pcur; // そのまま
        // 先に removal 効果
        ll remove_gain = 0;
        if(pcur != -1){
            ll Sp = S[pcur];
            ll gp_before = iabs(Sp - B[pcur]);
            ll gp_after  = iabs((Sp - v) - B[pcur]);
            remove_gain = gp_before - gp_after;
            // 移動先未使用
            if(remove_gain > best_gain){
                best_gain = remove_gain;
                best_to = -1;
            }
        }
        // 加算効果
        int start_j = 0;
        rep(j, M){
            if(j == pcur) continue;
            ll Sj = S[j];
            ll gj_before = iabs(Sj - B[j]);
            ll gj_after  = iabs((Sj + v) - B[j]);
            ll add_gain = gj_before - gj_after;
            ll total_gain = add_gain + (pcur==-1 ? 0 : remove_gain);
            if(total_gain > best_gain){
                best_gain = total_gain;
                best_to = j;
            }
        }
        if(best_gain > 0){
            // 採用：pcur -> best_to
            if(pcur != -1){
                S[pcur] -= v;
                X[cid] = 0;
            }
            if(best_to != -1){
                S[best_to] += v;
                X[cid] = best_to + 1;
            }
            coin_to_pile[cid] = best_to;
            ++best_accept;
        }
        // それ以外はスキップ（焼きなましは省略）
    }

    // 8) X を出力して flush
    rep(i, N){
        if(i) cout << ' ';
        cout << X[i];
    }
    cout << '\n' << flush;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    while(t--){
        solve();
    }
    return 0;
}
