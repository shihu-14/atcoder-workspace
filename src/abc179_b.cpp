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
    vector<P> d;
    rep(i, n){
        int a, b; cin >> a >> b;
        d.push_back({a, b});
    }
    rep(i, n-2){
        if(d[i].first == d[i].second && d[i+1].first == d[i+1].second && d[i+2].first == d[i+2].second){
            cout << "Yes" << endl;
            return 0;
        }  
    }
    cout << "No" << endl;
    return 0;
}