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
    int k; cin >> k;
    vector<int> used(k, -1);
    int now = 0, rc = 0;
    while(1){
        int to = (now*10+7)%k;
        if (used[to] != -1){
            cout << -1 << endl;
            return 0;
        }
        used[to] = rc++;
        if (to%k == 0){
            cout << rc << endl;
            return 0;
        }
        now = to;
    }
    return 0;
}