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
    double p = 1.0/n;
    double ans = 0;
    rep(i, n-1){
        ans += 1.0/(1.0-p*(i+1));
    }
    std::cout << fixed               
              << setprecision(16)   
              << setfill('0')        
              << setw(16)            
              << ans              
              << std::endl;
    return 0;
}