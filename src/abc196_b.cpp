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
    string x; cin >> x;
    int n = x.size();
    rep(i, n){
        if (x[i] == '.') break;
        cout << x[i];
    }
    cout << endl;    
    return 0;
}