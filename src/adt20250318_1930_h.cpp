// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
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
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
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
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<vector<int>> a(3, vector<int>(3));
    rep(i, 3)rep(j, 3) cin >> a[i][j];
    map<pair<vector<vector<int>>, int>, int> memo;
    auto dfs = [&](auto f, vector<vector<int>> now, int d) -> int{
        auto check = [&](vector<vector<int>> &now) -> int{
            rep(i, 3){
                bool flag = true;
                int cnt = 1;
                rep(j, 3){
                    if (now[i][j] == -1) flag = false;
                    else cnt *= now[i][j];
                }
                if (flag && cnt == 1){
                    return 1;
                }
            }
            rep(j, 3){
                bool flag = true;
                int cnt = 1;
                rep(i, 3){
                    if (now[i][j] == -1) flag = false;
                    else cnt *= now[i][j];
                }
                if (flag && cnt == 1){
                    return 1;
                }
            }
            rep(i, 3){
                bool flag = true;
                int cnt = 0;
                rep(j, 3){
                    if (now[i][j] == -1) flag = false;
                    else cnt += now[i][j];
                }
                if (flag && cnt == 0){
                    return 0;
                }
            }
            rep(j, 3){
                bool flag = true;
                int cnt = 0;
                rep(i, 3){
                    if (now[i][j] == -1) flag = false;
                    else cnt += now[i][j];
                }
                if (flag && cnt == 0){
                    return 0;
                }
            }
            {
                bool flag = true;
                int cnt = 1;
                rep(i, 3){
                    if (now[i][i] == -1) flag = false;
                    else cnt *= now[i][i];
                }
                if (flag && cnt == 1){
                    return 1;
                }
            }
            {
                bool flag = true;
                int cnt = 0;
                rep(i, 3){
                    if (now[i][i] == -1) flag = false;
                    else cnt += now[i][i];
                }
                if (flag && cnt == 0){
                    return 0;
                }
            }
            {
                bool flag = true;
                int cnt = 1;
                rep(i, 3){
                    if (now[i][2-i] == -1) flag = false;
                    else cnt *= now[i][2-i];
                }
                if (flag && cnt == 1){
                    return 1;
                }
            }
            {
                bool flag = true;
                int cnt = 0;
                rep(i, 3){
                    if (now[i][2-i] == -1) flag = false;
                    else cnt += now[i][2-i];
                }
                if (flag && cnt == 0){
                    return 0;
                }
            }
            {
                bool flag = true;
                ll x = 0, y = 0;
                rep(i, 3)rep(j, 3){
                    if (now[i][j] == -1) flag = false;
                    else{
                        if (now[i][j] == 1) x +=  a[i][j];
                        else y += a[i][j];
                    }
                }
                if (flag){
                    return x > y;
                }
            }
            return -1;
        };

        int flag = check(now);
        if (check(now) != -1){
            return flag == d;
        }
        if (memo.count({now, d})) return memo[{now, d}];
        int res = 1;
        rep(i, 3)rep(j, 3){
            if (now[i][j] == -1){
                now[i][j] = d;
                res &= f(f, now, d^1);
                now[i][j] = -1;
            }
        }
        return memo[{now, d}] = res^1;
    };

    if (dfs(dfs, vector<vector<int>>(3, vector<int>(3, -1)), 1)){
        cout << "Takahashi" << endl;
    }
    else{
        cout << "Aoki" << endl;
    }
    return 0;
}