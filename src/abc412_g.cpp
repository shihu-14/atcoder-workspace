#include <bits/stdc++.h>
using namespace std;
#define rep(i,n) for (int i = 0; i < (n); ++i)

int main() {
  int n, m;
  cin >> n >> m;
  vector<string> s(n);
  rep(i,n) cin >> s[i];
  vector a(n+1,vector<int>(n+1));
  vector b(n+1,vector<int>(n+2));
  rep(i,n)rep(j,n) {
    if (s[i][j] == 'X') continue;
    a[i][j]++; a[min(i+m,n)][j]--;
    // (i+m,j) + (-1,2)x (l<=x<=r)
    int l = max(0, i+m-n); // i+m-x <= n
    int r = min(m, (n-j)/2); // j+2x <= n
    if (l < r) {
      a[i+m-l][j+l*2]++;
      a[i+m-r][j+r*2]--;
    }
  }

  rep(i,n)rep(j,n) a[i+1][j] += a[i][j];
  rep(i,n)rep(j,n) a[i+1][j] += a[i][j+2];
//   rep(i,n)rep(j,n) a[i][j] += b[i][j];
  rep(i,n)rep(j,n) a[i][j+1] += a[i][j];

  int q;
  cin >> q;
  rep(qi,q) {
    int i, j;
    cin >> i >> j;
    --i; --j;
    cout << a[i][j] << '\n';
  }
  return 0;
}