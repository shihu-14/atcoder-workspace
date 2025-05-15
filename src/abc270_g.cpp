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

// a^x
template <typename T>
T power(T a, ll n){
    T res = 1;
    for (;n; n>>=1, a=a*a) if(n&1) res*=a;
    return res;
}
// floor(a^(1/n))
ll power2(ll a, ll n){
    assert(n >= 1);
    auto f = [&](ll w) -> bool{
        ll res = 1;
        rep(i, n){
            if (res > a/w) return false;
            res *= w;
        }
        return res <= a;
    };
    ll ac = 0, wa = a+1;
    while(wa-ac>1){
        ll wj = (ac+wa)/2;
        if (f(wj)) ac = wj;
        else wa = wj;
    }
    return ac;
}

void solve(){
    int p; cin >> p;
    mint::set_mod(p);
    auto f = [](){
        int x; cin >> x;
        return mint(x);
    };
    mint a = f(), b = f(), s = f(), g = f();

    if (a == 0){
        if (s == g) cout << 0 << "\n";
        else if (b == g) cout << 1 << "\n";
        else cout << -1 << "\n";
        return;
    }
    if (a == 1){
        if (b == 0){
            if (s == g) cout << 0 << "\n";
            else cout << -1 << "\n";
        }
        else{
            mint ans = (g-s)/b;
            cout << ans.val() << "\n";
        }
        return;
    }
    mint alpha = b / (1 - a);
    if (s == alpha) {
        cout << (g == alpha ? 0 : -1) << "\n";
        return;
    }

    mint q = (g-alpha)/(s-alpha);

    int D = ceil(sqrt(p));

    vector<mint> A(D, 1);
    rep(i, D-1) A[i+1] = A[i]*a;
    unordered_map<int, int> A2id;
    rep(i, D){
        if (!A2id.count(A[i].val())) A2id[A[i].val()] = i;
    }
    
    mint now = q, rate = a.pow(D).inv();
    rep(i, D+1){
        if (A2id.count(now.val())){
            int ans = D*i + A2id[now.val()];
            cout << ans << "\n";
            return;
        }
        now *= rate;
    }
    cout << -1 << "\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    while(t--){
        solve();
    }
    return 0;
}