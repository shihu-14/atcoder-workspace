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
void solve(){
    ll n, k; cin >> n >> k; n++;
    vector<int> d;
    while(n){
        d.emplace_back(n%2);
        n /= 2;
    }
    reverse(rng(d));
    int m = d.size();
    vector dp2(k+1, vector<mint>(2)); dp2[0][0] = 1;
    vector dp(k+1, vector<mint>(2));
    rep(i, m){
        vector old2(k+1, vector<mint>(2)); swap(old2, dp2);
        vector old(k+1, vector<mint>(2)); swap(old, dp);
        rep(x, k+1)rep(f, 2){
            if (old2[x][f] == 0) continue;
            rep(nd, 2){
                if (f == 0){
                    if (d[i] == 1){
                        if (nd == 1 && x+1 <= k){
                            dp[x+1][0] += old[x][f]+old2[x][f]*(1LL<<(m-1-i));
                            dp2[x+1][0] += old2[x][f];
                        }
                        else if (nd == 0){
                            dp[x][1] += old[x][f];
                            dp2[x][1] += old2[x][f];
                        }
                    }
                    else{
                        if (nd == 0){
                            dp[x][0] += old[x][f];
                            dp2[x][0] += old2[x][f];
                        }
                    }
                }
                else{
                    if (nd == 1 && x+1 <= k){
                        dp[x+1][1] += old[x][f]+old2[x][f]*(1LL<<(m-1-i));
                        dp2[x+1][1] += old2[x][f];
                    }
                    else if (nd == 0){
                        dp[x][1] += old[x][f];
                        dp2[x][1] += old2[x][f];
                    }
                }
            }
        }
    }
    mint ans = dp[k][1];
    cout << ans.val() << "\n";
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    while(t--){
        solve();
    }
    cout.flush();
    return 0;
}