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
    int n, k; cin >> n >> k;
    vector<vector<int>> T(n, vector<int>(n));
    rep(i, n)rep(j, n) cin >> T[i][j];

    vector<int> id(n-1);
    rep(i, n-1) id[i] = i+1;
    int ans = 0;
    do{
        int dist = 0;
        vector<int> tmp = id;
        tmp.insert(tmp.begin(), 0);
        tmp.push_back(0);
        rep(i, n){
            dist += T[tmp[i]][tmp[i+1]];
        }
        if (dist == k) ans++;

    } while (next_permutation(id.begin(), id.end()));
    cout << ans << endl;
    return 0;
}