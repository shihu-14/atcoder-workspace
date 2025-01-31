#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<ll, ll>;
using T = tuple<int, int, int>;

vector<vector<ll>> multi(vector<vector<ll>> a, vector<vector<ll>> b){
    int n = a.size(), m = b[0].size(), l = b.size(); 
    vector<vector<ll>> res(n, vector<ll>(m, 0));
    rep(i, n){
        rep(j, m){
            rep(k, l){
                res[i][j] += a[i][k]*b[k][j];
            }
        }
    }
    return res;
}

int main(){
    int n, m, Q; cin >> n;
    vector<P> p;
    rep(i, n){
        ll x, y; cin >> x >> y;
        p.emplace_back(x, y);
    }
    cin >> m;
    vector<P> op;
    rep(i, m){
        int t; cin >> t;
        if (t < 3){
            op.emplace_back(t, 0);
        }
        else{
            ll pi; cin >> pi;
            op.emplace_back(t, pi);
        }
    }
    cin >> Q;
    vector<T> q;
    rep(i, Q){
        int a, b; cin >> a >> b; b--;
        q.emplace_back(a, b, i);
    }
    sort(q.begin(), q.end());
    
    vector<P> ans(Q);
    vector<vector<ll>> H(3, vector<ll>(3, 0));
    rep(i, 3) H[i][i] = 1;
    int c = 0;
    rep(i, Q){
        auto [a, b, id] = q[i];
        if (a <= c){
            auto [x, y] = p[b];
            vector<vector<ll>> X(3, vector<ll>(1, 1)); X[0][0] = x, X[1][0] = y;
            X = multi(H, X);
            ans[id] = {X[0][0], X[1][0]};
            continue;
        }
        while(c < a){
            auto [t, pi] = op[c];
            if (t == 1){
                vector<vector<ll>> L(3, vector<ll>(3, 0));
                L[0][1] = 1;
                L[1][0] = -1;
                L[2][2] = 1;
                H = multi(L, H);
            }
            else if (t == 2){
                vector<vector<ll>> L(3, vector<ll>(3, 0));
                L[0][1] = -1;
                L[1][0] = 1;
                L[2][2] = 1;
                H = multi(L, H);
            }
            else if (t == 3){
                vector<vector<ll>> L(3, vector<ll>(3, 0));
                L[0][0] = -1; L[0][2] = 2*pi;
                L[1][1] = 1;
                L[2][2] = 1;
                H = multi(L, H);
            }
            else{
                vector<vector<ll>> L(3, vector<ll>(3, 0));
                L[0][0] = 1;
                L[1][1] = -1; L[1][2] = 2*pi;
                L[2][2] = 1;
                H = multi(L, H);
            }
            c++;
        }
        auto [x, y] = p[b];
        vector<vector<ll>> X(3, vector<ll>(1, 1)); X[0][0] = x, X[1][0] = y;
        X = multi(H, X);
        ans[id] = {X[0][0], X[1][0]};
    }
    rep(i, Q){
        cout << ans[i].first << " " << ans[i].second << endl;
    }
    return 0;
}