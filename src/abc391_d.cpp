// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
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
#define popcount_ll __builtin_popcountll
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
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, w; cin >> n >> w;
    set<T> st;
    rep(i, n) {
        int x, y; cin >> x >> y; x--;
        st.emplace(y, x, i);
    }
    int q; cin >> q;
    map<int, vector<pii>> mp;
    rep(qi, q){
        int t, a; cin >> t >> a; a--;
        mp[t].emplace_back(a, qi);
    }
    vector<pair<int, vector<pii>>> query;
    for (auto [t, v]: mp){
        query.emplace_back(t, v);
    }
    sort(rng(query));
    vector<bool> ans(q);
    vector<set<pii>> res(w);
    int full = 0;
    set<int> st2;
    rep(i, n) st2.insert(i);
    for(auto [t, v]: query){
        while(!st.empty()){
            auto [y, x, j] = *st.begin();
            if (y > t) break;
            st.erase(st.begin());
            res[x].emplace(y, j);
            if (res[x].size() == 1) full++;
        }
        while(full == w){
            rep(i, w){
                auto [_, id] = *res[i].begin();
                st2.erase(id);
                res[i].erase(res[i].begin());
                if (res[i].size() == 0) full--;
            }
        }
        for (auto [a, qi]: v){
            ans[qi] = st2.count(a);
        }
    }
    rep(i, q) cout << (ans[i] ? "Yes" : "No") << endl;
    return 0;
}