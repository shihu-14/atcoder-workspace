#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 入力受け取り
    string S;
    cin >> S;

    // 1. S を逆転した文字列 R を作る
    string R = S;
    reverse(R.begin(), R.end());

    // 2. T = S + '#' + R を構築
    string T = S + '#' + R;
    int n = (int)T.size();

    // 3. T に対して prefix function(失敗関数) を計算 (inline)
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && T[i] != T[j]) {
            j = pi[j - 1];
        }
        if (T[i] == T[j]) {
            j++;
        }
        pi[i] = j;
    }

    // 4. 最後の pi 値が S の末尾と R の先頭の一致長
    int l = pi.back();

    // 5. R の先頭 l 文字分を除いた部分を S に足して最短回文を構築
    string add = R.substr(l);
    string ans = S + add;

    // 出力
    cout << ans << "\n";
    return 0;
}
