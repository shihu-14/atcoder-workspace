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
    string s, t; cin >> s >> t;
    int n = s.size(), m = t.size();
    int ans = 1001001001;
    rep(i, n-m+1){
        int res = 0;
        rep(j, m){
            if (s[i+j] != t[j]) res++;
        }
        ans = min(ans, res);
    }
    cout << ans << endl;
    return 0;
}