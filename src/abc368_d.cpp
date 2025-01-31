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
    int n, k; cin >> n >> k;
    vector<int> deg(n), v(n);
    G g(n);
    rep(i, n-1){
        int a, b; cin >> a >> b;
        --a; --b;
        deg[a]++;
        deg[b]++;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    rep(i, k){
        int vv; cin >> vv; vv--;
        v[vv] = 1;
    }

    set<int> st;
    int ans = 0;
    rep(i, n) if (deg[i] == 1) st.insert(i);
    while(!st.empty()){
        bool flag = true;
        vector<int> tmp;
        for(auto u: st){
            if (v[u]) continue;
            flag = false;
            tmp.push_back(u);
        }
        for(auto u: tmp){
            ans++;
            st.erase(u);
            for(int to: g[u]){
                deg[to]--;
                if (deg[to] == 1) st.insert(to);
            }
        }
        if (flag) break;

    }
    cout << n-ans << endl;
    return 0;
}