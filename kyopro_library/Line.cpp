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
  bool operator<(const V& a) const { return x<a.x;}
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
    if (sameGrad(l)) return c*l.a < l.c*a; // if same grad then sort by y-intercept
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
    /* (例) 格子点であり、グリット内に収まっている交点を列挙する
      vector<pll> res;
      int n = ls.size();
      rep(i, n)rep2(j, i+1, n){
          V v = ls[i].isp(ls[j]);
          if (v.ongrid() && 1 <= v.x.a && v.x.a <= gs && 1 <= v.y.a && v.y.a <= gs){ 
              res.emplace_back(v.x.a, v.y.a);
          }
      }
      return res;
    */
    }
    ll isp_count(){ // Edit here
    /* (例) 直線が乗っているグリットを数え上げる
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
    */
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

    ll count_grid_points(){
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
