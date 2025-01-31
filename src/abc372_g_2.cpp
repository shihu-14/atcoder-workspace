#include <bits/stdc++.h>
#include <atcoder/all>
#define rep(i,n) for(int i = 0; i < (n); ++i)
#define rep1(i,n) for(int i = 1; i <= (n); ++i)
#define drep(i,n) for(int i = (n)-1; i >= 0; --i)
#define srep(i,s,t) for (int i = s; i < (t); ++i)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define fi first
#define se second
#define pb push_back
#define eb emplace_back
#define em emplace
#define pob pop_back
#define sz(x) (int)(x).size()
#define pcnt __builtin_popcountll
#define snuke srand((unsigned)clock()+(unsigned)time(NULL));
#define newline puts("")
#define vc vector
using namespace std;
using namespace atcoder;
template<class T> using vv = vc<vc<T>>;
template<class T> using PQ = priority_queue<T,vc<T>,greater<T>>;
using uint = unsigned; using ull = unsigned long long;
using vi = vc<int>; using vvi = vv<int>; using vvvi = vv<vi>;
using ll = long long; using vl = vc<ll>; using vvl = vv<ll>; using vvvl = vv<vl>;
using P = pair<int,int>; using vp = vc<P>; using vvp = vv<P>; using LP = pair<ll,ll>;
int geti(){int x;scanf("%d",&x);return x;}
vi pm(int n, int s=0) { vi a(n); iota(rng(a),s); return a;}
template<class T1,class T2>istream& operator>>(istream&i,pair<T1,T2>&v){return i>>v.fi>>v.se;}
template<class T1,class T2>ostream& operator<<(ostream&o,const pair<T1,T2>&v){return o<<v.fi<<","<<v.se;}
template<class T>istream& operator>>(istream&i,vc<T>&v){rep(j,sz(v))i>>v[j];return i;}
template<class T>string join(const T&v,const string&d=""){stringstream s;rep(i,sz(v))(i?s<<d:s)<<v[i];return s.str();}
template<class T>ostream& operator<<(ostream&o,const vc<T>&v){if(sz(v))o<<join(v," ");return o;}
template<class T>void vin(vc<T>&a){int n;cin>>n;a=vc<T>(n);cin>>a;}
template<class T>void vin(vv<T>&a){int n,m;cin>>n>>m;a=vv<T>(n,vc<T>(m));cin>>a;}
template<class T1,class T2>void operator--(pair<T1,T2>&a,int){a.fi--;a.se--;}
template<class T1,class T2>void operator++(pair<T1,T2>&a,int){a.fi++;a.se++;}
template<class T>void operator--(vc<T>&a,int){for(T&x:a)x--;}
template<class T>void operator++(vc<T>&a,int){for(T&x:a)x++;}
template<class T1,class T2>void operator+=(vc<T1>&a,T2 b){for(T1&x:a)x+=b;}
template<class T1,class T2>void operator-=(vc<T1>&a,T2 b){for(T1&x:a)x-=b;}
template<class T1,class T2>void operator*=(vc<T1>&a,T2 b){for(T1&x:a)x*=b;}
template<class T1,class T2>void operator/=(vc<T1>&a,T2 b){for(T1&x:a)x/=b;}
template<class T>void operator+=(vc<T>&a,const vc<T>&b){a.insert(a.end(),rng(b));}
template<class T1,class T2>pair<T1,T2>operator+(const pair<T1,T2>&a,const pair<T1,T2>&b){return {a.fi+b.fi,a.se+b.se};}
template<class T1,class T2>pair<T1,T2>operator-(const pair<T1,T2>&a,const pair<T1,T2>&b){return {a.fi-b.fi,a.se-b.se};}
template<class T>pair<T,T>operator*(const pair<T,T>&a,T b){return {a.fi*b,a.se*b};}
template<class T1,class T2>bool mins(T1& x,const T2&y){if(y<x){x=y;return true;}else return false;}
template<class T1,class T2>bool maxs(T1& x,const T2&y){if(x<y){x=y;return true;}else return false;}
template<class T>T min(const vc<T>&a){return *min_element(rng(a));}
template<class T>T max(const vc<T>&a){return *max_element(rng(a));}
template<class Tx,class Ty>Tx dup(Tx x, Ty y){return (x+y-1)/y;}
template<class T>ll suma(const vc<T>&a){ll s=0;for(auto&&x:a)s+=x;return s;}
template<class T>ll suma(const vv<T>&a){ll s=0;for(auto&&x:a)s+=suma(x);return s;}
template<class T>void uni(T&a){sort(rng(a));a.erase(unique(rng(a)),a.end());}
template<class T>void prepend(vc<T>&a,const T&x){a.insert(a.begin(),x);}
const double eps = 1e-10;
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;
#define dame { puts("-1"); return;}
#define yes { puts("Yes"); return;}
#define no { puts("No"); return;}
#define rtn(x) { cout<<(x)<<'\n'; return;} // flush!
#define yn {puts("Yes");}else{puts("No");}

// Fraction
struct frac {
  ll a, b;
  frac(ll _a=0, ll _b=1): a(_a), b(_b) {
    if (b == 0) a = 1; else if (b < 0) a = -a, b = -b;
    // ll g = gcd(abs(a),b);
    // a /= g; b /= g;
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
  ll ceil() const { return dup(a,b);}
};
istream& operator>>(istream&i,frac&a){i>>a.a>>a.b;return i;}
ostream& operator<<(ostream&o,const frac&a){o<<a.a<<"/"<<a.b;return o;}
//

struct V {
  frac x, y;
  V(frac x=0, frac y=0):x(x),y(y) {}
  bool operator<(const V& _) const { return x<_.x;}
};
istream& operator>>(istream&i,V&a){return i>>a.x>>a.y;}
ostream& operator<<(ostream&o,const V&a){return o<<a.x<<" "<<a.y;}

struct Line {
  ll a, b, c;
  Line(ll a=0, ll b=0, ll c=0):a(a),b(b),c(c) {}
  bool sameGrad(const Line& l) const {
    return a*l.b == l.a*b;
  }
  bool operator<(const Line& l) const {
    if (sameGrad(l)) return c*l.a < l.c*a;
    return a*l.b < l.a*b;
  }
  V xp(const Line& l) const {
    frac x = frac(c*l.b - l.c*b, a*l.b - l.a*b);
    frac y = frac(c*l.a - l.c*a, b*l.a - l.b*a);
    return V(x,y);
  }
  bool need(const Line& s, const Line& t) const {
    V sx = xp(s), tx = xp(t);
    return sx.x.floor() < tx.x.floor();
  }
};
istream& operator>>(istream&i,Line&a){return i>>a.a>>a.b>>a.c;}
ostream& operator<<(ostream&o,const Line&a){return o<<a.a<<","<<a.b<<","<<a.c;}

// arithmetic progression sum
// first:s, difference:d, length: n
ll asum(ll s, ll d, ll n) {
  return s*n + (n-1)*n/2*d;
}
// first:a, difference:d, length:n, denominator: e
// sum{(a+d*i)/e}
ll gaussSum(ll a, ll d, ll n, ll e) {
  if (!n) return 0;
  ll res = asum(a/e,d/e,n);
  a %= e; d %= e;
  if (!d) return res;
  ll z = a+d*(n-1);
  return res + gaussSum(z%e+d,e,z/e,d);
}

struct Solver {
  void solve() {
    int n;
    scanf("%d",&n);
    vc<Line> ls(n);
    cin>>ls;
    sort(rng(ls));
    rep(i,n) ls[i].c--;
    {
      vc<Line> d;
      for (auto l : ls) {
        if (sz(d) && d.back().sameGrad(l)) continue;
        while (sz(d) >= 2 && !d.back().need(d.end()[-2], l)) d.pob();
        d.pb(l);
      }

      int si = 0;
      while (si+1 < sz(d) && d[si].xp(d[si+1]).x <= 0) si++;
      d.erase(d.begin(), d.begin()+si);
      while (sz(d) >= 2 && d.back().xp(d.end()[-2]).y <= 0) d.pob();
      swap(d,ls);
    }


    vc<V> xs;
    xs.eb(ls[0].xp(Line(1,0,0)));
    rep(i,sz(ls)-1) xs.eb(ls[i].xp(ls[i+1]));
    xs.eb(ls.back().xp(Line(0,1,0)));

    ll ans = 0;
    rep(i,sz(ls)) {
      Line l = ls[i];
      V s = xs[i], t = xs[i+1];
      ll lx = s.x.floor()+1, rx = t.x.floor();
      ans += floor_sum(rx-lx+1, l.b, -l.a, l.c-l.a*lx);
    }
    cout<<ans<<'\n';
  }
};

int main() {
  // cin.tie(nullptr); ios::sync_with_stdio(false);
  int ts = 1;
  scanf("%d",&ts);
  rep1(ti,ts) {
    Solver solver;
    solver.solve();
  }
  return 0;
}
