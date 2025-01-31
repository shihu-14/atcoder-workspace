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
    vector<T> q;
    rep(i, n){
        int t, l, r; cin >> t >> l >> r;
        q.emplace_back(t, l, r);
    }
    int ans = 0;
    rep(i, n){
        rep(j, n){
            if (i >= j) continue;
            auto [t1, l1, r1] = q[i];
            auto [t2, l2, r2] = q[j];
            int cl1 = (t1 == 1 || t1 == 2);
            int cr1 = (t1 == 1 || t1 == 3);
            int cl2 = (t2 == 1 || t2 == 2);
            int cr2 = (t2 == 1 || t2 == 3);
            if (l2 <= r1 && l1 <= r2){
                if ((cl2*cr1 == 0 && l2 == r1) || (cl1*cr2 == 0 && l1 == r2)){
                    continue;
                }
                ans++;
            }
        }
    }
    cout << ans << endl;
    return 0;
}