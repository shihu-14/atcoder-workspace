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
    int n, k; cin >> n >> k;
    int ans = n;
    rep(i, k){
        string s = to_string(ans);
        string t = s;
        sort(s.begin(), s.end());
        sort(t.rbegin(), t.rend());
        ans = stoi(t) - stoi(s);
    }
    cout << ans << endl;
    return 0;
}