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
// using mint = modint998244353;
using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, ll, int>;
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
using T4 = tuple<ll, int, int, int>;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    while(1){
        int n, p, q;
        cin >> n >> p >> q;
        if(n == 0 && p == 0 && q == 0) return 0;
        vector<vector<T3>> g(n);
        rep(i, p){
            int a, b; ll c; cin >> a >> b >> c; a--, b--;
            g[a].emplace_back(b, c, 0);
            g[b].emplace_back(a, c, 0);
        }  
        rep(i, q){
            int a, b; ll c; cin >> a >> b >> c; a--, b--;
            g[a].emplace_back(b, c, 1);
            g[b].emplace_back(a, c, 1);
        }
        vector<int> distk(n, INF);
        {
            queue<int> q;
            q.emplace(0);
            distk[0] = 0;
            while(!q.empty()){
                int u = q.front(); q.pop();
                for(auto [v, c, r] : g[u]){
                    if (r == 0) continue;
                    if (distk[v] != INF) continue;
                    distk[v] = distk[u] + 1;
                    q.emplace(v);
                }
            }
        }
        int D = 35;
        vector<ll> two(D+1, 1);
        rep(i, D) two[i+1] = two[i]*2;
        vector<vector<vector<ll>>> dist(n, vector<vector<ll>>(D, vector<ll>(2, LINF)));
        dist[0][0][0] = 0;
        {
            priority_queue<T4, vector<T4>, greater<T4>> pq;
            pq.emplace(0, 0, 0, 0); 
            while(!pq.empty()){
                auto [d, u, j, f] = pq.top(); pq.pop();
                if (distk[u] >= D) continue;
                if (dist[u][j][f] < d) continue;
                for(auto [v, c, r] : g[u]){
                    if (r == 0){
                        if (dist[v][j][0] > dist[u][j][f] + c){
                            dist[v][j][0] = dist[u][j][f] + c;
                            pq.emplace(dist[v][j][0], v, j, 0);
                        }
                    }
                    else{
                        if (f == 0 && j+1 < D && dist[v][j+1][1] > dist[u][j][f] + c + two[j]){
                            dist[v][j+1][1] = dist[u][j][f] + c + two[j];
                            pq.emplace(dist[v][j+1][1], v, j+1, 1);
                            
                        }
                        if (f == 1 && dist[v][j][1] > dist[u][j][f] + c){
                            dist[v][j][1] = dist[u][j][f] + c;
                            pq.emplace(dist[v][j][1], v, j, 1);
                        }
                    }
                }
            }
        }
        vector<vector<pll>> dist2(n, vector<pll>(2, {LINF, LINF}));
        {
            priority_queue<T3, vector<T3>, greater<T3>> pq;
            rep(i, n) if (distk[i] == B) {
                pq.emplace(B, 0, i, 0);
                dist2[i][0] = {B, 0};
            }
            while(!pq.empty()){
                auto [d, u, f] = pq.top(); pq.pop();
                if (dist2[u][f].first < d) continue;
                for(auto [v, c, r] : g[u]){
                    if (r == 0){
                        if (dist2[v][f].first > dist2[u][f].first + c){
                            dist2[v][f] = {dist2[u][f].first + c, dist2[u][f].second};
                            pq.emplace(dist2[v][f].first, v, f);
                        }
                    }
                    else{
                        if (f == 0 && dist2[v][1].first > dist2[u][f].first + c + two[distk[u]]){
                            dist2[v][1] = {dist2[u][f].first + c + two[distk[u]], dist2[u][f].second + 1};
                            pq.emplace(dist2[v][1].first, v, 1);
                        }
                        if (f == 1 && dist2[v][1].second < dist2[u][f].second + 1){
                            dist2[v][1] = {dist2[u][f].first + c, dist2[u][f].second + 1};
                            pq.emplace(dist2[v][1].first, v, 1);
                        }
                    }
                }
            }
        }
        ll ans = LINF;
        rep(i, D)rep(j, 2){
            chmin(ans, dist[n-1][i][j]);
        }
        if (ans == LINF) ans = -1;
        else ans %= 1000000007;
        cout << ans << "\n";
    }
    return 0;
}