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
    vector<int> a(n); 
    rep(i, n) cin >> a[i];
    vector<int> ans;
    int sum = 0;
    auto f = [&](auto f, int now) -> void{
        if (now == n){
            if (sum%k != 0) return;
            rep(i, n) cout << ans[i] << " ";
            cout << endl;
            return;
        }
        for (int i = 1; i <= a[now]; i++){
            ans.push_back(i);
            sum += i;
            f(f, now+1);
            ans.pop_back();
            sum -= i;
        }
    };
    f(f, 0);
    return 0;
}