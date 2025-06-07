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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w; cin >> h >> w;
    ll sum = 0;
    vector<vector<ll>> a(h, vector<ll>(w));
    rep(i, h)rep(j, w) cin >> a[i][j], sum ^= a[i][j];

    ll ans = 0;
    vector<vector<bool>> used(h, vector<bool>(w, false));
    auto dfs = [&](auto f, int x, int y, ll res=0) -> void{
        if (x == h-1 && y == w-1){
            chmax(ans, sum^res);
            return;
        }

        if (used[x][y]){
            if (y == w-1) f(f, x+1, 0, res);
            else f(f, x, y+1, res);
            return;
        }

        if (y == w-1){
            used[x][y] = true;
            f(f, x+1, 0, res);
            used[x][y] = false;
        }
        else{
            used[x][y] = true;
            f(f, x, y+1, res);
            used[x][y] = false;
        }

        
        if (y == w-1){
            used[x][y] = true;
            used[x+1][y] = true;
            f(f, x+1, 0, res^a[x][y]^a[x+1][y]);
            used[x][y] = false;
            used[x+1][y] = false;
        }
        else if (x == h-1){
            if (!used[x][y+1]){
                used[x][y] = true;
                used[x][y+1] = true;
                f(f, x, y+1, res^a[x][y]^a[x][y+1]);
                used[x][y] = false;
                used[x][y+1] = false;
            }
        }
        else{
            used[x][y] = true;
            used[x+1][y] = true;
            f(f, x, y+1, res^a[x][y]^a[x+1][y]);
            used[x][y] = false;
            used[x+1][y] = false;
            if (!used[x][y+1]){
                used[x][y] = true;
                used[x][y+1] = true;
                f(f, x, y+1, res^a[x][y]^a[x][y+1]);
                used[x][y] = false;
                used[x][y+1] = false;
            }
        }
    };
    dfs(dfs, 0, 0);
    cout << ans << endl;
    return 0;
}