// #pragma GCC target("avx2")
// #pragma GCC optimize("O3")
// #pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using namespace boost::multiprecision;

// using mint = modint;
using mint = modint998244353;
// using mint = modint1000000007;

using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;

#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)
#define rep2(i, a, b) for (ll i = (ll)(a); i < (ll)(b); ++i)
#define rrep(i, n) for (ll i = (ll)(n) - 1; i >= 0; --i)
#define rrep2(i, a, b) for (ll i = (ll)(a) - 1; i >= (ll)(b); --i)
#define rep3(i, a, b, c) for (ll i = (ll)(a); i < (ll)(b); i += (ll)(c))
#define rng(a) (a).begin(), (a).end()
#define rrng(a) (a).rbegin(), (a).rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), (v).erase(unique(rng(v)), (v).end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v), 0LL)
#define IN(v, x) (find(rng(v), (x)) != (v).end())

template<class T> bool chmin(T& a, const T& b){if(a>b){a=b;return true;}return false;}
template<class T> bool chmax(T& a, const T& b){if(a<b){a=b;return true;}return false;}
template<class T> void printv(const vector<T>& v){rep(i,v.size()) cout<<v[i]<<" \n"[i==(ll)v.size()-1];}
template<class T> void printvv(const vector<vector<T>>& v){for(const auto& row:v) printv(row);}

constexpr ll dx[] = {-1, 0, 1, 0};
constexpr ll dy[] = {0, 1, 0, -1};
constexpr ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
constexpr ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
constexpr ll LINF = 3001002003004005006LL;
constexpr int INF = 1001001001;

int op(int a, int b)
{
    return a+b;
}
int e()
{
    return 0;
}
void solve()
{
    int n, q; cin >> n >> q;
    vector<int> a(n+2); a[n+1] = -2; a[0] = -3;
    rep2(i, 1, n+1) cin >> a[i];
    segtree<int, op, e> seg(n+2);
    set<int> s;
    rep(i, n+2)
    {
        if (a[i] != -1)
        {
            s.emplace(i);
        }
    }
    rep(i, n+1)
    {
        if (s.count(i))
        {
            auto r = *next(s.find(i));
            seg.set(i, a[i] != a[r]);
        }
    } 
    rep(qi, q)
    {
        int type; cin >> type; 
        if (type == 1)
        {
            int i, x; cin >> i >> x;
            if (a[i] == x) continue;
            a[i] = x;
            if (x == -1)
            {
                auto it = s.find(i);
                int l = *prev(it);
                int r = *next(it);
                seg.set(l, a[l] != a[r]);
                seg.set(i, 0);
                s.erase(i);
            }
            else
            {
                s.emplace(i);
                auto it = s.find(i);
                int l = *prev(it);
                int r = *next(it);
                seg.set(l, a[l] != a[i]);
                seg.set(i, a[i] != a[r]);
            }
        }
        else
        {
            int l, r; cin >> l >> r;
            l = *s.lower_bound(l);
            r = *prev(s.upper_bound(r));
            if (l >= r)
            {
                cout << "Yes" << '\n';
                continue;
            }
            cout << (seg.prod(l, r) == 0 ? "Yes": "No") << '\n';
        }
        // cout << "--------------- " << qi <<  " ---------------\n";
        // printv(a);
        // rep(i, n+2) cout << seg.get(i) << " ";
        // cout << '\n';
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--)
    {
        solve();
    }

    return 0;
}