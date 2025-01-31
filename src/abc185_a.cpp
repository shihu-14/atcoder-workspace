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
    int a1, a2, a3, a4; cin >> a1 >> a2 >> a3 >> a4;
    cout << min({a1, a2, a3, a4}) << endl;
    return 0;
}