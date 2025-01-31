#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using ull = unsigned long long;
using ld = long ll;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
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

// ベクトル演算を行うライブラリ。誤差で落ちる場合は適宜ll->longlong, exp->0に変更すること。
const double eps = 1e-10;
bool equal(double a, double b) { return abs(a-b) < eps;}
struct V {
  ll x, y;
  V(ll x=0, ll y=0): x(x), y(y) {}
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
    if (dir().cross(p-s) > 0) return +1; // 方向ベクトルから見て点pは反時計回り。
    if (dir().cross(p-s) < -0) return -1; // 方向ベクトルから見て点pは時計回り。
    if (dir().dot(p-s) < -0) return +2; // 方向ベクトルの始点側(s側)の延長線上にある。
    if (dir().dot(t-p) < -0) return -2; // 方向ベクトルの終点側(t側)の延長線上にある。
    return 0; // 線分st上にある。
  }
  // 2直線が交わるかどうかの判定を行う関数
  bool touch(const Line& l) const {
    int a = ccw(l.s)*ccw(l.t), b = l.ccw(s)*l.ccw(t);
    return !a || !b || (a == -1 && b == -1);
  }
};

int border(vector<V> &v, V &p){
    int n = v.size();
    auto getLine = [&](int i, int j){
        return Line(v[i], v[j]);
    };
    Line l1(v[0],v[1]), ln(v[0],v[n-1]);
    if (l1.ccw(p) < 0 || ln.ccw(p) > 0) return -1;
    else if (l1.ccw(p) == 0 || ln.ccw(p) == 0) return 0;

    int ac = 1, wa = n-1;
    while(wa-ac > 1){
        int wj = (ac+wa)/2;
        if (getLine(0, wj).ccw(p) >= 0) ac = wj;
        else wa = wj;
    }

    int ret = getLine(ac, ac+1).ccw(p);
    if (ret == 0) return 0;
    else if (ret == 1) return 1;
    else return -1;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<V> p(n);
    rep(i,n) cin >> p[i];
    int q; cin >> q;
    rep(i, q){
        V v; cin >> v;
        int ret = border(p, v);
        if (ret == 0) cout << "ON" << endl;
        else if (ret == 1) cout << "IN" << endl;
        else cout << "OUT" << endl;
    }
    return 0;
}