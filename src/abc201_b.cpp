#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, string>;
using T = tuple<int, int, int>;

int main(){
    int n; cin >> n;
    vector<P> p;
    rep(i, n){
        string b; int a; cin >> b >> a;
        p.push_back(P(a, b));
    }
    sort(p.rbegin(), p.rend());
    cout << p[1].second << endl;
    return 0;
}  