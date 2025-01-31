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
    if (s.back() == 's') cout << s << "es" << endl;
    else cout << s << "s" << endl;
    return 0;
}