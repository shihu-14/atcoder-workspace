// ベクトル演算を行うライブラリ. 誤差で落ちる場合は適宜double->longlongに変更すること.
const double eps = 1e-10;
bool equal(double a, double b) { return abs(a-b) < eps;}
struct V {
  double x, y;
  V(double x=0, double y=0): x(x), y(y) {}
  V operator-() const { return V(-x, -y); }
  V& operator+=(const V& v) { x += v.x; y += v.y; return *this;}
  V operator+(const V& v) const { return V(*this) += v;}
  V& operator-=(const V& v) { x -= v.x; y -= v.y; return *this;}
  V operator-(const V& v) const { return V(*this) -= v;}
  V& operator*=(double s) { x *= s; y *= s; return *this;}
  V operator*(double s) const { return V(*this) *= s;}
  V& operator/=(double s) { x /= s; y /= s; return *this;}
  V operator/(double s) const { return V(*this) /= s;}
  double dot(const V& v) const { return x*v.x + y*v.y;} // 内積
  double cross(const V& v) const { return x*v.y - v.x*y;} // 外積: A.cross(B)でAxBの意味
  double norm1() const { return abs(x) + abs(y);} 
  double norm2() const { return sqrt(norm2s());} 
  double norm2s() const { return x*x + y*y;} 
  V normalize() const { return *this/norm2();} // 正規化(ノルムを1にスケールする)
  V rotate90c() const { return V(-y, x);} // 反時計回りに90度回転
  V rotate90() const { return V(y, -x);} // 時計回りに90度回転
  // Vがどの象限に属するかを返す関数．
  int ort() const {
    if (abs(x) < eps && abs(y) < eps) return 0;
    if (y > 0) return x>0 ? 1 : 2;
    else return x>0 ? 4 : 3;
  }
  // 偏角の大小関係を定義するための<演算子override
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
  double norm2() const { return dir().norm2();} // ベクトルのL2ノルムを取得。
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