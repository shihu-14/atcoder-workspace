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
    int q; cin >> q;
    vector<int> id(2*n);
    rep(i, 2*n) id[i] = i;
    int on = 0;
    rep(i, q){
        int t, a, b; cin >> t >> a >> b;
        a--, b--;
        if (t == 1){
            if (on){
                if (a >= n) a -= n;
                else a += n;
                if (b >= n) b -= n;
                else b += n;
            }
            swap(id[a], id[b]);
        }
        else{
            on ^= 1;
        }
    }
    rep(i, 2*n){
        int j = i;
        if (on){
            if (j >= n) j -= n;
            else j += n;
        }
        cout << s[id[j]];
    }
    cout << endl;
    return 0;
}