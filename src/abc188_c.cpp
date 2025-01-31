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
    vector<int> tree(1<<(n+1)), a(1<<n);
    rep(i, 1<<n) cin >> a[i];
    rep(i, 1<<n) tree[(1<<n)+i] = i;
    int m = 1<<n;
    rep(i, n){
        for(int j = m; j < 2*m; j+=2){
            if (a[tree[j]] > a[tree[j+1]]){
                tree[j/2] = tree[j];
            }
            else{
                tree[j/2] = tree[j+1];
            }
        }
        m /= 2;
    }
    if (a[tree[2]] > a[tree[3]]){
        cout << tree[3]+1 << endl;
    }
    else{
        cout << tree[2]+1 << endl;
    }

    return 0;
}