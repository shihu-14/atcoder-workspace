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
    rep(i,n) cin >> a[i];
    vector<ll> odd, even;
    ll sum = 0;
    rep(i, n){
        sum += a[i];
        if (i&1) odd.emplace_back(2*a[i]);
        else even.emplace_back(2*a[i]);
    }
    rep(i, odd.size()-1) odd[i+1] += odd[i];
    rep(i, even.size()-1) even[i+1] += even[i];
    int os = odd.size(), es = even.size();
    odd.insert(odd.begin(), 0);
    even.insert(even.begin(), 0);
    for(int i = 0; i < n-1; i++){
        ll res = 0;
        if (i&1){
            res += even[es] - even[(i+1)/2];
            if ((i-1)/2 >= 0) res += odd[(i-1)/2];
        }
        else{
            res += odd[os] - odd[i/2];
            res += even[i/2];
        }
        cout << sum-res << " ";
    }
    cout << sum-even[es-1] << " ";
    cout << endl;
    return 0;
}