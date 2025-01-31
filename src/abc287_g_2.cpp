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
template<typename T=int>
struct CC {
    bool initialized;
    vector<T> xs;
    CC(): initialized(false) {}
    void add(T x) { xs.push_back(x);}
    void init() {
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(),xs.end()),xs.end());
        initialized = true;
    }
    int operator()(T x) {
        if (!initialized) init();
        return upper_bound(xs.begin(), xs.end(), x) - xs.begin() - 1;
    }
    T operator[](int i) {
        if (!initialized) init();
        return xs[i];
    }
    int size() {
        if (!initialized) init();
        return xs.size();
    }
};
int target;
int op(int a, int b){return a+b;}
int e(){return 0;}
bool f(int x){return x <= target;}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    CC c;
    vector<pii> kind;
    map<int, int> mp;
    rep(i, n){
        int a, b; cin >> a >> b;
        kind.emplace_back(a, b);
        c.add(a);
        mp[a] += b;
    }
    int q; cin >> q;
    vector<T> query;
    rep(i, q){
        int type; cin >> type;
        if (type == 3){
            int x; cin >> x;
            query.emplace_back(type, x, -1);
        }
        else{
            int x, y; cin >> x >> y;
            if (type == 1) c.add(y);
            query.emplace_back(type, x, y);
        }
    }
    int m = c.size();
    segtree<int, op, e> seg(m);
    fenwick_tree<ll> fw(m);
    rep(i, m) seg.set(i, mp[c[i]]);
    rep(i, m) fw.add(i, (ll)c[i]*mp[c[i]]);

    for (auto [t, x, y]: query){
        if (t == 1){
            x--;
            auto [a, b] = kind[x];
            mp[a] -= b;
            mp[y] += b;
            kind[x].fi = y;
            seg.set(c(a), mp[a]);
            seg.set(c(y), mp[y]);
            fw.add(c(a), -(ll)a*b);
            fw.add(c(y), (ll)y*b);
        }
        else if (t == 2){
            x--;
            auto [a, b] = kind[x];
            mp[a] -= b;
            mp[a] += y;
            kind[x].se = y;
            seg.set(c(a), mp[a]);
            fw.add(c(a), (ll)a*(y-b));
        }
        else{
            if (seg.all_prod() < x){
                cout << -1 << '\n';
                continue;
            }
            ll ans = 0;
            target = x;
            int l = seg.min_left<f>(m);
            int sum_b = seg.prod(l, m);
            x -= sum_b;
            ans += fw.sum(l, m);
            if (l > 0) ans += (ll)x*c[l-1];
            cout << ans << '\n';
        }
    }
    cout.flush();
    

    return 0;
}