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
    ll n; cin >> n;
    ll ten = 1, ans = 0;
    int d = 0;
    rep(i, 16){
        if (ten <= n && n <= ten*10-1){
            ans += (n-ten+1)*(d/3);
            break;
        }
        ans += (ten*10 - ten)*(d/3);
        ten *= 10;
        d++;
    }
    cout << ans << endl;
    return 0;
}