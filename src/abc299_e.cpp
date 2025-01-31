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
    int n, m; cin >> n >> m;
    G g(n);
    rep(i, m){
        int u, v; cin >> u >> v;
        u--; v--;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<vector<int>> dist(n, vector<int>(n, INF));
    rep(i, n) dist[i][i] = 0;
    rep(i, n){
        priority_queue<pii, vector<pii>, greater<pii>> pq;
        pq.emplace(0, i);
        while(!pq.empty()){
            auto [c, now] = pq.top(); pq.pop();
            if(dist[i][now] < c) continue;
            for(auto to : g[now]){
                if(dist[i][to] <= c+1) continue;
                dist[i][to] = c+1;
                pq.emplace(dist[i][to], to);
            }
        }
    }

    int k; cin >> k;
    vector<int> ans(n, 1);
    vector<pii> q;
    rep(i, k){
        int p, d; cin >> p >> d; p--;
        q.emplace_back(p, d);
        rep(j, n){
            if(dist[p][j] < d) ans[j] = 0;
        }
    }

    for(auto [p, d]: q){
        bool flag = true;
        rep(i, n){
            if (dist[p][i] < d && ans[i] != 0){
                cout << "No" << endl;
                return 0;
            }
            else if (dist[p][i] == d && ans[i] == 1){
                flag = false;
            }
        }
        if (flag) {
            cout << "No" << endl;
            return 0;
        }
    }

    cout << "Yes" << endl;
    rep(i, n) cout << ans[i];
    cout << endl;
    return 0;
}