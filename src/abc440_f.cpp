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
const int D = 1e6+10;
void solve()
{
    int n, q; cin >> n >> q;
    int cnt_1 = 0;
    ll sum = 0;
    vector<int> a(n), b(n);
    multiset<int> mb1, mb2;
    rep(i, n)
    {
        cin >> a[i] >> b[i];
        sum += a[i]*2;
        if (b[i] == 1)
        {
            cnt_1++;
            mb1.emplace(a[i]);
        }
        else
        {
            mb2.emplace(a[i]);
        }
    }

    fenwick_tree<ll> fw1(D), fw2(D);
    rep(i, n)
    {
        fw1.add(a[i], 1);
    }
    rep(i, n)
    {
        fw2.add(a[i], a[i]);
    }
    rep(qi, q)
    {
        int w, x, y; cin >> w >> x >> y; w--;
        // revert
        fw1.add(a[w], -1);
        fw2.add(a[w], -a[w]);
        if (b[w] == 1)
        {
            cnt_1--;
            mb1.erase(mb1.find(a[w]));
        }
        else
        {
            mb2.erase(mb2.find(a[w]));
        }
        sum -= a[w]*2;
        // update
        a[w] = x;
        b[w] = y;
        fw1.add(a[w], 1);
        fw2.add(a[w], a[w]);
        if (b[w] == 1)
        {
            cnt_1++;
            mb1.emplace(a[w]);
        }
        else
        {
            mb2.emplace(a[w]);
        }
        sum += a[w]*2;
        // query
        ll res = sum;
        if (mb1.size() == 0)
        {
            res -= *mb2.begin();
        }
        else
        {
            int wa = 0, ac = D-10;
            while(ac-wa > 1)
            {
                auto f = [&](int w)
                {
                    return fw1.sum(0, w+1) >= cnt_1;
                };
                int wj = (wa+ac)/2;
                if (f(wj)) ac = wj;
                else wa = wj;
            }
            int rem = cnt_1-fw1.sum(0, ac);
            res -= fw2.sum(0, ac);
            res -= (ll)rem*ac;
            if (mb2.size())
            {
                int mb1_max = *mb1.rbegin();
                int mb2_min = *mb2.begin();
                if (mb1_max < mb2_min)
                {
                    res -= mb2_min;
                    res += mb1_max;
                }
            }
        }
        cout << res << "\n";
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

