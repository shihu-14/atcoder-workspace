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
    string s; cin >> s;
    set<int> st;
    rep(i, n){
        if (s[i] == 'R') st.insert(i);
    }
    int ans = 0;
    rep(i, n){
        if (s[i] == 'W' && st.size() > 0){
            int j = *st.rbegin();
            if (i > j) break; 
            swap(s[i], s[j]);
            st.erase(j);
            ans++;
        }
    }
    bool flag = false;
    rep(i, n){
        if (s[i] == 'W') flag = true;
        if (flag && s[i] == 'R') ans++;
    }
    cout << ans << endl;
    return 0;
}