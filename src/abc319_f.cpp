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
using T = tuple<ll, ll, ll>;
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
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<vector<T>> g(n);
    vector<pii> med;
    map<int, int> mp;
    rep2(i, 1, n){
        int p, t, s, v; cin >> p >> t >> s >> v; p--;
        g[p].emplace_back(i, s, v);
        if (t == 2){
            mp[i] = med.size();
            med.emplace_back(i, v);
        }
    }
    int m = med.size();
    // max, pq[s, t], set[med]
    vector<tuple<ll, priority_queue<T, vector<T>, greater<T>>, set<int>>> dp(1<<m);
    {
        auto [mx, pq, st] = dp[0];
        mx++;
        for (auto [u, s, val]: g[0]){
            if (s == 0) st.insert(mp[u]);
            else pq.emplace(s, val, u);
        }
        while(!pq.empty()){
            auto [s, val, v] = pq.top();
            if (s > mx) break;
            pq.pop();
            mx += val;
            if (mx > INF){
                cout << "Yes" << endl;
                return 0;
            }
            for (auto [u, s2, val2]: g[v]){
                if (s2 == 0) st.insert(mp[u]);
                else pq.emplace(s2, val2, u);
            }
        }
        dp[0] = {mx, pq, st};
    }
    rep(i, 1<<m){
        if (get<0>(dp[i]) == 0) continue;
        vector<int> tmp;
        for (auto j: get<2>(dp[i])) tmp.emplace_back(j);
        for (auto j: tmp){
            if (i>>j&1) continue;
            auto [mx, pq, st] = dp[i];
            auto [k, val] = med[j];
            if (mx > INF/val){
                cout << "Yes" << endl;
                return 0;
            }
            mx *= val;
            
            pq.emplace(0, 0, k);
            while(!pq.empty()){
                auto [s, val, v] = pq.top();
                if (s > mx) break;
                pq.pop();
                mx += val;
                if (mx > INF){
                    cout << "Yes" << endl;
                    return 0;
                }
                for (auto [u, s2, val2]: g[v]){
                    if (s2 == 0) st.insert(mp[u]);
                    else pq.emplace(s2, val2, u);
                }
            }
            if (get<0>(dp[i|1<<j]) < mx){
                dp[i|1<<j] = {mx, pq, st};
            }
        }
    }
    auto [mx, pq, _] = dp[(1<<m)-1];
    if (pq.size() || mx == 0){
        cout << "No" << endl;
    }    
    else cout << "Yes" << endl;

    return 0;
}