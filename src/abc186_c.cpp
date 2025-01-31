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
    int ans = 0;
    vector<int> eight(7, 1);
    rep(i, 6) eight[i+1] = eight[i]*8; 

    for (int i = 1; i <= n; i++){
        string s = to_string(i);
        int m = s.size();
        bool flag = false;
        rep(j, m){
            if (s[j] == '7'){
                flag = true;
                break;
            }
        }
        if (flag) continue;
        string t;
        int d = i;
        for (int j = 6; j >= 0; j--){
            int q = d/eight[j];
            int r = d%eight[j];
            if (q == 0) continue;
            t += q + '0';
            d = r;
        }

        m = t.size();
        flag = false;
        rep(j, m){
            if (t[j] == '7'){
                flag = true;
                break;
            }
        }
        if (flag) continue;
        ans++;
    }
    cout << ans << endl;
    return 0;
}