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

void solve()
{
    cin >> n;
    H.resize(n);
    C.resize(n);
    rep(i, n) cin >> H[i];
    rep(i, n) cin >> C[i];
    a.resize(n, vector<int>(n));
    rep(i, n) rep(j, n) cin >> a[i][j];

    vector<int> h = H, c = C;
    vector<char> used(n, 0);
    int used_cnt = 0;

    vector<ll> S(n, 0); 

    ll sumH = 0;
    rep(i, n) sumH += H[i];
    vector<pii> ans;
    ans.reserve((int)sumH);

    const int threshold = 5; 

    while (used_cnt < n)
    {
        int best_w = -2, best_b = -1, best_score = -1, best_eff = -1;
        rep(w, n)
        {
            if (!used[w]) continue;
            if (c[w] == 0) continue;
            rep(b, n)
            {
                if (used[b]) continue;
                int A = a[w][b];
                if (A <= 1) continue;
                int d = min(A, h[b]);
                int score = d - 1;
                if (score > best_score || (score == best_score && h[b] > best_eff))
                {
                    best_score = score;
                    best_eff = h[b];
                    best_w = w;
                    best_b = b;
                }
            }
        }


        int current_threshold = (used_cnt < n * 0.8) ? threshold : 0;

        if (best_score > current_threshold)         
        {
            int A = a[best_w][best_b];
            int need = (h[best_b] + A - 1) / A;
            int k = min(c[best_w], need);
            rep(t, (ll)k) ans.emplace_back(best_w, best_b);
            h[best_b] -= k * A;
            c[best_w] -= k;
            if (h[best_b] <= 0)
            {
                used[best_b] = 1;
                used_cnt++;
            }
            continue;
        }

        
        rep(i, n)
        {
            if (used[i])
            {
                S[i] = -1; 
                continue;
            }
            S[i] = 0;
            vector<int> v;
            rep(j, n)
            {
                if (!used[j] && h[j] > 0) 
                {
                    v.emplace_back(max(0, min(a[i][j], h[j]) - 1));
                }
            }
            int k = min((int)v.size(), C[i]); 
            if (k > 0)
            {
                nth_element(v.begin(), v.begin() + k, v.end(), greater<int>());
                rep(t, k) S[i] += v[t];
            }
        }

        int tmp_b = -1;
        double bestR = -1.0;
        rep(b, n)
        {
            if (used[b]) continue;
            if (h[b] <= 0) continue;
            
            double R = (double)S[b] / (double)h[b]; 
            
            if (R > bestR || (R == bestR && h[b] < h[tmp_b >= 0 ? tmp_b : b]))
            {
                bestR = R;
                tmp_b = b;
            }
        }
        if (tmp_b == -1)
        {
            rep(b, n)
            {
                if (!used[b] && h[b] > 0)
                { 
                    tmp_b = b;
                    break;
                }
            }
        }
        int k = h[tmp_b];
        rep(t, (ll)k) ans.emplace_back(-1, tmp_b);
        h[tmp_b] = 0;
        used[tmp_b] = 1;
        used_cnt++;
    }

    for (auto [w, b] : ans)
    {
        cout << w << " " << b << endl;
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;

    while(t--)
    {
        solve();
    }
    return 0;
}