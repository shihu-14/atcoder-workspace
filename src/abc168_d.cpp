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
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[i==v[i].size()-1];cout<<endl;}
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
        int a, b; cin >> a >> b;
        a--; b--;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    vector<int> dist(n, INF), ans(n, -1);
    dist[0] = 0;
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.emplace(0, 0);
    while(!pq.empty()){
        auto [c, v] = pq.top(); pq.pop();
        if (dist[v] < c) continue;
        for(int nv : g[v]){
            if (dist[nv] > dist[v]+1){
                dist[nv] = dist[v]+1;
                ans[nv] = v;
                pq.emplace(dist[nv], nv);
            }
        }
    }
    cout << "Yes" << endl;
    rep2(i, 1, n) cout << ans[i]+1 << endl;
    return 0;
}