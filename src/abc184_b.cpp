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
    int n, x; cin >> n >> x;
    string s; cin >> s;
    rep(i, n){
        if (s[i] == 'o') x++;
        else x--;
        if (x < 0) x = 0;
    }
    cout << x << endl;
    return 0;
}