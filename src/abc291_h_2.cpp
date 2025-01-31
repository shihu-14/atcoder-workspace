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
    G g(n);
    rep(i, n-1){
        int a, b; cin >> a >> b; --a, --b;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    vector<int> ans(n, -1), tv(n);
    vector<bool> used(n);
    auto f2 = [&](auto f2, int now,  int pre=-1) -> int{
        int res = 1;
        for (int to: g[now]) if(!used[to]){
            if (to == pre) continue;
            res += f2(f2, to, now);
        }  
        return tv[now] = res;
    };

    auto f = [&](auto f, int s, int now, int pre=-1) -> pii{
        int mx_s = s-tv[now];
        pii res = {INF, now};
        for (int to: g[now]) if(!used[to]){
            if (to == pre) continue;
            res = min(res, f(f, s, to, now));
            chmax(mx_s, tv[to]); 
        }
        return min(res, pii(mx_s, now));
    };

    auto de = [&](auto de, int now, int pre=-1) -> int {
        int sz = f2(f2, now);
        int cv = f(f, sz, now).se;
        used[cv] = true;
        for(auto to: g[cv]) if (!used[to]){
            if (to == pre) continue;
            ans[de(de, to, cv)] = cv+1;
        }
        return cv;
    };

    de(de, 0);
    printv(ans);
    return 0;
}