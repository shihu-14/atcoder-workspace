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

template<typename T=int>
struct CC {
    bool initialized;
    vector<T> xs;
    CC(): initialized(false) {}
    void add(T x) { xs.push_back(x);}
    void init() {
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(),xs.end()),xs.end());
        initialized = true;
    }
    int operator()(T x) {
        if (!initialized) init();
        return upper_bound(xs.begin(), xs.end(), x) - xs.begin() - 1;
    }
    T operator[](int i) {
        if (!initialized) init();
        return xs[i];
    }
    int size() {
        if (!initialized) init();
        return xs.size();
    }
};


void solve()
{
    int n, m, q; cin >> n >> m >> q;
    vector<ll> a(n), b(m);
    CC<ll> cc;
    rep(i, n) cin >> a[i], cc.add(a[i]);
    rep(i, m) cin >> b[i], cc.add(b[i]);
    vector<T3> query; query.reserve(q);
    rep(qi, q)
    {
        int t, i, x; cin >> t >> i >> x; i--;
        query.emplace_back(t, i, x);
        cc.add(x);
    }
    cc.init();
    fenwick_tree<ll> fw_cnt(cc.size()), fw_sum(cc.size());
    rep(i, n)
    {
        int j = cc(a[i]);
        fw_cnt.add(j, 1);
        fw_sum.add(j, a[i]);
    }
    rep(i, m)
    {
        int j = cc(b[i]);
        fw_cnt.add(j, 1);
        fw_sum.add(j, b[i]);
    }
    for (auto [type, i, x]: query)
    {
        if (type == 1)
        {
            fw_cnt.add(cc(a[i]), -1);
            fw_sum.add(cc(a[i]), -a[i]);
            a[i] = x;
            fw_cnt.add(cc(a[i]), 1);
            fw_sum.add(cc(a[i]), a[i]);
        }
        else
        {
            fw_cnt.add(cc(b[i]), -1);
            fw_sum.add(cc(b[i]), -b[i]);
            b[i] = x;
            fw_cnt.add(cc(b[i]), 1);
            fw_sum.add(cc(b[i]), b[i]);
        }
        auto f = [&]() ->ll
        {
            ll res = 0;
            {
                int wa = -1, ac = cc.size()-1;
                while(ac-wa>1)
                {
                    int wj = (wa+ac)/2;
                    ll cnt = fw_cnt.sum(0, wj+1);
                    if (cnt*2 >= n)
                    {
                        ac = wj;
                    }
                    else
                    {
                        wa = wj;
                    }
                }
                ll rem = n/2-fw_cnt.sum(0, ac);
                res += fw_sum.sum(0, ac) + rem * cc[ac];
            }
            {
                int ac = 0, wa = cc.size();
                while(wa-ac>1)
                {
                    int wj = (wa+ac)/2;
                    ll cnt = fw_cnt.sum(wj, cc.size());
                    if (cnt*2 >= n)
                    {
                        ac = wj;
                    }
                    else
                    {
                        wa = wj;
                    }
                }
                ll rem = n/2-fw_cnt.sum(ac+1, cc.size());
                res += fw_sum.sum(ac+1, cc.size()) + rem * cc[ac];
            }
            return res;
        };
        ll ans = f();
        cout << ans << '\n';
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