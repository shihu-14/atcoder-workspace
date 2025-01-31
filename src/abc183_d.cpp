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
    int n, w; cin >> n >> w;
    vector<ll> a(200010);
    rep(i, n){
        int s, t, p; cin >> s >> t >> p;
        a[s] += p;
        a[t] -= p;
    }
    rep(i, 200001) a[i+1] += a[i];
    rep(i, 200001) if (a[i] > w){
        cout << "No" << endl;
        return 0;
    }
    cout << "Yes" << endl;
    return 0;
}