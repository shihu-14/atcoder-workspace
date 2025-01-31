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
    int ok = 0;
    rep(i, n){
        if (s[i] == 'o') ok++;
    }
    if (ok > 4){
        cout << 0 << endl;
        return 0;
    }
    int ans = 0;
    rep(i, 10000){
        ostringstream oss;
        oss << setw(4) << setfill('0') << i;
        string t = oss.str();
        vector<int> used(10);
        bool flag = false;
        rep(j, 4){
            if (s[t[j]-'0'] == 'o'){
                used[t[j]-'0'] = true;
            }
            else if (s[t[j]-'0'] == 'x'){
                flag = true;
                break;
            }
        }
        int tmp = 0;
        rep(j, 10){
            if (used[j]) tmp++;
        }
        if (flag || ok != tmp) continue;
        ans++;
    }
    cout << ans << endl;
    return 0;
}