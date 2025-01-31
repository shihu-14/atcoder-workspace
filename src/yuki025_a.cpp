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

ll gcd(ll a, ll b){ return (b ? gcd(b, a%b) : a); }
ll lcm(ll a, ll b){ return a/gcd(a, b)*b; }

// ax+by=gとなるg=gcd(a, b), x, yを求める拡張gcd
tuple<ll, ll, ll> extgcd(ll a, ll b) {
    if (b == 0) return {a, 1, 0};
    ll g, x, y;
    tie(g, x, y) = extgcd(b, a%b);
    return {g, y, x-a/b*y};
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll n, m; cin >> n >> m;
    ll g = gcd(n, m);
    n /= g, m /= g;
    int cnt2 = 0, cnt5 = 0; 
    while(m%2 == 0) m /= 2, cnt2++;
    while(m%5 == 0) m /= 5, cnt5++;
    if (m != 1){
        cout << -1 << endl;
        return 0;
    }
    while(n%10 == 0) n /= 10;
    if (cnt2 < cnt5){
        rep(i, cnt5 - cnt2){
            n *= 2;
            n %= 10;
        }
    }
    else if(cnt2 > cnt5){
        rep(i, cnt2 - cnt5){
            n *= 5;
            n %= 10;
        }        
    }
    cout << n%10 << endl;
    return 0;
}