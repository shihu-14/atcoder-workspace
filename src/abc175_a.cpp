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
    if (s[0] == 'R' && s[1] == 'R' && s[2] == 'R') cout << 3 << endl;
    else if ((s[0] == 'R' && s[1] == 'R') || (s[1] == 'R' && s[2] == 'R')) cout << 2 << endl;
    else if (s[0] == 'R' || s[1] == 'R' || s[2] == 'R') cout << 1 << endl;
    else cout << 0 << endl;
    return 0;
}