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
    vector<int> c(4);
    rep(i, n){
        string s; cin >>s;
        if (s == "AC") c[0]++;
        if (s == "WA") c[1]++;
        if (s == "TLE") c[2]++;
        if (s == "RE") c[3]++;
    }
    vector<string> s(4);
    s[0] = "AC", s[1] = "WA", s[2] = "TLE", s[3] = "RE";
    rep(i, 4){
        printf("%s x %d\n", s[i].c_str(), c[i]);
    }
    return 0;
}