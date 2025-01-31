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
    int n = s.size();
    rep(i, n){
        if (s[i] == '6') s[i] = '9';
        else if (s[i] == '9') s[i] = '6';
    }
    reverse(s.begin(), s.end());
    cout << s << endl;
    return 0;
}