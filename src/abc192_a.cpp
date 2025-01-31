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
    int x; cin >> x;
    int ans = 0;
    for (int i = x+1; i < 1000000; i++){
        if (i%100 == 0){
            ans++;
            cout << ans << endl;
            break;
        }
        ans++;
    }
    
    return 0;
}