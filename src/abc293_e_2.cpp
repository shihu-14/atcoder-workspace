// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;
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
    // 演算子オーバーロード(行列)
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
        col = v.col; rep(i, row) mat[i].resize(col); rep(i, row)rep(j, col) mat[i] = res[i];
        return *this;
    }
    bool operator==(const Matrix& v){assert(row == v.row && col == v.col); return mat == v.mat;}
    bool operator!=(const Matrix& v){assert(row == v.row && col == v.col); return mat != v.mat;}

    Matrix operator+(const Matrix& v) const { return Matrix(*this) += v;}
    Matrix operator-(const Matrix& v) const { return Matrix(*this) -= v;}
    Matrix operator*(const Matrix& v) const { return Matrix(*this) *= v;}
    // 演算子オーバーロード(スカラー値)
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
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll a, x, m; cin >> a >> x >> m;
    Matrix<ll> H(2, 2); H[0][0] = a, H[0][1] = 1, H[1][0] = 0, H[1][1] = 1;
    Matrix<ll> v(2, 1, 1);
    H = H.power(x-1, m);
    H *= v;
    H %= m;
    cout << H[0][0] << endl;
    return 0;
}
