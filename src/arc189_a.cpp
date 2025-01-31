// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
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
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    vector<mint> fact2(n, 1), fact(n, 1);
    rep(i, n-1) fact[i+1] = fact[i]*(i+1);
    for (int i = 0; i < n-2; i+=2){
        fact2[i+2] = fact2[i]*(i+2);
        fact2[i+3] = fact2[i+1]*(i+3);
    }
    if (a[0] == 0 || a.back() == ~n&1){
        cout << 0 << endl;
        return 0;
    }
    int tmp = 0, now = 1;
    vector<int> c;
    mint ans = 1;
    rep(i, n){  
        if (a[i] != now){
            if (~tmp&1){
                cout << 0 << endl;
                return 0;
            }
            tmp /= 2;
            if (tmp >= 1){
                c.emplace_back(tmp);
                ans *= fact2[2*tmp-1];
            }
            now = a[i];
            tmp = 1;
            continue;
        }
        tmp++;
    }
    tmp /= 2;
    if (tmp >= 1){
        c.emplace_back(tmp);
        ans *= fact2[2*tmp-1];
    }
    int sum_c = 0;
    for (auto v: c){
        sum_c += v;
        ans /= fact[v];
    }
    ans *= fact[sum_c];
    cout << ans.val() << endl;
    return 0;   
}