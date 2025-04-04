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
using T5 = tuple<int, int, int, int, int>;
int dist[60][60][60][60];
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<string> g(n);
    rep(i, n) cin >> g[i];
    int s1x, s1y, s2x, s2y;
    {
        int t = 0;
        rep(i, n)rep(j, n){
            if (g[i][j] == 'P' && t == 0) {
                s1x = i, s1y = j;
                t++;
            }
            else if (g[i][j] == 'P' && t == 1) {
                s2x = i, s2y = j;
                t++;
            }
        }
    }
    rep(i, n)rep(j, n)rep(k, n)rep(l, n) dist[i][j][k][l] = INF;
    dist[s1x][s1y][s2x][s2y] = 0;
    priority_queue<T5, vector<T5>, greater<T5>> pq;
    pq.emplace(0, s1x, s1y, s2x, s2y);
    while(!pq.empty()){
        auto [d, x1, y1, x2, y2] = pq.top(); pq.pop();
        if (dist[x1][y1][x2][y2] < d) continue;
        rep(t, 4){
            int nx1 = x1+dx[t], ny1 = y1+dy[t], nx2 = x2+dx[t], ny2 = y2+dy[t];
            if (nx1 < 0 || nx1 >= n || ny1 < 0 || ny1 >= n){
                nx1 = x1, ny1 = y1;
            }
            if (nx2 < 0 || nx2 >= n || ny2 < 0 || ny2 >= n){
                nx2 = x2, ny2 = y2;
            }
            if (g[nx1][ny1] == '#'){
                nx1 = x1, ny1 = y1;
            }
            if (g[nx2][ny2] == '#'){
                nx2 = x2, ny2 = y2;
            }
            if (dist[nx1][ny1][nx2][ny2] > dist[x1][y1][x2][y2]+1){
                dist[nx1][ny1][nx2][ny2] = dist[x1][y1][x2][y2]+1;
                pq.emplace(dist[nx1][ny1][nx2][ny2], nx1, ny1, nx2, ny2);
            }
        }
    }
    int ans = INF;
    rep(i, n)rep(j, n){
        chmin(ans, dist[i][j][i][j]);
    }
    cout << (ans == INF ? -1 : ans) << endl;
    return 0;
}