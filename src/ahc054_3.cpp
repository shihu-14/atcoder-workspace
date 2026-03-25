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
template<class T>
using G = vector<vector<T>>;
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


using T4 = tuple<int, int, int, int>;

int n, gx, gy;
vector<vector<char>> g;

// for debug
int debug_count = 0;


// 


bool isValid(int sx=1, int sy=n/2+1)
{
    queue<pii> q;
    q.emplace(sx, sy);
    vector<vector<int>> dist(n+2, vector<int>(n+2, INF));
    dist[sx][sy] = 0;
    while(!q.empty())
    {
        auto [x, y] = q.front(); q.pop();
        if (x == gx && y == gy) return true;
        rep(i, 4)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(g[nx][ny] == 'T') continue;
            if(dist[nx][ny] != INF) continue;
            dist[nx][ny] = dist[x][y] + 1;
            q.emplace(nx, ny);
        }
    }
    return false;
}

vector<pii> alignment()
{
    auto start = chrono::steady_clock::now();
    vector<pii> res;
    rep3(i, 1, n+1, 2)
    {
        rep3(j, 1, n+1, 2)
        {
            if (g[i][j] != '.') continue;
            res.emplace_back(i, j);
        }
    }
    random_device rd;
    mt19937 gen(rd());
    int located_size = res.size(), changed_times = 0, max_changed_times = 1;
    int t = 1;
    auto f = [&](int x)
    {
        return max(2000, x*x*x*x*x);
    };
    while(1)
    {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start).count();
        if (elapsed > 1800) break;
        if (located_size <= 0) break;
        shuffle(rng(res), gen);
        // place trees
        rep(i, located_size)
        {
            auto [x, y] = res[i];
            g[x][y] = 'T';
        }
        if (isValid())
        {
            res.erase(res.begin()+located_size, res.end());
            return res;
        }
        // revert
        rep(i, located_size)
        {
            auto [x, y] = res[i];
            g[x][y] = '.';
        }
        // adjust located_size
        if (changed_times < max_changed_times)
        {
            changed_times++;
        }
        else
        {
            changed_times = 0;
            max_changed_times = f(t++);
            located_size--;
        }
    }
    return {};
}

void solve()
{
    cin >> n >> gx >> gy; gx++; gy++;
    g.resize(n+2, vector<char>(n+2, 'T'));
    rep(i, n)rep(j, n)
    {
        cin >> g[i+1][j+1];
    }
    g[1][n/2+1] = 'S'; // start
    g[gx][gy] = 'G'; // goal
    int x = -1, y = -1;
    // first
    {
        cin >> x >> y; x++; y++;
        debug_count++;
        { // for judge
            int m; cin >> m;
            rep(i, m)
            {
                int a, b; cin >> a >> b; a++; b++;
            }
        }
        auto res = alignment();
        cout << res.size() << " ";
        for (auto [a, b] : res) cout << a-1 << " " << b-1 << " ";
        cout << endl;
    }
    // loop
    while(1)
    {
        cin >> x >> y;
        x++; y++;
        debug_count++;
        { // for judge
            int m; cin >> m;
            rep(i, m)
            {
                int a, b; cin >> a >> b; a++; b++;
            }
        }
        if (x == gx && y == gy)
        {
            return;
        }
        cout << 0 << endl;
    } 
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    // cin >> t;
    while(t--)
    {
        solve();
    }
    // for debug
    {
        // cerr << "debug_count: " << debug_count << endl;
    }
    return 0;
}