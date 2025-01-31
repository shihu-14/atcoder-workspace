#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;
const int inf = 1001001001;
int main(){
    int n, m, q; cin >> n >> m >> q;
    vector<P> a;
    rep(i, n){
        int w, v; cin >> w >> v;
        a.push_back({w, v});
    }
    sort(a.begin(), a.end());
    vector<P> b;
    rep(i, m){
        int x; cin >> x;
        b.push_back({x, i});
    }
    sort(b.begin(), b.end());

    rep(t, q){
        int l, r; cin >> l >> r;
        l--; r--;
        priority_queue<int> pq;
        int ai = 0, ans = 0;
        rep(i, m){
            auto [x, id] = b[i];
            if (l <= id && id <= r) continue;
            while (ai < n && a[ai].first <= x){
                pq.emplace(a[ai].second);
                ai++;
            }
            if (!pq.empty()){
                ans += pq.top();
                pq.pop();
            }
        }
        cout << ans << endl;
    }
    return 0;
}