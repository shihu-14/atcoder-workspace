// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<ll, ll, ll>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

// fraction(a/b)
struct frac {
  ll a, b;
  frac(ll _a=0, ll _b=1): a(_a), b(_b) {
    if (b == 0) a = 1; else if (b < 0) a = -a, b = -b;
    ll g = gcd(abs(a),b);
    a /= g; b /= g;
  }
  frac inv() const { return frac(b,a);}
  frac operator+(const frac& x) const { return frac(a*x.b + x.a*b, b*x.b);}
  frac operator-(const frac& x) const { return frac(a*x.b - x.a*b, b*x.b);}
  frac operator*(const frac& x) const { return frac(a*x.a, b*x.b);}
  frac operator/(const frac& x) const { return frac(a*x.b, b*x.a);}
  frac& operator+=(const frac& x) { return *this = *this + x;}
  frac& operator-=(const frac& x) { return *this = *this - x;}
  frac& operator*=(const frac& x) { return *this = *this * x;}
  frac& operator/=(const frac& x) { return *this = *this / x;}
  bool operator<(const frac& x) const { return a*x.b < x.a*b;}
  bool operator<=(const frac& x) const { return a*x.b <= x.a*b;}
  bool operator==(const frac& x) const { return a == x.a && b == x.b;}
  bool operator!=(const frac& x) const { return a != x.a || b != x.b;}
  ll floor() const { return a/b;}
  ll ceil() const { return (a+b-1)/b;}
  ll gcd(ll a, ll b){ return (b ? gcd(b, a%b) : a); }
};
istream& operator>>(istream&i,frac&a){i>>a.a>>a.b;return i;}
ostream& operator<<(ostream&o,const frac&a){o<<a.a<<"/"<<a.b;return o;}

// coordinate(rational)
struct V {
  frac x, y;
  V(frac x=0, frac y=0):x(x),y(y) {}
  bool operator<(const V& a) const { return (x!=a.x ? x<a.x : y<a.y);}
  bool ongrid() const {return x.b == 1 && y.b == 1;}
};
istream& operator>>(istream&i,V&a){return i>>a.x>>a.y;}
ostream& operator<<(ostream&o,const V&a){return o<<a.x<<" "<<a.y;}

// Line(ax+by=c)
struct Line {
  ll a, b, c; // a,b,c >= 0
  Line(ll a=0, ll b=0, ll c=0):a(a),b(b),c(c) {}
  bool sameGrad(const Line& l) const {
    return a*l.b == l.a*b;
  }
  bool operator<(const Line& l) const { // overloaded for sorting by grad
    if (sameGrad(l)) return (a == 0 && l.a == 0 ? c < l.c : c*l.a < l.c*a); // if same grad then sort by y-intercept
    return a*l.b < l.a*b; // **オーバーロードする演算子と逆の不等式関係になっているのは、傾きは負であることは保証されているためその絶対値が昇順に並ぶようにしたいから
  }
  V isp(const Line& l) const { // calc intersection point (rational)
    frac x = frac(c*l.b - l.c*b, a*l.b - l.a*b);
    frac y = frac(c*l.a - l.c*a, b*l.a - l.b*a);
    return V(x,y);
  }
  bool need(const Line& s, const Line& t) const { // Check if this line is needed for a convex hull
    V sp = isp(s), tp = isp(t); // **spはstk[-2]とこの直線との交点, tpはstk[-1]とこの直線との交点
    return sp.x.floor() < tp.x.floor(); // sp < tp なら残す
  }
};
istream& operator>>(istream&i,Line&a){return i>>a.a>>a.b>>a.c;}
ostream& operator<<(ostream&o,const Line&a){return o<<a.a<<","<<a.b<<","<<a.c;}

// for counting intersections in a line set
struct LineSet{
    int gs; // grid size
    vector<Line> ls; // line set
    LineSet(vector<Line>& _ls, int _gs):ls(_ls), gs(_gs){}
    vector<pll> isp_set(){ // Edit here 
    // (例) 格子点であり、グリット内に収まっている交点を列挙する
      vector<pll> res;
      int n = ls.size();
      rep(i, n)rep2(j, i+1, n){
          V v = ls[i].isp(ls[j]);
          if (v.ongrid() && 1 <= v.x.a && v.x.a <= gs && 1 <= v.y.a && v.y.a <= gs){ 
              res.emplace_back(v.x.a, v.y.a);
          }
      }
      return res;
    }
    ll isp_count(){ // Edit here
    //  (例) 直線が乗っているグリットを数え上げる
      ll res = 0;
      // 「x=a, y=b, y=x+c, y=-x+c」が乗るグリットの個数を重複を許して数え上げる
      for (auto [a, b, c]: ls){
        if (a == 0 || b == 0) res -= gs;
        if (a == 1 && b == 1) res -= (c-1>gs ? 2*(1+gs)-c-1 : c-1);
        if (a == -1 && b == 1) res -= (c>0 ? gs-c : gs+c);
      }
      // 交点による重複を排除する
      auto isp_v = isp_set();
      map<pll, int> mp; //** 構造体は遅いのでなるべくpair型にする
      for (auto [x, y]: isp_v) mp[{x, y}]++;
      for (auto [k, v]: mp){
        if (v == 6) res += 3; // 4つの直線が交わる
        else if (v == 3) res += 2; // 3つの直線が交わる
        else if (v == 1) res += 1; // 2つの直線が交わる
        else assert(0);
      }
      return res;
    }
};

// ConvexHull by Line
struct CH {
    vector<Line> stk; // Lines forming the convex hull
    CH(vector<Line>& lines) {
        sort(rng(lines));
        for (auto& l : lines){
            if (stk.size() && stk.back().sameGrad(l)) continue;
            while (stk.size() >= 2 && !stk.back().need(stk[stk.size()-2], l)) stk.pop_back();
            stk.emplace_back(l);
        }
    }
    void xytrim() { // remove all but the first quadrant
        int ei = 0;
        while (ei+1 < stk.size() && stk[ei].isp(stk[ei+1]).x <= 0) ei++; stk.erase(stk.begin(), stk.begin()+ei); // 交点のx座標が負の直線を除く
        while (stk.size() >= 2 && stk.back().isp(stk[stk.size()-2]).y <= 0) stk.pop_back(); // 交点のy座標が負の直線を除く
    }
    // void add_boundary() {
    //     stk.insert(stk.begin(), Line(1, 0, 0)); // add x=0
    //     stk.emplace_back(Line(0, 1, 0)); // add y=0
    // }
    vector<V> get_isp() {
        vector<V> res;
        res.emplace_back(stk[0].isp(Line(1, 0, 0))); 
        rep(i, stk.size()-1) res.emplace_back(stk[i].isp(stk[i+1])); 
        res.emplace_back(stk.back().isp(Line(0, 1, 0)));
        return res;
    }
    ll count_inside_grid_points(){
        ll res = 0;
        vector<V> ispv = get_isp();
        rep(i, stk.size()) {
            Line l = stk[i];
            V s = ispv[i], t = ispv[i+1];
            ll lx = s.x.floor()+1, rx = t.x.floor();
            res += floor_sum(rx-lx+1, l.b, l.a, l.c-l.a*rx);
        }
        return res;
    }
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<Line> ls;
    map<T, bool> used;
    ll ans = (ll)n*n;
    rep(i, m){
        int a, b; cin >> a >> b;
        if (!used[{1, 0, a}]){
            ls.emplace_back(1, 0, a);
            used[{1, 0, a}] = true;
        }
        if (!used[{0, 1, b}]){
            ls.emplace_back(0, 1, b);
            used[{0, 1, b}] = true;
        }
        if (!used[{-1, 1, b-a}]){
            ls.emplace_back(-1, 1, b-a);
            used[{-1, 1, b-a}] = true;
        }
        if (!used[{1, 1, b+a}]){
            ls.emplace_back(1, 1, b+a);
            used[{1, 1, b+a}] = true;
        }
    }
    LineSet LS(ls, n);
    ans += LS.isp_count();
    cout << ans << endl;
    return 0;
}