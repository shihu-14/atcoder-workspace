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
using T = tuple<ll, int, int, int, int>;
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
    map<int, ll> mpl, mpr;
    map<pii, int> memo;
    rep(i, n){
        int r, c, x; cin >> r >> c >> x;
        mpl[r] += x;
        mpr[c] += x;
        memo[{r, c}] = x;
    }

    vector<pll> suml, sumr;
    for(auto [k, v]: mpl){
        suml.emplace_back(v, k);
    }
    for(auto [k, v]: mpr){
        sumr.emplace_back(v, k);
    }
    sort(rrng(suml));
    sort(rrng(sumr));
    priority_queue<T> pq;
    pq.emplace(suml[0].first+sumr[0].first, 0, 0, suml[0].second, sumr[0].second);
    map<pii, bool> used; used[{0, 0}] = true;
    int t = 0;
    ll ans = -LINF;
    while(!pq.empty() && t <= n+1){
        auto [v, r, c, k1, k2] = pq.top(); pq.pop();
        if (memo.count({k1, k2})) v -= memo[{k1, k2}];
        chmax(ans, v);
        t++;
        if (r+1 <= suml.size() && !used[{r+1, c}]){
            pq.emplace(suml[r+1].first+sumr[c].first, r+1, c, suml[r+1].second, sumr[c].second);
            used[{r+1, c}] = true;
        }
        if (c+1 <= sumr.size() && !used[{r, c+1}]){
            pq.emplace(suml[r].first+sumr[c+1].first, r, c+1, suml[r].second, sumr[c+1].second);
            used[{r, c+1}] = true;
        }
    }
    cout << ans << endl;
    return 0;
}