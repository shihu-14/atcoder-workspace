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
#define rrep2(i, a, b) for (ll i = (a)-1; i >= (b); --i)
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
template<class T> void printv(const vector<T> &v){for(size_t i=0;i<v.size();++i){cout<<v[i]<<(i+1==v.size()?'\n':' ');} }
template<class T> void printvv(const vector<vector<T>> &v){for(size_t i=0;i<v.size();++i){for(size_t j=0;j<v[i].size();++j){cout<<v[i][j]<<(j+1==v[i].size()?'\n':' ');} } cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;

static inline ull ceil128(__int128 a, __int128 b){
    if(a<=0) return 0ULL;
    return (ull)((a + b - 1) / b);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, M; ull L, U;
    if(!(cin >> N >> M >> L >> U)) return 0;

    __int128 width = ( __int128)U - ( __int128)L;
    ull d = ceil128(width, 512);
    if(d == 0) d = 1;

    ll sets = (N >= 10LL*M ? M : min<ll>(M, N/10));
    vector<ull> A(max<ll>(0,N), 1);

    vector<int> anchor_ids; anchor_ids.reserve(sets);
    array<vector<int>,9> coin_ids; rep(k,9) coin_ids[k].reserve(sets);

    auto idx_anchor = [&](ll j)->ll{ return j; };
    auto idx_piece  = [&](ll j, ll k)->ll{ return sets + j*9 + k; };

    rep(j, sets){
        ll ia = idx_anchor(j);
        if(ia < N){ A[ia] = L; anchor_ids.push_back((int)ia); }
        rep(k, 9){
            __int128 piece128 = ( (__int128)d << k );
            ull piece = (piece128 > ( __int128)U ? U : (ull)piece128);
            ll id = idx_piece(j,k);
            if(id < N){ A[id] = piece; coin_ids[k].push_back((int)id); }
        }
    }

    printv(A);
    cout.flush();

    vector<ull> B(M, 0);
    rep(j, M){ if(!(cin >> B[j])) B[j]=L; }

    vector<ll> Xr(N,0), Xf(N,0);
    vector<long long> Sr(M,0), Sf(M,0);

    int pa=0; array<int,9> pk{}; rep(k,9) pk[k]=0;
    rep(j, sets){
        int id = anchor_ids[pa++]; Xr[id]=j+1; Sr[j]+= (long long)L;
        ull Bj=B[j];
        ull r = (Bj >= L ? Bj - L : 0ULL);
        ull t = (r + d/2) / d;
        if(t > 511ULL) t = 511ULL;
        rep(k,9){
            if(t & (1ULL<<k)){
                int cid = coin_ids[k][pk[k]++];
                Xr[cid]=j+1; Sr[j]+= (long long)A[cid];
            }
        }
    }
    long double Er=0; rep(j,M) Er += (long double) llabs(Sr[j] - (long long)B[j]);

    pa=0; rep(k,9) pk[k]=0;
    rep(j, sets){ int id=anchor_ids[pa++]; Xf[id]=j+1; Sf[j]+= (long long)L; }
    vector<long long> rnow(M,0);
    rep(j,M) rnow[j] = Sf[j] - (long long)B[j];

    for(int k=8;k>=0;--k){
        ull v = (ull)((__int128)d<<k);
        int supply = (int)coin_ids[k].size();
        if(supply<=0) continue;
        int S = 0, P = 1, base = 2, T = 2 + (int)M;
        mcf_graph<long long,long long> g(T+1);
        g.add_edge(S,P,supply,0);
        vector<int> eidx(M,-1);
        rep(j,M){
            long long delta = llabs(rnow[j] + (long long)v) - llabs(rnow[j]);
            eidx[j] = g.add_edge(P, base + j, 1, delta);
            g.add_edge(base + j, T, 1, 0);
        }
        g.add_edge(P, T, supply, 0);
        g.flow(S, T, supply);
        auto es = g.edges();
        int used = 0;
        rep(j,M){
            if(eidx[j] >= 0 && es[eidx[j]].flow > 0){
                int cid = coin_ids[k][used++];
                Xf[cid] = j+1;
                Sf[j] += (long long)v;
                rnow[j] += (long long)v;
            }
        }
    }
    long double Ef=0; rep(j,M) Ef += (long double) llabs(Sf[j] - (long long)B[j]);

    const vector<ll>& X = (Ef < Er ? Xf : Xr);
    printv(X);
    cout.flush();
    return 0;
}
