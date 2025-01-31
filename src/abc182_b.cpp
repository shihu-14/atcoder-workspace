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
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    int max_k = 0, ans = -1;
    for(int i = 2; i <= 1000; i++){
        int tmp = 0;
        rep(j, n){
            if (a[j]%i == 0) tmp++;
        }
        if (max_k < tmp){
            max_k = tmp;
            ans = i;
        }
    }
    cout << ans << endl;
    return 0;
}