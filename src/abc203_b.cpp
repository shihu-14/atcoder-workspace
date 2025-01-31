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
    int  n, k; cin >> n >> k;
    int ans = 0;
    for (int i = 1; i <= n; i++){
        for(int j = 1; j <= k; j++){
            ans += 100*i+j;
        }
    }
    cout << ans << endl;
    return 0;
}