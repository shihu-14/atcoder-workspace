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

void solve()
{
    int n; cin >> n;
    vector<int> a(n), b(n);
    rep(i, n) cin >> a[i];
    rep(i, n) cin >> b[i];

    bool flag = false;
    rep(t, 2)
    {
        int stat = 0;
        int i = 0, j = 0;
        int move_id_1 = -1, move_id_2 = -1;
        auto tmp_a = a;
        auto tmp_b = b;
        while(i < n && j < n)
        {
            // cout << i << " " << j << " " << stat << '\n';
            if (a[i] == b[j])
            {
                i++; 
                j++;
            }
            else if (stat == 0)
            {
                stat++;
                if (t == 0)
                {
                    move_id_1 = i;
                    i++;
                }
                else
                {
                    move_id_1 = j;
                    j++;
                }
            }
            else if (stat == 1)
            {
                stat++;
                if (t == 0)
                {
                    move_id_2 = j;
                    j++;
                }
                else
                {
                    move_id_2 = i;
                    i++;
                }
            }
            else
            {
                break;
            }
        }
        // cout << i << " " << j << '\n';
        if (stat == 0)
        {
            flag = true;
            break;
        }
        if (i == n-1 && j == n)
        {
            move_id_2 = i;
        }
        else if (i == n && j == n-1)
        {
            move_id_2 = j;
        }
        else if (i == n && j == n)
        {
            
        }
        else
        {
            continue;
        }
        if (t == 1) swap(move_id_1, move_id_2);
        tmp_a.erase(tmp_a.begin()+move_id_1);
        tmp_b.erase(tmp_b.begin()+move_id_2);
        // printv(tmp_a);
        // printv(tmp_b);
        if (tmp_a == tmp_b && a[move_id_1] == b[move_id_2])
        {
            flag = true;
        }
    }
    cout << (flag ? "Yes": "No") << '\n';
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