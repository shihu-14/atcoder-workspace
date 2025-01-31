#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < n; ++i)
#define rep2(i, a, b) for (ll i = a; i < b; ++i)
#define rep3(i, a, b, c) for (ll i = a; i < b; i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
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

ll power(ll a, ll n){
    ll res = 1;
    for(; n; n>>=1, a*=a) if(n&1) res *= a;
    return res;
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<int> a(n);
    rep(i, n) cin >> a[i];
    rep(i, n) a[i] /= 2;
    int max_b = -INF;
    rep(i, n){
        int cnt = 0, tmp = a[i];
        while(tmp%2 == 0){
            tmp /= 2;
            cnt++;
        }
        chmax(max_b, cnt);
    }

    ll b = power(2, max_b);
    rep(i, n){
        if (a[i]%b != 0){
            cout << 0 << endl;
            return 0;
        }
    }
    m /= b;
    rep(i, n) a[i] /= b;

    ll l = lcm(a[0], 1);
    rep2(i, 1, n){
        l = lcm(l, a[i]);
        if (l > m){
            cout << 0 << endl;
            return 0;
        }
    }
    m /= l;
    int ans = (1+m)/2;
    cout << ans << endl;
    return 0;
}