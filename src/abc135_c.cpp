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
#define rep(i, n) for (ll i = 0; i < n; ++i)
#define rep2(i, a, b) for (ll i = a; i < b; ++i)
#define rrep2(i, a, b) for (ll i = a; i > b; --i)
#define rep3(i, a, b, c) for (ll i = a; i < b; i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
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
    vector<int> a(n+1), b(n);
    rep(i, n+1) cin >> a[i];
    rep(i, n) cin >> b[i];

    ll ans = 0;
    if (a[0] <= b[0]){
        ans += a[0];
        b[0] -= a[0];
    }
    else{
        ans += b[0];
        b[0] = 0;
    }
    if (a[n] <= b[n-1]){
        ans += a[n];
        b[n-1] -= a[n];
    }
    else{
        ans += b[n-1];
        b[n-1] = 0;
    }
    rep2(i, 1, n){
        if (a[i] <= b[i-1]){
            ans += a[i];
            a[i] = 0;
        }
        else{
            ans += b[i-1];
            a[i] -= b[i-1];
        }
        if (a[i] <= b[i]){
            ans += a[i];
            b[i] -= a[i];
        }
        else{
            ans += b[i];
            b[i] = 0;
        }
    }
    cout << ans << endl;
    return 0;
}