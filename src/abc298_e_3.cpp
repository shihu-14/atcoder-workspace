#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j].val()<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, a, b, p, q; cin >> n >> a >> b >> p >> q;

    mint x = mint(1)/p, y = mint(1)/q;
    map<T, mint> memo;
    auto f = [&](auto f, int a, int b, int c)-> mint{
        if (a == n || b == n) return (a==n ? 1 : 0);
        if (memo.count({a, b, c})) return memo[{a, b, c}];
        mint res = 0;
        if (c == 0){
            rep2(k, 1, p+1){
                res += f(f, min(a+k, (ll)n), b, 1)*x;
            }
        }
        else{
            rep2(k, 1, q+1){
                res += f(f, a, min(b+k, (ll)n), 0)*y;
            }
        }
        return memo[{a, b, c}] = res;
    };
    mint ans = f(f, a, b, 0);
    cout << ans.val() << endl;
    return 0;
}