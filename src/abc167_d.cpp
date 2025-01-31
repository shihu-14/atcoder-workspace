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
    int n; ll k; cin >> n >> k;
    vector<int> a(n);
    rep(i,n) cin >> a[i];
    rep(i, n) a[i]--;

    vector<int> used(n, -1), root;
    int ri, rc = 0, rs, rd, now = 0; 
    used[now] = rc++;
    root.emplace_back(now);
    while(1){
        int to = a[now];
        if (used[to] != -1){
            ri = used[to];
            rs = rc - ri;
            rd = to;
            break;
        }
        used[to] = rc++;
        root.emplace_back(to);
        now = to;
    }
    if (k < rc){
        cout << root[k]+1 << endl;
        return 0;
    }

    k -= rc;
    k %= rs;
    cout << root[ri+k]+1 << endl;
    return 0;
}