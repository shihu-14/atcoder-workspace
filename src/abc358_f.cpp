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
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;
int gx, gy, nx, ny, res_dist;
int md(int x, int y) {return abs(x-gx)+abs(y-gy); }
int solve(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, k; cin >> n >> m >> k;
    if (k < n || k%2 != n%2){
        cout << "No" << endl;
        return 0;
    }
    gx = n-1, gy = m-1;
    res_dist = k-1, nx = 0, ny = m-1;
    vector<vector<int>> g(n, vector<int>(m, 0)); g[nx][ny] = 1;
    vector<vector<char>> ans(2*n+1, vector<char>(2*m+1, '?'));

    auto wrt = [&](int d){
        ans[2*nx+1][2*ny+1] = 'o';
        for (int t = 0; t < 4; t++){
            ans[nx*2+1+dx[t]][ny*2+1+dy[t]] = '+';
        }
        ans[nx*2+1+dx[d]][ny*2+1+dy[d]] = '.';
    };

    auto adv = [&](int d){
        int pd = d;
        if (d == 0) nx--;
        else if (d == 1) ny++;
        else if (d == 2) nx++;
        else ny--;
        g[nx][ny] = 1;
        res_dist--;
        if (d < 2) pd += 2;
        else pd -= 2;
        wrt(pd);
    };

    auto fill1 = [&](){
        while(ny < m-1) adv(1);
        while(nx < n-1) adv(2);
    };

    auto fill2 = [&](){
        while(nx < n-1) adv(2);
        while(ny < m-1) adv(1);
    };

    auto check = [&]() -> bool{
        if (md(nx, ny) < res_dist) return true;
        if (md(nx, ny) > res_dist) return false;
        if (n&1 && n-2 <= nx) fill1();
        else fill2();
        return false;
    };

    wrt(0); ans[2*nx][2*ny+1] = 'S';
    while(check() && ny > 0) adv(3);
    if (n&1){
        while(check() && nx < n-3){
            if (nx != gx || ny != gy) adv(2);
            if (nx&1) while(check() && ny < m-1) adv(1);
            else while(check() && ny > 0) adv(3);
        }
        while(check() && ny < m-1){
            if (ny&1) while(check() && nx > n-2) adv(0);
            else while(check() && nx < n-1) adv(2);
            if (nx != gx || ny != gy) adv(1);
        }
    }
    else{
        while(check() && nx < n-1){
            if (nx != gx || ny != gy) adv(2);
            if (nx&1) while(check() && ny < m-1) adv(1);
            else while(check() && ny > 0) adv(3);
        }
    }
    ans[2*nx+2][2*ny+1] = 'G';
    rep(i, 2*n+1)rep(j, 2*m+1) if (ans[i][j] == '?'){
        if (i&1 && j&1) ans[i][j] = 'o';
        else ans[i][j] = '+';
    }
    rep3(i, 2, 2*n, 2)rep3(j, 1, 2*m, 2) if (ans[i][j-1] == '+' && ans[i][j] == '+' && ans[i][j+1] == '+') ans[i][j] = '-';
    rep3(j, 2, 2*m, 2)rep3(i, 1, 2*n, 2) if (ans[i-1][j] == '+' && ans[i][j] == '+' && ans[i+1][j] == '+') ans[i][j] = '|';

    cout << "Yes" << endl;
    rep(i, 2*n+1){
        rep(j, 2*m+1) cout << ans[i][j];
        cout << endl;
    }
    return 0;
}

int main(){
    solve();
    return 0;
}