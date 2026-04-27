struct bit_Matrix {
    using BS = bitset<64>; // Edit bitsize (default 64)
    int h, w;
    vector<BS> d;
    vector<vector<BS>> exp_d;
    
    bit_Matrix(int h, int w): h(h), w(w), d(h), exp_d(64, vector<BS>(h)) {}
    
    BS operator[](int i) const { return d[i]; }
    BS& operator[](int i) { return d[i]; }

    void build() { // build for using power()
        assert(h == w); 
        exp_d[0] = d;
        vector<BS> d_trans(h);
        for(int i = 0; i < h; i++) for(int j = 0; j < h; j++) d_trans[i][j] = d[j][i];
        
        for(int t = 1; t < 64; t++) {
            for(int i = 0; i < h; i++) {
                for(int j = 0; j < h; j++) {
                    exp_d[t][i][j] = (exp_d[t-1][i] & d_trans[j]).any();
                }
            }
            for(int i = 0; i < h; i++) {
                for(int j = 0; j < h; j++) {
                    d_trans[j][i] = exp_d[t][i][j];
                }
            }
        }
    }

    BS power(BS x, long long n) { // calc d^(n)*x
        assert(h == w); 
        BS res = x;
        for (int i = 0; n > 0; n >>= 1, i++) {
            if (n & 1) {
                BS nxt; // 【改善】元の変数を直接書き換えずに一時変数で安全に受ける
                for(int j = 0; j < h; j++) nxt[j] = (exp_d[i][j] & res).any();
                res = nxt;
            }
        }
        return res;
    }

    BS solve() { // treat matrix 'd' as augmented coefficient matrix and solve
        int ni = 0;
        rep(j,w-1) {
        for (int i = ni; i < h; i++) {
            if (d[i][j]) {swap(d[i], d[ni]); break;}
        }
        if (!d[ni][j]) continue;
        rep(i,h) if (ni != i && d[i][j]) d[i] ^= d[ni];
        ni++; if (ni == h) break;
        }
        BS nan, res; nan[w-1] = 1;
        rep(i,h) {
        int j = 0;
        while (j < w-1 && !d[i][j]) j++;
        if (j == w-1) {
            if (d[i][w-1]) return nan;
            continue;
        }
        res[j] = d[i][w-1];
        }
        return res;
    }
};
ostream& operator<<(ostream&o,const bit_Matrix&a) {
  rep(i,a.h) o<<a[i]<<endl;
  return o;
}

/* testcase

abc388_f
abc366_g

*/