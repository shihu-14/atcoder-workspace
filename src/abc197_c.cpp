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
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    int m = 1<<(n-1);
    int ans = 1<<31-1;
    rep(i, m){
        vector<int> id;
        rep(j, n-1){
            if (i&(1<<j)) id.push_back(j+1);
        }
        int k = 0, tmp = 0, sum = 0;
        rep(j, n){
            if (k < id.size() && j == id[k]){
                sum ^= tmp;
                tmp = 0;
                k++;
            }
            tmp |= a[j];
        }
        sum ^= tmp;
        ans = min(ans, sum);
    }
    cout << ans << endl;
    return 0;
}