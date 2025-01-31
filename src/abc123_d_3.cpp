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
    int x, y, z, K; cin >> x >> y >> z >> K;
    vector<ll> a(x), b(y), c(z);
    rep(i,x) cin >> a[i];
    rep(i,y) cin >> b[i];
    rep(i,z) cin >> c[i];
    sort(rrng(a));
    sort(rrng(b));
    sort(rrng(c));

    auto f = [&](ll w) -> bool{
        int res = 0;
        rep(i, x)rep(j, y)rep(k, z){
            if (a[i]+b[j]+c[k] < w) break;
            res++;
            if (res >= K) return true;
        }
        return false;
    };

    ll ac = 0, wa = LINF;
    while(wa-ac > 1){
        ll wj = (wa+ac)/2;
        if (f(wj)) ac = wj;
        else wa = wj;
    }

    vector<ll> ans;
    rep(i, x)rep(j, y)rep(k, z){
        if (a[i]+b[j]+c[k] <= ac) break;
        ans.emplace_back(a[i]+b[j]+c[k]);
    }
    sort(rrng(ans));
    rep(i, ans.size()) cout << ans[i] << endl;
    K -= ans.size();
    rep(i, K) cout << ac << endl;
    return 0;
}