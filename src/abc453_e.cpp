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
#define rrep2(i, a, b) for (ll i = int(a)-1; i >= (b); --i)
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

struct Combination
{
    vector<mint> fact, factinv;
    Combination(int n): fact(n+1), factinv(n+1)
    {
        fact[0] = 1;
        for(int i=1; i<=n; i++) fact[i] = fact[i-1]*i;
        factinv[n] = fact[n].inv();
        for(int i=n; i>0; i--) factinv[i-1] = factinv[i]*i;
    }

    mint operator()(int n, int k)
    {
        if(n < 0 || k < 0 || k > n) return 0;
        return fact[n]*factinv[k]*factinv[n-k];
    }

    mint power(mint a, ll b)
    {
        mint res = 1;
        for(; b; b>>=1, a*=a) if(b&1) res *= a;
        return res;
    }
};


void solve()
{
    int n; cin >> n;
    Combination C(n+1);

    mint ans;
    vector<int> cnt_a(n+1), cnt_b(n+1), cnt(n+1);
    rep(i, n)
    {
        int l1, r1; cin >> l1 >> r1;
        int l2 = n-r1, r2 = n-l1;
        l1--; l2--;
        cnt_a[l1]++; cnt_a[r1]--;
        cnt_b[l2]++; cnt_b[r2]--;
        if (l1 > l2)
        {
            swap(l1, l2);
            swap(r1, r2);
        }
        if (l2 <= r1)
        {
            cnt[l1]++;
            cnt[max(r1, r2)]--;
        }
        else
        {
            cnt[l1]++;
            cnt[r1]--;
            cnt[l2]++;
            cnt[r2]--;
        }
    }
    rep(i, n) cnt_a[i+1] += cnt_a[i];
    rep(i, n) cnt_b[i+1] += cnt_b[i];
    rep(i, n) cnt[i+1] += cnt[i];
    rep(i, n)
    {
        if (cnt[i] != n) continue;
        int s = cnt_a[i]+cnt_b[i]-n;
        if (cnt_a[i] == 0 || cnt_b[i] == 0) continue;
        int a = cnt_a[i]-s, b = cnt_b[i]-s;
        if (a <= i+1 && b <= n-i-1)
        {
            ans += C(s, i+1-a);
        }
    }
    cout << ans.val() << endl;
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