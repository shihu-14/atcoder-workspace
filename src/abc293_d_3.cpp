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
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
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
    int n, m; cin >> n >> m;
    G g(2*n);
    rep(i, n){
        g[i].emplace_back(i+n);
        g[i+n].emplace_back(i);
    }
    int ansx = 0, ansy = 0;
    rep(i, m){
        int a, c; char b, d; cin >> a >> b >> c >> d;
        a--; c--;
        if (b == 'R') a += n;
        if (d == 'R') c += n;
        g[a].emplace_back(c);
        g[c].emplace_back(a);
    }
    vector<bool> used(2*n, false);
    auto f = [&](auto f, int now, int pre) -> bool{
        for (auto to : g[now]){
            if (to == pre) continue;
            if ((used[to] && abs(now-to) != n) || (used[to] && abs(now-to) == n && g[now].size() == 2)) return true;
            used[to] = true;
            if (f(f, to, now)) return true;
        }
        return false;
    };
    rep(i, n){
        if (used[i]) continue;
        used[i] = true;
        if (f(f, i, -1)) ansx++;
        else ansy++;
    }
    cout << ansx << " " << ansy << endl;
    return 0;
}