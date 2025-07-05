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

// template<typename T>
struct Bell{
    int n, ans=INF; // 4213597(n=12), 27644437(n=13), 190899322(n=14)
    vector<vector<int>> g;
    // add other data
    Bell(vector<vector<int>> &_g): n(_g.size()), g(_g) {}
    void build(){
        vector<vector<int>> groups;
        auto op = [&](){
            int res = 0;
            rep(i, groups.size()){
                if (groups[i].size() < 3) return;
                vector<int> vs = groups[i];
                sort(rng(vs));
                int res2 = INF;
                do{
                    int m = vs.size();
                    int tmp = 0;
                    rep(j, vs.size()){
                        rep(k, n){
                            if (k == vs[(j+1)%m]){
                                tmp += g[vs[j]][k]==0;
                            }
                            else if (k == vs[(j-1+m)%m]){
                                tmp += g[vs[j]][k]==0;
                            }
                            else{
                                tmp += g[vs[j]][k];
                            }
                        }
                    }
                    chmin(res2, tmp);
                } while (next_permutation(rng(vs)));
                res += res2;
            }
            chmin(ans, res/2);
        };
        auto dfs = [&](auto f, int u=0) -> void{
            if (u == n){
                op();
                return;
            }
            rep(i, groups.size()){
                groups[i].emplace_back(u);
                f(f, u+1);
                groups[i].pop_back();
            }
            groups.emplace_back(vector<int>{u});
            f(f, u+1);
            groups.pop_back();
        };
        dfs(dfs);
    }
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<int>> g(n, vector<int>(n));
    rep(i, m){
        int u, v; cin >> u >> v;
        u--; v--;
        g[u][v] = 1;
        g[v][u] = 1;
    }
    Bell bell(g);
    bell.build();
    cout << bell.ans << endl;
    return 0;
}