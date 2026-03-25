#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <climits>
#include <cfloat>
#include <cassert>
#include <ctime>
#include <cctype>
#include <cwctype>
#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <utility>
#include <bitset>
#include <vector>
#include <deque>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <tuple>
#include <array>
#include <new>
#include <memory>
#include <limits>
#include <random>
#include <exception>
#include <stdexcept>
#include <regex>
#include <complex>
#include <chrono>
#include <future>
#include <thread>
#include <mutex>
#include <atomic>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using mint = modint;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = int(a)-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;

template<class T>
struct Matrix{
    int row, col;
    vector<vector<T>> mat;
    Matrix():mat(vector<vector<T>>()){}
    Matrix(int h, int w):row(h), col(w), mat(h, vector<T>(w, LINF)){ // 単位行列で初期化
        if (h == w) rep(i, h) mat[i][i] = 0;
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
        Matrix res(row, v.col, LINF);
        rep(i, row)
        {
            rep(j, v.col)
            {
                rep(k, col) 
                {
                    if (mat[i][k] == LINF || v[k][j] == LINF) continue;
                    chmin(res[i][j], mat[i][k]+v[k][j]);
                }
            }
        }
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
    Matrix& rotate90(){
        Matrix res(col, row);
        rep(i,row)rep(j,col) res[j][row-1-i]=mat[i][j];
        swap(row, col);
        mat=res.mat;
        return *this;
    }
    Matrix& rotate90(int sx, int sy, int n){
        assert(row == col);
        vector<vector<T>> t(n,vector<T>(n));
        rep(i,n)rep(j,n) t[i][j]=mat[sx+i][sy+j];
        rep(i,n)rep(j,n) mat[sx+j][sy+n-1-i]=t[i][j];
        return *this;
    }
    Matrix& rotate180(){
        Matrix r(row,col);
        rep(i,row)rep(j,col)r[row-1-i][col-1-j]=mat[i][j];
        mat=r.mat;
        return *this;
    }
};


const int D = 30;
void solve()
{
    int n, k; cin >> n >> k;
    vector<vector<ll>> a(n, vector<ll>(n));
    rep(i, n)rep(j, n) cin >> a[i][j];
    Matrix<ll> mat(a);
    mat = mat.power(k);

    rep(si, n)
    {
        cout << mat[si][si] << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while(t--)
    {
        solve();
    }
    return 0;
}