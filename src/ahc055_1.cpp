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

int n;
vector<int> H, C;
vector<vector<int>> a;


vector<pii> calculate(int target, vector<int> &buki, 
    vector<int> &h, vector<int> &c)
{
    vector<pii> res;
    set<pii> pq; //攻撃力のmax, そのbuki名 
    for (auto w: buki)
    {
        if (w == -1) pq.emplace(1, w);
        else pq.emplace(a[w][target], w);
    }
    while(h[target] > 0 && pq.size())
    {
        auto it = pq.lower_bound(pii{h[target], -1});
        if (it != pq.begin()) it--;
        auto [atk, w] = *it;
        h[target] -= atk;
        if (w != -1) c[w]--;
        res.emplace_back(w, target);
        if (w != -1 && c[w] == 0) pq.erase(it);
    }
    buki.clear();
    for (auto [atk, w]: pq)
    {
        buki.emplace_back(w);
    }
    return res;
}

vector<pii> evaluate(vector<int> &perm)
{
    vector<pii> res;
    vector<int> h = H, c = C, buki = {-1};
    rep(i, perm.size())
    {
        auto path = calculate(perm[i], buki, h, c);
        res.insert(res.end(), rng(path));
        buki.emplace_back(perm[i]); // add buki
    }
    return res;
}

vector<pii> optimize()
{
    auto start = chrono::steady_clock::now();
    vector<pii> res;
    int min_score = INF;
    vector<int> perm;
    rep(i, n) perm.emplace_back(i);
    // rand
    random_device rd;
    mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, n-1);
    while(1)
    {
        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - start).count();
        if (elapsed > 1850) break;
        // for revert
        int tmp_i = -1, tmp_j = -1;
        // 近傍そうさ
        {
            int i = -1, j = -1;
            while(i == j)
            { 
                i = dist(gen);
                j = dist(gen);
            }
            swap(perm[i], perm[j]);
            tmp_i = i; tmp_j = j;
        }
        auto path = evaluate(perm);
        int score = path.size();
        if (chmin(min_score, score))
        {
            res = path;
            continue;
        }
        // revert
        {
            swap(perm[tmp_i], perm[tmp_j]);
        }

    }
    return res;
}



void solve()
{
    cin >> n;
    H.resize(n);
    C.resize(n);
    rep(i, n) cin >> H[i];
    rep(i, n) cin >> C[i];
    a.resize(n, vector<int>(n));
    rep(i, n)rep(j, n) cin >> a[i][j];
    auto ans = optimize();
    for (auto[w, b]: ans)
    {
        cout << w << " " << b << "\n";
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
    return 0;
}