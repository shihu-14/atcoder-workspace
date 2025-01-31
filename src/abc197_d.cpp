#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;

int main(){
    int n; cin >> n;
    double x0, y0, xn, yn; cin >> x0 >> y0 >> xn >> yn;
    double theta = 360.0*M_PI/180.0/n, alpha = (M_PI-theta)/2.0;
    double r = sqrt((x0-xn)*(x0-xn)+(y0-yn)*(y0-yn))/2.0;
    double l = sqrt(r*r+r*r-2.0*r*r*cos(theta));
    double a = (x0+xn)/2, b = (y0+yn)/2;
    double x1 = (a-x0)*cos(alpha)*l/r + (b-y0)*sin(alpha)*l/r + x0;
    double y1 = -(a-x0)*sin(alpha)*l/r + (b-y0)*cos(alpha)*l/r + y0;
    printf("%.16f %.16f\n", x1, y1);
    
    return 0;
}