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
    int ans = n*1.08;
    if (ans < 206) cout << "Yay!" << endl;
    else if (ans == 206) cout << "so-so" << endl;
    else cout << ":(" << endl;
    return 0;
}