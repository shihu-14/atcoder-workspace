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
    vector<int> c(10);
    rep(i, n){
        c[s[i]-'0']++;
    }
    if (n == 1){
        if (s[0] == '8') cout << "Yes" << endl;
        else cout << "No" << endl;
        return 0;
    }
    if (n == 2){
        for(int i = 16; i <= 99; i+=8){
            string t = to_string(i);
            vector<int> d(10);
            rep(j, t.size()){
                d[t[j]-'0']++;
            }
            bool ok = true;
            rep(j, 10){
                if (d[j] > c[j]) ok = false;
            }
            if (ok) {
                cout << "Yes" << endl;
                return 0;
            }
        }
        cout << "No" << endl;
        return 0;
    }
    
    for(int i = 104; i <= 999; i+=8){
        string t = to_string(i);
        vector<int> d(10);
        rep(j, t.size()){
            d[t[j]-'0']++;
        }
        bool ok = true;
        rep(j, 10){
            if (d[j] > c[j]) ok = false;
        }
        if (ok) {
            cout << "Yes" << endl;
            return 0;
        }
    }
    cout << "No" << endl;
    return 0;
}