#include <bits/stdc++.h>
using namespace std;
#define rep(i,n) for (int i = 0; i < (n); ++i)
using P = pair<int,int>;

struct Bus {
  int a, b, s, t;
  bool operator<(const Bus& x) const {
    return s > x.s;
  }
};

int main() {
  int n, m, q;
  cin >> n >> m >> q;
  vector<Bus> bus;
  rep(i,m) {
    int a, b, s, t;
    cin >> a >> b >> s >> t;
    --a; --b;
    bus.emplace_back((Bus){a,b,s,t});
  }
  sort(bus.begin(), bus.end());
  vector<set<P>> ts(n);
  const int D = 17;
  vector dp(D,vector<int>(m,-1));
  auto get = [&](int v, int t) {
    auto it = ts[v].lower_bound(P(t, -1));
    if (it == ts[v].end()) return -1;
    return it->second;
  };
  rep(i,m) {
    Bus b = bus[i];
    ts[b.a].emplace(b.s, i);
  }
  rep(i,m){
    Bus b = bus[i];
    dp[0][i] = get(b.b, b.t);
  }

  rep(i,D-1)rep(j,m) {
    if (dp[i][j] != -1) dp[i+1][j] = dp[i][dp[i][j]];
  }

  rep(qi,q) {
    int x, y, z;
    cin >> x >> y >> z;
    --y;
    int i = get(y, x);
    if (i == -1) cout << y+1 << endl;
    else {
      for (int j = D-1; j >= 0; --j) {
        int ni = dp[j][i];
        if (ni != -1 && bus[ni].s < z) i = ni;
      }
      Bus b = bus[i];
      if (z <= b.s) cout << b.a+1 << endl;
      else if (z <= b.t) printf("%d %d\n", b.a+1, b.b+1);
      else cout << b.b+1 << endl;
    }
  }
  return 0;
}