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

void solve(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N, M;
    ull L, U;
    cin >> N >> M >> L >> U;
    
    __int128 width = ( __int128)U - ( __int128)L;
    ull d = ceil128(width, 512);
    if(d == 0) d = 1;

    ll sets = (N >= 10LL*M ? M : min<ll>(M, N/10));
    vector<ull> A(max<ll>(0,N), 1);

    auto idx_anchor = [&](ll j)->ll{ return j; };
    auto idx_piece  = [&](ll j, ll k)->ll{ return sets + j*9 + k; };

    rep(j, sets){
        if(idx_anchor(j) < N) A[idx_anchor(j)] = L;
        rep(k, 9){
            __int128 piece128 = ( (__int128)d << k );
            ull piece = (piece128 > ( __int128)U ? U : (ull)piece128);
            ll id = idx_piece(j,k);
            if(id < N) A[id] = piece;
        }
    }

    printv(A);
    cout.flush();

    vector<ull> B(M, 0);
    rep(j, M) if(!(cin >> B[j])) B[j]=L;

    vector<ll> X(N, 0);
    rep(j, sets){
        ull Bj = B[j];
        ll ia = idx_anchor(j);
        if(0 <= ia && ia < N) X[ia] = (int)(j+1);
        ull r = (Bj >= L ? Bj - L : 0ULL);
        ull t = (r + d/2) / d;
        if(t > 511ULL) t = 511ULL;
        rep(k, 9){
            if(t & (1ULL<<k)){
                ll id = idx_piece(j,k);
                if(0 <= id && id < N) X[id] = (int)(j+1);
            }
        }
    }

    printv(X);
    cout.flush();
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
