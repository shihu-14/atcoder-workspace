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
    int a, b, x, y; cin >> a >> b >> x >> y;
    if (a > x){
        swap(a, x);
        swap(b, y);
    }
    if (b > y){
        int nx, ny;
        nx = b - y + a;
        ny = x - a + b;
        x = nx, y = ny;
    }

    auto f = [&](int a, int b, int x, int y) -> bool{
        for(int i = -3; i <= 3; i++){
            for(int j = -3; j <= 3; j++){
                if (abs(i)+abs(j) > 3) continue;
                int na = a+i, nb = b+j;
                if (na-x == nb-y) return true;
            }
        }
        return false;
    };
    int ans;
    if (x == a && y == b) ans = 0;
    else if (x-a+y-b <= 3) ans = 1;
    else if (x != a && (y-b) == (x-a)) ans = 1;
    else if (x-a+y-b <= 6) ans = 2;
    else if (f(a, b, x, y)) ans = 2;
    else if ((y-x-b+a)%2 == 0) ans = 2;
    else ans = 3;
    cout << ans << endl;
    return 0;
}