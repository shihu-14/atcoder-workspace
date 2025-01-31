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
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q; ll k; cin >> q >> k;
    CC<ll> cc;
    vector<pll> query;
    set<ll> st;
    rep(qi, q){
        int type; ll x; cin >> type >> x;
        query.emplace_back(type, x);
        cc.add(x);
    }
    cc.add(-LINF), cc.add(LINF);
    st.insert(-LINF), st.insert(LINF);
    int m = cc.size();
    fenwick_tree<int> fw(m);
    rep(i, m) fw.add(i, 0);
    for (auto [type, x]: query){
        if (type == 1){
            int j = cc(x);
            if (st.count(x)){
                auto it = st.find(x);
                ll pre = *prev(it), nxt = *next(it);
                int pj = cc(pre), nj = cc(nxt);
                if (abs(pre-x) <= k){
                    fw.add(j, -fw.sum(j, j+1));
                }
                if (abs(nxt-x) <= k){
                    fw.add(nj, -fw.sum(nj, nj+1));
                }
                if (abs(pre-nxt) <= k){
                    fw.add(nj, -fw.sum(nj, nj+1)+1);
                }
                st.erase(x);
            }
            else{
                st.insert(x);
                auto it = st.find(x);
                ll pre = *prev(it), nxt = *next(it);
                int pj = cc(pre), nj = cc(nxt);
                if (abs(pre-x) <= k){
                    fw.add(j, -fw.sum(j, j+1)+1);
                }
                if (abs(nxt-x) <= k){
                    fw.add(nj, -fw.sum(nj, nj+1)+1);
                }
                if (abs(pre-nxt) <= k){
                    fw.add(nj, -fw.sum(nj, nj+1));
                }
            }
        }
        else{
            int j = cc(x);
            int ans = 1;
            auto f1 = [&](int w) -> bool{
                int s = fw.sum(j+1, j+1+w);
                return s == w;
            };
            auto f2 = [&](int w) -> bool{
                int s = fw.sum(j-w, j+1);  
                return s == w+1;
            };
            int ac = 0, wa = m-j;
            while(wa-ac>1){
                int wj = (ac+wa)/2;
                if (f1(wj)) ac = wj;
                else wa = wj;
            }
            ans += ac;
            ac = 0, wa = j+1;
            while(wa-ac>1){
                int wj = (ac+wa)/2;
                if (f2(wj)) ac = wj;
                else wa = wj;
            }
            ans += ac;
            cout << ans << '\n';
        }
    }
    cout.flush();
    return 0;
}