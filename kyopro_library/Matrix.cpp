template<class T>
struct Matrix{
    int row, col;
    vector<vector<T>> mat;
    Matrix():mat(vector<vector<T>>()){}
    Matrix(int h, int w):row(h), col(w), mat(h, vector<T>(w, 0)){ // 単位行列で初期化
        if (h == w) rep(i, h) mat[i][i] = 1;
    }
    Matrix(int h, int w, T a):row(h), col(w), mat(h, vector<T>(w, a)){} // 値で初期化
    Matrix(vector<vector<T>> v):row(v.size()), col(v[0].size()), mat(v) {} // 行列で初期化
    vector<T> operator[](const int i) const {return mat[i];} // 読み取り
    vector<T>& operator[](const int i){return mat[i];} // 書き込み
    Matrix& operator=(const Matrix& v){
        row = v.row; col = v.col; mat = v.mat;
        return *this;
    }
    // 演算子オーバーロード(行列*行列)
    Matrix& operator+=(const Matrix& v){
        assert(row == v.row && col == v.col);
        rep(i, row)rep(j, col) mat[i][j] += v[i][j];
        return *this;
    }
    Matrix& operator-=(const Matrix& v){
        assert(row == v.row && col == v.col);
        rep(i, row)rep(j, col) mat[i][j] -= v[i][j];
        return *this;
    }
    Matrix& operator*=(const Matrix& v){
        assert(col == v.row);
        Matrix res(row, v.col, 0);
        rep(i, row)rep(j, v.col)rep(k, col) res[i][j] += mat[i][k]*v[k][j];
        col = v.col; rep(i, row) mat[i].resize(col); rep(i, row)rep(j, col) mat[i][j] = res[i][j];
        return *this;
    }
    bool operator==(const Matrix& v){assert(row == v.row && col == v.col); return mat == v.mat;}
    bool operator!=(const Matrix& v){assert(row == v.row && col == v.col); return mat != v.mat;}

    Matrix operator+(const Matrix& v) const { return Matrix(*this) += v;}
    Matrix operator-(const Matrix& v) const { return Matrix(*this) -= v;}
    Matrix operator*(const Matrix& v) const { return Matrix(*this) *= v;}

    // 演算子オーバーロード(行列*スカラー値)
    Matrix& operator+=(const T& a){rep(i, row)rep(j, col) mat[i][j] += a; return *this;}
    Matrix& operator-=(const T& a){rep(i, row)rep(j, col) mat[i][j] -= a; return *this;}
    Matrix& operator*=(const T& a){rep(i, row)rep(j, col) mat[i][j] *= a; return *this;}
    Matrix& operator/=(const T& a){rep(i, row)rep(j, col) mat[i][j] /= a; return *this;}
    Matrix& operator%=(const T& a){rep(i, row)rep(j, col) mat[i][j] %= a; return *this;}
    Matrix operator+(const T& a) const { return Matrix(*this) += a;}
    Matrix operator-(const T& a) const { return Matrix(*this) -= a;}
    Matrix operator*(const T& a) const { return Matrix(*this) *= a;}
    Matrix operator/(const T& a) const { return Matrix(*this) /= a;}
    Matrix operator%(const T& a) const { return Matrix(*this) %= a;}

    void print(){rep(i, row)rep(j, col) cout << mat[i][j] << " \n"[j==col-1]; cout << endl;}
    Matrix power(ll b) const {
        assert(row == col);
        Matrix res(row, col);
        Matrix a = *this;
        for(;b;b>>=1,a*=a) if(b&1) res*=a;
        return res;
    }
    Matrix power(ll b, ll mod) const {
        assert(row == col);
        Matrix res(row, col);
        Matrix a = *this;
        for(;b;b>>=1,a=(a*a)%mod) if(b&1) res = (res*a)%mod;
        return res;
    }
    // Matrix& rotate90{
        // 
    // }
};
