#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
double dp[110][110][110];
int main(){
    int a, b, c; cin >> a >> b >> c;
    rep(i, 110) rep(j, 110) rep(k, 110) dp[i][j][k] = 0;
    // vector dp(110, vector<vector<double>>(110, vector<double>(110, 0.0)));
    dp[a][b][c] = 1;
    for(int i = a; i <= 99; i++){
        for(int j = b; j <= 99; j++){
            for(int k = c; k <= 99; k++){
                dp[i+1][j][k] += dp[i][j][k]*i/(i+j+k);
                dp[i][j+1][k] += dp[i][j][k]*j/(i+j+k);
                dp[i][j][k+1] += dp[i][j][k]*k/(i+j+k);
            }
        }
    }

    double ans = 0;
    for (int i = b; i <= 99; i++){
        for (int j = c; j <= 99; j++){
            ans += dp[100][i][j]*(100-a+i-b+j-c);
        }
    }
    for (int i = a; i <= 99; i++){
        for (int j = c; j <= 99; j++){
            ans += dp[i][100][j]*(100-b+i-a+j-c);
        }
    }
    for (int i = a; i <= 99; i++){
        for (int j = b; j <= 99; j++){
            ans += dp[i][j][100]*(100-c+i-a+j-b);
        }
    }
    printf("%.016f\n", ans);
    return 0;
}