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
// using ld = long ll;
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


// ベクトル演算を行うライブラリ。誤差で落ちる場合は適宜ll->longlong, exp->0に変更すること。
const ll eps = 0;
bool equal(ll a, ll b) { return abs(a-b) < eps;}
struct V {
  ll x, y;
  V(ll x=0, ll y=0): x(x), y(y) {}
  V operator-() const { return V(-x, -y); }
  V& operator+=(const V& v) { x += v.x; y += v.y; return *this;}
  V operator+(const V& v) const { return V(*this) += v;}
  V& operator-=(const V& v) { x -= v.x; y -= v.y; return *this;}
  V operator-(const V& v) const { return V(*this) -= v;}
  V& operator*=(ll s) { x *= s; y *= s; return *this;}
  V operator*(ll s) const { return V(*this) *= s;}
  V& operator/=(ll s) { x /= s; y /= s; return *this;}
  V operator/(ll s) const { return V(*this) /= s;}
  ll dot(const V& v) const { return x*v.x + y*v.y;} // 内積
  ll cross(const V& v) const { return x*v.y - v.x*y;} // 外積 
  ll norm1() const { return abs(x) + abs(y);} // L1ノルム
  ll norm2() const { return sqrt(norm2s());} // L2ノルム
  ll norm2s() const { return x*x + y*y;} // L2ノルムの二乗
  V normalize() const { return *this/norm2();} // 正規化(ノルムを1にスケールする)
  V rotate90c() const { return V(-y, x);} // 反時計回りに90度回転
  V rotate90() const { return V(y, -x);} // 時計回りに90度回転
  // Vがどの象限に属するかの判定を行う関数
  int ort() const {
    if (abs(x) < eps && abs(y) < eps) return 0;
    if (y > 0) return x>0 ? 1 : 2;
    else return x>0 ? 4 : 3;
  }
  // 偏角の大小関係を定義するための<演算子オーバーロード
  bool operator<(const V& v) const {
    int o = ort(), vo = v.ort();
    if (o != vo) return o < vo;
    return cross(v) > 0;
  }
};
istream& operator>>(istream& is, V& v) {
  is >> v.x >> v.y; return is;
}
ostream& operator<<(ostream& os, const V& v) {
  os<<"("<<v.x<<","<<v.y<<")"; return os;
}
struct Line {
  V s, t;
  Line(V s=V(0,0), V t=V(0,0)):s(s),t(t){}
  V dir() const { return t-s;} // 方向ベクトル(stベクトル)を求める。
  V normalize() const { return dir().normalize();} // ベクトルの正規化
  ll norm2() const { return dir().norm2();} // ベクトルのL2ノルムを取得。
  // 点と直線の位置関係を求める関数
  int ccw(const V& p) const {
    if (dir().cross(p-s) > eps) return +1; // 方向ベクトルから見て点pは反時計回り。
    if (dir().cross(p-s) < -eps) return -1; // 方向ベクトルから見て点pは時計回り。
    if (dir().dot(p-s) < -eps) return +2; // 方向ベクトルの始点側(s側)の延長線上にある。
    if (dir().dot(t-p) < -eps) return -2; // 方向ベクトルの終点側(t側)の延長線上にある。
    return 0; // 線分st上にある。
  }
  // 2直線が交わるかどうかの判定を行う関数
  bool touch(const Line& l) const {
    int a = ccw(l.s)*ccw(l.t), b = l.ccw(s)*l.ccw(t);
    return !a || !b || (a == -1 && b == -1);
  }
};

void solve()
{
    int n, q; cin >> n >> q;
    vector<pair<V, int>> p;
    vector<int> inv(n);
    rep(i, n)
    {
        V v; cin >> v;
        p.emplace_back(v, i);
    }
    sort(rng(p));
    rep(i, n)
    {
        inv[p[i].second] = i;
    }
    // rep(i, 2*n) p[i+n] = p[i];
    vector<int> L(n), R(n);
    {
        int i = 0;
        while(i < n)
        {
            auto f = [&](V a, V b)
            {
                return a.cross(b) == 0 && a.dot(b) > 0;
            };
            if (i && f(p[i].first, p[i-1].first))
            {
                L[i] = L[i-1];
                i++;
                continue;
            }
            int j = i;
            while(j < n && f(p[i].first, p[j].first)) j++;

            for(int k = i; k < j; k++)
            {
                L[k] = i;
                R[k] = j - 1;
            }
            i = j;
        }
    }
    rep(qi, q)
    {
        int a, b; cin >> a >> b; a--; b--;
        int ans = (R[inv[a]] - L[inv[b]] + n)%n +1;
        cout << ans << "\n";
    }
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