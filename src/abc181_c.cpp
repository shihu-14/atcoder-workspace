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
    vector<int> X(n), Y(n);
    rep(i, n){
        cin >> X[i] >> Y[i];
    }
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            for(int k = j+1; k < n; k++){
                if (X[i] == X[j]){
                    if (X[i] == X[k]){
                        cout << "Yes" << endl;
                        return 0;
                    }
                    continue;
                }          
                if ((Y[k]-Y[i])*(X[i]-X[j]) == (Y[i]-Y[j])*(X[k]-X[i])){
                    cout << "Yes" << endl;
                    return 0;
                }
            }
        }
    }
    cout << "No" << endl;
    return 0;
}
