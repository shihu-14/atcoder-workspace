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
    string n; cin >> n;
    int m = n.size();
    int ans = m;
    rep(i, 1<<m){
        if (i == 0) continue;
        ll res = 0, ten = 1; int cnt = m;
        rep(j, m){
            if (i&(1<<j)){
                res += ten*(n[j]-'0');
                cnt--;
            }
            ten *= 10;
        }
        if (res%3 == 0){
            ans = min(ans, cnt);
        }
    }
    cout << (ans == m ? -1 : ans)<< endl;
    return 0;
}