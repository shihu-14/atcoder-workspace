#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint1000000007;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;

int main(){
    int n; cin >> n;
    vector<mint> a(n), s(n);
    rep(i, n){
        int av; cin >> av;
        a[i] = av;
    }
    s[0] = a[0];
    rep(i, n-1) s[i+1] = s[i] + a[i+1];
    mint ans;
    rep(i, n-1){
        ans += a[i]*(s[n-1]-s[i]);
    }
    cout << ans.val() << endl;
    return 0;
}