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
const char dc[] = {'U', 'R', 'D', 'L'};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}
int n=20; 
vector<pair<char, int>> calc(vector<pii> op_que, map<pii, int> op_dir){
    vector<pair<char, int>> res;
    vector<vector<bool>> used(n, vector<bool>(n));
    for (auto [i, j]: op_que){
        if (used[i][j]) continue;
        int d = op_dir[{i, j}];
        int x = i, y = j;
        used[i][j] = true;
        while(1){
            int nx = x+dx[d], ny = y+dy[d];
            if (nx < 0 || nx >= n || ny < 0 || ny >= n){
                x = nx, y = ny;
                res.emplace_back(dc[d], (d==0 || d==2 ? j : i));
                break;
            }
            used[nx][ny] = true;
            x = nx, y = ny;
            res.emplace_back(dc[d], (d==0 || d==2 ? j : i));
        }
        if (d < 2) d += 2;
        else d -= 2;
        while(1){
            int nx = x+dx[d], ny = y+dy[d];
            if (nx == i && ny == j){
                res.emplace_back(dc[d], (d==0 || d==2 ? j : i));
                break;
            }
            x = nx, y = ny;
            res.emplace_back(dc[d], (d==0 || d==2 ? j : i));
        }
    }
    return res;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = std::chrono::high_resolution_clock::now();
    cin >> n;
    vector<string> g(n);
    rep(i, n) cin >> g[i];
    vector<vector<int>> row_huku(n), col_huku(n), row_oni(n), col_oni(n);
    set<pii> st_oni;
    vector<pii> op_que;
    rep(i, n){
        rep(j, n){
            if (g[i][j] == 'o') row_huku[i].emplace_back(j);
            if (g[i][j] == 'x') row_oni[i].emplace_back(j);
            if (g[i][j] == 'o') col_huku[j].emplace_back(i);
            if (g[i][j] == 'x') col_oni[j].emplace_back(i);
            if (g[i][j] == 'x'){
                st_oni.emplace(i, j); 
                op_que.emplace_back(i, j);
            }
        }
    }
    map<pii, int> mp_oni;
    for (auto [i, j]: st_oni){
        int min_d = n+1, min_dir = -1;
        int it = lower_bound(rng(row_huku[i]), j) - row_huku[i].begin();
        if (it == 0 && i+1 < min_d){
            min_d = i+1, min_dir = 3;
        }
        if (it == row_huku[i].size() && n-i < min_d){
            min_d = n-i, min_dir = 1;
        }
        it = lower_bound(rng(col_huku[j]), i) - col_huku[j].begin();
        if (it == 0 && j+1 < min_d){
            min_d = j+1, min_dir = 0;
        }
        if (it == col_huku[j].size() && n-j < min_d){
            min_d = n-j, min_dir = 2;
        }
        mp_oni[{i, j}] = min_dir;
    }

    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, op_que.size()-1);
    vector<pair<char, int>> ans;
    int max_score = INF;
    int times = 0, update_times = 0;
    while(1){
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (elapsed.count() > 1950) break;
        int t = 1;
        while(t--){
            int x = dist(gen), y = dist(gen);
            swap(op_que[x], op_que[y]);
        }
        auto res = calc(op_que, mp_oni);
        if (8*n*n-res.size() < max_score){
            update_times++;
            max_score = 8*n*n-res.size();
            ans = res;
        }
        times++;
    }
    cerr << "times: " << times << endl;
    cerr << "update: " << update_times << endl;
    cerr << "score: " << 8*n*n-ans.size() << endl;
    for (auto [c, d]: ans) cout << c << " " << d << endl;
    return 0;
}