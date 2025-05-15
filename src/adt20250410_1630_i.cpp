// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
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
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m, q; cin >> n >> m >> q;
    rep(qi, q){
        ll a, b, c, d; cin >> a >> b >> c >> d; b++, d++;
        mint ans;
        if ((a+c)%2 == 0){
            mint x1 = (a-1)*m+c;
            mint w1 = (d-c+1)/2;
            mint h1 = (b-a+1)/2;
            {
                mint sum_w = (x1+w1-1)*w1;
                ans += sum_w*h1+m*(h1-1)*h1*w1;
            }
            mint x2 = (a)*m+c+1;
            mint w2 = d-c-w1;
            mint h2 = b-a-h1;
            {
                mint sum_w = (x2+w2-1)*w2;
                ans += sum_w*h2+m*(h2-1)*h2*w2;
            }
        }
        else{
            mint x1 = (a)*m+c;
            mint w1 = (d-c+1)/2;
            mint h1 = (b-a)/2;
            {
                mint sum_w = (x1+w1-1)*w1;
                ans += sum_w*h1+m*(h1-1)*h1*w1;
            }
            mint x2 = (a-1)*m+c+1;
            mint w2 = d-c-w1;
            mint h2 = b-a-h1;
            {
                mint sum_w = (x2+w2-1)*w2;
                ans += sum_w*h2+m*(h2-1)*h2*w2;
            }
        }
        cout << ans.val() << "\n";
    }
    cout.flush();
    return 0;
}