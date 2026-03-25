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
template<typename T=int>
struct CC {
    bool initialized;
    vector<T> xs;
    CC(): initialized(false) {}
    void add(T x) { xs.push_back(x);}
    void init() {
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(),xs.end()),xs.end());
        initialized = true;
    }
    int operator()(T x) {
        if (!initialized) init();
        return upper_bound(xs.begin(), xs.end(), x) - xs.begin() - 1;
    }
    T operator[](int i) {
        if (!initialized) init();
        return xs[i];
    }
    int size() {
        if (!initialized) init();
        return xs.size();
    }
};

class PairHash {
    template < typename T >
    size_t hash_combine(size_t hr, const T &x) const {
      size_t h = std::hash< T >()(x);
      return hr ^ (h + (hr << 11) + (hr >> 13));
    }

   public:
    template < typename F, typename S >
    size_t operator()(const std::pair< F, S > &p) const {
      return hash_combine(hash_combine(0, p.first), p.second);
    }
};
const int M = 200010;

void solve(){
    int h, w, k; cin >> h >> w >> k;
    if ((h == 1 && w == 1 ) || k == 0){
        cout << "Yes" << endl;
        return;
    }
    CC ccx, ccy;
    ccx.add(0);
    ccy.add(0);
    ccx.add(h-1);
    ccy.add(w-1);
    vector<int> vx(k), vy(k);
    rep(i, k){
        int x, y; cin >> x >> y;
        x--; y--;
        ccx.add(x);
        ccy.add(y);
        vx[i] = x;
        vy[i] = y;
        rep(t, 4){
            int nx = x + dx[t], ny = y + dy[t];
            if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
            ccx.add(nx);
            ccy.add(ny);
        }
    }
    unordered_set<pii, PairHash> st;
    st.reserve(M);
    rep(i, k){
        st.emplace(ccx(vx[i]), ccy(vy[i]));
    }
    unordered_set<pii, PairHash> used;
    used.reserve(4*M);
    used.emplace(ccx(0), ccy(0));
    queue<pii> q;
    q.emplace(ccx(0), ccy(0));
    int gx = ccx(h-1), gy = ccy(w-1);
    while(!q.empty()){
        auto [x, y] = q.front(); q.pop();
        if (x == gx && y == gy){
            cout << "Yes" << endl;
            return;
        }
        rep(i, 4){
            int nx = x + dx[i], ny = y + dy[i];
            if (nx < 0 || nx >= ccx.size() || ny < 0 || ny >= ccy.size()) continue;
            if (used.count({nx, ny}) || st.count({nx, ny})) continue;
            used.emplace(nx, ny);
            q.emplace(nx, ny);
        }
    }
    cout << "No" << endl;
}   

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while(t--){
        solve();
    }
    return 0;
}