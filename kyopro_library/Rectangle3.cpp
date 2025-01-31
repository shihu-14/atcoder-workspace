struct Rectangle3{
    int m; // h*wの長方形をm*mの3つの正方形が走査する
    vector<vector<ll>> g; // 走査する盤面
    Rectangle3(vector<vector<ll>> a, int _m): g(a), m(_m){}
    void rotate90(vector<vector<ll>>& a) {
        int h = a.size(), w = a[0].size();
        vector<vector<ll>> res(w, vector<ll>(h));
        rep(i, h)rep(j, w) res[w-1-j][i] = a[i][j];
        a = move(res);
    }
    ll solve(){
        ll res = 0;
        rep(rt, 4){
            int h = g.size(), w = g[0].size();
            // Edit here (required data structure)
            {
                // Edit here (pre-processing)
            }
            rep(j, w)rep(i, h){
                if (j+1 < m || w-1-j < m || i+1 < m || h-1-i < m) continue;
                // Edit here (calc)
            }
            rep(j1, w)rep2(j2, j1+1, w){
                if (j1+1 < m || j2-j1 < m || w-1-j2 < m) continue;
                // Edit here (calc)
            }
            rotate90(g);
        }
        return res;
    }
};

/* testcase

abc347_f

*/
