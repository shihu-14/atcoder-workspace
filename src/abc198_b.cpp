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
    if (n == 0){
        cout << "Yes" << endl;
        return 0;
    }
    while(n%10 == 0) n /= 10;
    string s = to_string(n);
    string t = s;
    reverse(t.begin(), t.end());
    if (s == t) cout << "Yes" << endl;
    else cout << "No" << endl;
    return 0;
}