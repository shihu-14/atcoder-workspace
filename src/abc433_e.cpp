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

void solve()
{
    int n, m; cin >> n >> m;
    vector<int> X(n), Y(m);
    rep(i, n) cin >> X[i];
    rep(i, m) cin >> Y[i];
    {
        auto X_tmp = X, Y_tmp = Y;
        UNIQUE(X_tmp);
        UNIQUE(Y_tmp);
        if (X_tmp.size() != n || Y_tmp.size() != m)
        {
            cout << "No" << "\n";
            return;
        }
    }
    vector<vector<int>> ans(n, vector<int>(m, -1));
    rep(i, n)rep(j, m)
    {
        if (X[i] == Y[j]) 
        {
            ans[i][j] = X[i];
        }
    }
    rep(i, n)
    {
        bool flag = false;
        rep(j, m)
        {
            if (ans[i][j] == X[i])
            {
                flag = true;
                break;
            }
        }
        if (flag) continue;
        rep(j, m)
        {
            if (ans[i][j] == -1 && X[i] < Y[j])
            {
                ans[i][j] = X[i];
                flag = true;
                break;
            }
        }
        if (flag) continue;
        cout << "No" << "\n";
        return;
    }
    rep(j, m)
    {
        bool flag = false;
        rep(i, n)
        {
            if (ans[i][j] == Y[j])
            {
                flag = true;
                break;
            }
        }
        if (flag) continue;
        rep(i, n)
        {
            if (ans[i][j] == -1 && Y[j] < X[i])
            {
                ans[i][j] = Y[j];
                flag = true;
                break;
            }
        }
        if (flag) continue;
        cout << "No" << "\n";
        return;
    }

    vector<pii> a;
    vector<vector<bool>> used(n, vector<bool>(m, false));
    rep(i, n)rep(j, m)
    {
        if (ans[i][j] != -1)
        {
            int id = ans[i][j]-1;
            used[id/m][id%m] = true;
            continue;
        }
        a.emplace_back(min(X[i], Y[j]), i*m + j);
    }

    sort(rrng(a));
    int t = 0;
    rrep2(i, n*m, 0)
    {   
        int x = i / m;
        int y = i % m;
        if (used[x][y]) continue;
        if (a[t].fi < i+1)
        {
            cout << "No" << "\n";
            return;
        }
        int id = a[t].se;
        ans[id/m][id%m] = i+1;
        t++;
    }

    cout << "Yes" << "\n";
    rep(i, n)
    {
        rep(j, m)
        {
            cout << ans[i][j] << " ";
        }
        cout << "\n";
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    cin >> t;
    while(t--)
    {
        solve();
    }
    return 0;
}