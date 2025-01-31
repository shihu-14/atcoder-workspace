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
    int n, q; cin >> n;
    vector<int> cnt(100010);
    ll sum = 0;
    rep(i, n){
        int av; cin >> av;
        cnt[av]++;
        sum += av;
    } 
    cin >> q;
    rep(i, q){
        ll b, c; cin >> b >> c;
        sum -= b*cnt[b];
        sum -= c*cnt[c];
        cnt[c] += cnt[b];
        cnt[b] = 0;
        sum += c*cnt[c];
        cout << sum << endl;
    }
    return 0;
}