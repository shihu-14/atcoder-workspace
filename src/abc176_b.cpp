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
    string s; cin >> s;
    int m = s.size();
    int ans = 0;
    rep(i, m){
        ans += (s[i]-'0');
        ans %= 9;
    }
    cout << (ans%9 == 0 ? "Yes" : "No") << endl;
    return 0;
}