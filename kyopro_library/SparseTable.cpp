// Sparse Table 前処理 O(NlogN), クエリ O(1)
template<typename T>
struct SparseTable {
    int n;
    vector<vector<T>> table; // table[i][j] = dataの区間[i, i+2^j)の演算結果を格納
    vector<int> logtable; // floor(log2(i))を格納
    function<T(T,T)> op; // 2項演算子(結合性・冪等性を満たすもの)
    SparseTable() {}
    void build(const vector<T> &data, function<T(T,T)> f){
        n = data.size(); logtable.resize(n+1); op = f; // メンバ変数の初期化
        for(int i = 2; i <= n; i++) logtable[i] = logtable[i/2] + 1;
        int max_log = logtable[n];
        table.resize(n, vector<T>(max_log+1));
        for(int i = 0; i < n; i++) table[i][0] = data[i];
        for(int j = 1; j <= max_log; j++){
            for(int i = 0; i+(1<<(j-1)) < n; i++){
                if(i+(1<<j) > n) break;
                table[i][j] = op(table[i][j-1], table[i+(1<<(j-1))][j-1]);
            }
        }
    }
    // dataの区間[l, r)の演算結果を返す
    T query(int l, int r){
        int ql = r-l;
        assert(ql > 0 && ql <= n);
        int k = logtable[ql];
        return op(table[l][k], table[r-(1<<k)][k]);
    }
};

/* testcase

abc262_f

*/
