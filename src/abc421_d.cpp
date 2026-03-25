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
using T3 = tuple<ll, ll, ll>;
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

void solve()
{
    ll sax, say, sbx, sby;
    cin >> sax >> say >> sbx >> sby;
    ll n, M, L; cin >> n >> M >> L;  
    vector<pll> as, bs;
    rep(i, M)
    {
        char c; ll a; cin >> c >> a;
        if (c == 'U') as.emplace_back(0, a);
        else if (c == 'R') as.emplace_back(1, a);
        else if (c == 'D') as.emplace_back(2, a);
        else if (c == 'L') as.emplace_back(3, a);
    }
    rep(i, L)
    {
        char c; ll a; cin >> c >> a;
        if (c == 'U') bs.emplace_back(0, a);
        else if (c == 'R') bs.emplace_back(1, a);
        else if (c == 'D') bs.emplace_back(2, a);
        else if (c == 'L') bs.emplace_back(3, a);
    }
    vector<T3> query;
    for (int i = 0, j = 0; i < M && j < L; ) {
        ll w = min(as[i].se, bs[j].se);
        query.emplace_back(w, as[i].fi, bs[j].fi);
        as[i].se -= w; if (as[i].se == 0) ++i;
        bs[j].se -= w; if (bs[j].se == 0) ++j;
    }
    
    ll ans = 0;
    ll ax = sax, ay = say, bx = sbx, by = sby;
    for (auto [w, d1, d2]: query)
    {
        if (d1 == d2 && ax == bx && ay == by)
        {
            ans += w;
        }
        else if (abs(d1-d2) == 2)
        {
            bool flag = false;
            if (ax == bx && (d1 == 1 || d1 == 3))
            {
                ll tmp_a = ay, tmp_b = by;
                if (d1 == 3)
                {
                    flag = true;
                    swap(tmp_a, tmp_b);
                    swap(d1, d2);
                }
                if (tmp_a < tmp_b && (tmp_b-tmp_a)%2 == 0 && (tmp_b - tmp_a)/2 <= w)
                {
                    ans++;
                }
            }
            else if (ay == by && (d1 == 0 || d1 == 2))
            {
                ll tmp_a = ax, tmp_b = bx;
                if (d1 == 0)
                {
                    flag = true;
                    swap(tmp_a, tmp_b);
                    swap(d1, d2);
                }
                if (tmp_a < tmp_b && (tmp_b-tmp_a)%2 == 0 && (tmp_b - tmp_a)/2 <= w)
                {
                    ans++;
                }
            }
            if (flag)
            {
                swap(d1, d2);
            }
        }
        else if (abs(d1-d2) == 1 || abs(d1-d2) == 3)
        {
            ll tmp_ax = ax, tmp_ay = ay, tmp_bx = bx, tmp_by = by;
            bool flag = false;
            if ((abs(d1-d2)==1 && d1 > d2) || (abs(d1-d2)==3 && d1 < d2))
            {
                flag = true;
                swap(tmp_ax, tmp_bx);
                swap(tmp_ay, tmp_by);
                swap(d1, d2);                
            }
            bool ok =
            (d1==0 && tmp_ax>tmp_bx && tmp_ay>tmp_by) ||
            (d1==1 && tmp_ax>tmp_bx && tmp_ay<tmp_by) || 
            (d1==2 && tmp_ax<tmp_bx && tmp_ay<tmp_by) ||
            (d1==3 && tmp_ax<tmp_bx && tmp_ay>tmp_by); 
            if (ok && abs(tmp_ax-tmp_bx) == abs(tmp_ay - tmp_by) 
            && 0 < abs(tmp_ax - tmp_bx) 
            && abs(tmp_ax - tmp_bx) <= w)
            {
                ans++;
            }
            if (flag)
            {
                swap(d1, d2);
            }

        }
        ax += w*dx[d1], ay += w*dy[d1];
        bx += w*dx[d2], by += w*dy[d2];
    }
    cout << ans << endl;
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