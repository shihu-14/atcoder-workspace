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
using S = ll;
using F = ll;
S op(S a, S b) { return max(a, b);}
S e() { return 0; }
S mapping(F f, S x) { return f+x; }
F composition(F f, F g) { return f+g; }
F id() { return 0; }

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; ll d, a; cin >> n >> d >> a;
    vector<pll> p(n);
    rep(i, n) cin >> p[i].first >> p[i].second;
    sort(rng(p));
    lazy_segtree<S, op, e, F, mapping, composition, id> seg(n);
    rep(i, n) seg.set(i, p[i].second);
    ll ans = 0;
    d *= 2;
    rep(i, n){
        ll h = seg.get(i);
        if (h <= 0) continue;
        ll q = (h+a-1)/a;
        ans += q;
        ll damage = q*a;
        int it = lower_bound(p.begin(), p.end(), pll(p[i].first+d, -1)) - p.begin();
        if (it == n || p[it].first != p[i].first+d){
            it--;
        }
        seg.apply(i, it+1, -damage);
    }
    cout << ans << endl;
    return 0;
}