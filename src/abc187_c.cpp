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
    vector<string> s;
    set<string> st1, st2;
    rep(i, n){
        string t; cin >> t;
        if (t[0] == '!'){
            st2.insert(t);
            continue;
        }
        s.push_back(t);
        st1.insert(t);
    }
    rep(i, s.size()){
        string t = '!'+s[i];
        if (st1.count(s[i]) && st2.count(t)){
            cout << s[i] << endl;
            return 0;
        }
    }
    cout << "satisfiable" << endl;
    return 0;
}