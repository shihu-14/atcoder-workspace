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
    vector<int> a(3); 
    rep(i, 3) cin >> a[i];
    do{
        if (a[2] - a[1] == a[1] - a[0]){
            cout << "Yes" << endl;
            return 0;
        }
    }while(next_permutation(a.begin(), a.end()));
    cout << "No" << endl;
    return 0; 
}