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
    int k; cin >> k;
    string s, t; cin >> s >> t;
    vector<int> ten(8, 1);
    rep(i, 7) ten[i+1] = ten[i]*10;
    vector<int> c(10, k), cs(10, 0), ct(10, 0);
    rep(i, 4) c[s[i]-'0']--;
    rep(i, 4) c[t[i]-'0']--;
    rep(i, 4) cs[s[i]-'0']++;
    rep(i, 4) ct[t[i]-'0']++;
    double ans = 0;
    for(int i = 1; i < 10; i++){
        for (int j = 1; j < 10; j++){
            if (c[i] == 0) continue;
            c[i]--;
            if (c[j] == 0){
                c[i]++;
                continue;
            }
            c[i]++;
            cs[i]++, ct[j]++;
            int res1 = 0, res2 = 0;
            for (int k = 1; k < 10; k++){
                res1 += k*ten[cs[k]];
                res2 += k*ten[ct[k]];
            }
            cs[i]--, ct[j]--;
            if (res1 > res2){
                double res = 1;
                res *= (double)c[i]/(9*k-8); c[i]--;
                res *= (double)c[j]/(9*k-9); c[i]++;
                ans += res;
            }   
        }
    }
    printf("%.016f\n", ans);
    return 0;
}