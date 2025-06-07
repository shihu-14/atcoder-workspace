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

const int p1 = 1000000007;
const int p2 = 998244353;
const int b = 27;
struct mints{
    ll d1, d2;
    mints(ll val=0): d1(val), d2(val) {}
    mints(ll d1, ll d2): d1(d1), d2(d2) {}
    mints operator+(const mints &r) const { return mints((d1+r.d1)%p1, (d2+r.d2)%p2); }
    mints operator*(const mints &r) const { return mints((d1*r.d1)%p1, (d2*r.d2)%p2); }
    bool operator==(const mints &r) const { return d1 == r.d1 && d2 == r.d2; }
};

struct D{
    mints h, x;
    D(){}
    D(mints h, mints x): h(h), x(x) {}
};

D op(D a, D b){ return D(a.h*b.x+b.h, a.x*b.x);}
D e(){return D(0, 1);}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    string s; cin >> s;
    string t = s;
    reverse(rng(t));
    segtree<D, op, e> seg1(n), seg2(n);
    rep(i, n) seg1.set(i, D(s[i], b));
    rep(i, n) seg2.set(i, D(t[i], b));
    rep(qi, q){
        int type; cin >> type;
        if (type == 1){
            int x; char c; cin >> x >> c; x--;
            seg1.set(x, D(c, b));
            seg2.set(n-1-x, D(c, b));
        }   
        else{
            int l, r; cin >> l >> r; l--;
            mints h1 = seg1.prod(l, r).h;
            mints h2 = seg2.prod(n-r, n-l).h;
            cout << (h1 == h2 ? "Yes" : "No") << "\n";
        }
    }
    cout.flush();
    return 0;
}