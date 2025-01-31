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
    vector<int> c(1000010), used(1000010);
    rep(i, n) c[a[i]]++;
    bool flag1 = true, flag2 = true;
    for(int i = 2; i <= 1000000; i++){
        if (used[i]) continue;
        int res = 0;
        for(int j = i; j <= 1000000; j += i){
            used[j] = 1;
            res += c[j];
        }
        if (res >= 2){
            flag1 = false;
        }
        if (res == n){
            flag2 = false;
        }
    }
    if (flag1){
        cout << "pairwise coprime" << endl;
    }
    else if (flag2){
        cout << "setwise coprime" << endl;
    }
    else{
        cout << "not coprime" << endl;
    }

    return 0;
}