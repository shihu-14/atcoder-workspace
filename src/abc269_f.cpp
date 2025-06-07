// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
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
mint sum(mint a, mint n){
    return (a+n-1)*n;
}
mint sum2(mint a, mint d, mint n){
    return (2*a+d*(n-1))*n/2;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m, q; cin >> n >> m >> q;
    vector<mint> ans;
    rep(qi, q){
        ll a, b, c, d; cin >> a >> b >> c >> d; b++; d++;
        if ((a+c)%2 == 1){
            ll r1 = (d-c)/2, r2 = (d-c+1)/2;
            mint s1 = sum((a-1)*m+c+1, r1);
            mint s2 = sum(a*m+c, r2);
            
            ll l1 = (b-a+1)/2, l2 = (b-a)/2;
            mint s3 = sum2(s1, 2*m*r1, l1);
            mint s4 = sum2(s2, 2*m*r2, l2);
            ans.emplace_back(s3+s4);            
        }
        else{
            ll r1 = (d-c+1)/2, r2 = (d-c)/2;
            mint s1 = sum((a-1)*m+c, r1);
            mint s2 = sum(a*m+c+1, r2);
           
            ll l1 = (b-a+1)/2, l2 = (b-a)/2;
            mint s3 = sum2(s1, 2*m*r1, l1);
            mint s4 = sum2(s2, 2*m*r2, l2);
            ans.emplace_back(s3+s4);
        }
    }
    for (auto x: ans) cout << x.val() << endl;
    return 0;
}