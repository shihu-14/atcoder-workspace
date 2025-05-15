// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
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
using mint = modint;
// using mint = modint998244353;
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

/* ---------- baby-step giant-step for A^n ≡ q (mod P) ---------- */
static ll discrete_log(ll P, ll A0, ll Q0) {
    mint::set_mod(P);
    mint A = A0, Q = Q0;
    int m = int(::sqrt(double(P))) + 1;

    /* baby steps */
    unordered_map<int,int> pos;
    mint cur = 1;
    for (int j=0;j<m;++j){
        if(!pos.count(cur.val())) pos[cur.val()] = j;
        cur *= A;
    }
    mint factor = A.pow(m).inv();          // (A^{-m})
    cur = Q;
    for (int i=0;i<=m;++i){
        auto it = pos.find(cur.val());
        if(it!=pos.end()) return 1LL*i*m + it->second;
        cur *= factor;
    }
    return -1;                            // 不存在
}
/* ---------------------------------------------------------------- */

ll solve_case(int P, int A0, int B0, int S0, int G0){
    mint::set_mod(P);
    mint A=A0,B=B0,S=S0,G=G0;

    /* X_0 = S */
    // if(S==G) return 0;

    /* ----- A == 0 special ----- */
    if(A0==0){
        // 列: S , B , B , B ...
        if(B==G) return 1;
        return -1;
    }

    /* ----- A == 1 special (線形列) ----- */
    if(A0==1){
        // X_n = S + nB (mod P)
        if(B0==0) return -1;                          // 列はずっと S ≠ G (S==G は上で処理)
        mint n = (G - S) / B;                         // n = (G-S)*B^{-1}
        return n.val();
    }

    /* ----- 一般形 (A ≠ 0,1) ----- */
    mint alpha = B / (1 - A);                         // 定常点
    if(S == alpha) return (G==alpha ? 0 : -1);

    mint q = (G - alpha) / (S - alpha);               // A^n = q

    ll ans = discrete_log(P, A0, q.val());
    return ans;
}

/* --------------------------- main --------------------------- */
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;  cin >> T;
    while(T--){
        int P,A,B,S,G; cin >> P >> A >> B >> S >> G;
        cout << solve_case(P,A,B,S,G) << '\n';
    }
    return 0;
}
