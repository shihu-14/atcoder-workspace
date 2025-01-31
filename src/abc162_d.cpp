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
    int n; cin >> n;
    string s; cin >> s;
    vector<int> r(n+1), g(n+1), b(n+1);
    rep(i, n){
        if (s[i] == 'R') r[i+1]++;
        if (s[i] == 'G') g[i+1]++;
        if (s[i] == 'B') b[i+1]++;
    }
    rep(i, n){
        r[i+1] += r[i];
        g[i+1] += g[i];
        b[i+1] += b[i];
    }
    ll ans = 0;
    rep2(i, 2, n){
        rep2(j, 0, i-1){
            if (s[i] == 'R'){
                if (s[j] == 'G'){
                    ans += b[i]-b[j+1];
                    if ((i+j)%2 == 0 && s[(i+j)/2] == 'B') ans--;
                }
                if (s[j] == 'B'){
                    ans += g[i]-g[j+1];
                    if ((i+j)%2 == 0 && s[(i+j)/2] == 'G') ans--;
                }
                
            }
            else if (s[i] == 'G'){
                if (s[j] == 'R'){
                    ans += b[i]-b[j+1];
                    if ((i+j)%2 == 0 && s[(i+j)/2] == 'B') ans--;
                }
                if (s[j] == 'B'){
                    ans += r[i]-r[j+1];
                    if ((i+j)%2 == 0 && s[(i+j)/2] == 'R') ans--;
                }
            }
            else{
                if (s[j] == 'R'){
                    ans += g[i]-g[j+1];
                    if ((i+j)%2 == 0 && s[(i+j)/2] == 'G') ans--;
                }
                if (s[j] == 'G'){
                    ans += r[i]-r[j+1];
                    if ((i+j)%2 == 0 && s[(i+j)/2] == 'R') ans--;
                }
            }
        }
    }
    cout << ans << endl;
    return 0;
}