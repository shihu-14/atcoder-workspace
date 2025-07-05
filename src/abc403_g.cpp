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

struct S{
    ll even, odd;
    int parity;
    S(): even(0), odd(0), parity(0) {}
    S(ll even, ll odd, int parity): even(even), odd(odd), parity(parity) {}
};
S op(S a, S b){
    S res;
    if (a.parity == 0){           
        res.even = a.even + b.even;
        res.odd  = a.odd  + b.odd;
    }
    else{                        
        res.even = a.even + b.odd;
        res.odd  = a.odd  + b.even;
    }
    res.parity = a.parity ^ b.parity;
    return res;
}
S e(){
    return S();
}
template <typename T, T (*op)(T, T), T (*e)()>
struct SegmentTree{
  private:
    int n;
    unordered_map<int, T> data;
    void thrust(int i, T x){
        i += n-1;
        data[i] = x;
        while (i>0){
            i = (i-1)/2;
            data[i] = op(data[i*2+1], data[i*2+2]);
        }
    }
    T query(int l, int r, int k, int L, int R){
        if (r <= L || R <= l) return e();
        if (l <= L && R <= r) return data[k];
        T vl = query(l, r, k*2+1, L, (L+R)/2);
        T vr = query(l, r, k*2+2, (L+R)/2, R);
        return op(vl, vr);
    }
  public:
    SegmentTree(int _n){
        int x = 1;
        while (x < _n) x *= 2;
        n = x;
        // data.assign(2*n-1, e());
    }
    void set(int i, T x){ // a[i] = x
        assert(0 <= i && i < n);
        thrust(i, x);
    }
    void update(int i, T x){ // a[i] <= op(a[i], x)
        assert(0 <= i && i < n);
        thrust(i, op(data[i+n-1], x));
    }
    T get(int i){ // a[i]
        assert(0 <= i && i < n);
        return data[i+n-1];
    }
    T prod(int l, int r){ // op(a[l], a[l+1], ..., a[r-1])
        assert(0 <= l && l <= r && r <= n);
        return query(l, r, 0, 0, n);
    }
    T all_prod(){ // op(a[0], a[1], ..., a[n-1])
        return query(0, n, 0, 0, n);
    }
};

const int M = 1e9;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q; cin >> q;
    SegmentTree<S, op, e> seg(M);
    ll z = 0;
    rep(qi, q){
        int y; cin >> y;
        int x = (z+y)%M+1;
        seg.update(x, S(0, x, 1));
        z = seg.all_prod().odd;
        cout << z << "\n";
    }
    return 0;
}
