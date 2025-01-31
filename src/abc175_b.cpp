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
    vector<int> L(n);
    rep(i, n) cin >> L[i];
    int ans = 0;
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            for(int k = j+1; k < n; k++){
                if (L[i] == L[j] || L[j] == L[k] || L[k] == L[i]) continue;
                if (L[i]+L[j] > L[k] && L[j]+L[k] > L[i] && L[k]+L[i] > L[j]){
                    ans++;
                }
            }
        }
    }
    cout << ans << endl;
    return 0;
}