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
    int a, b, c; cin >> a >> b >> c;
    if (c%2 == 0){
        cout << (abs(a) < abs(b) ? "<" : abs(a) == abs(b) ? "=" : ">") << endl;
    } 
    else{
        cout << (a > b ? ">" : a == b ? "=" : "<") << endl;
    }
    return 0;
}