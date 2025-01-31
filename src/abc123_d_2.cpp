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
using T = tuple<ll, int, int, int>;
using T2 = tuple<int, int, int>;
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
    int x, y, z, k; cin >> x >> y >> z >> k;
    vector<ll> a(x), b(y), c(z);
    rep(i,x) cin >> a[i];
    rep(i,y) cin >> b[i];
    rep(i,z) cin >> c[i];
    sort(rrng(a));
    sort(rrng(b));
    sort(rrng(c));
    priority_queue<T> pq;
    pq.emplace(a[0]+b[0]+c[0], 0, 0, 0);
    map<T2, bool> used;
    used[{0, 0, 0}] = true;
    while(!pq.empty() && k--){
        auto [v, i, j, k] = pq.top(); pq.pop();
        cout << v << endl;
        if(i+1 < x && !used[{i+1, j, k}]) pq.emplace(a[i+1]+b[j]+c[k], i+1, j, k), used[{i+1, j, k}] = true;
        if(j+1 < y && !used[{i, j+1, k}]) pq.emplace(a[i]+b[j+1]+c[k], i, j+1, k), used[{i, j+1, k}] = true;
        if(k+1 < z && !used[{i, j, k+1}]) pq.emplace(a[i]+b[j]+c[k+1], i, j, k+1), used[{i, j, k+1}] = true;
    }
    return 0;
}