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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, x, y; cin >> n >> x >> y; x--, y--;
    rep2(k, 1, n){
        int ans = 0;
        rep2(i, y, n){
            if (i+k < n) ans++;
        }
        rep2(i, 0, x+1){
            int d = k;
            d -= x-i;
            if (d <= 0){
                ans++;
                continue;
            }
            d--;
            if (d == 0){
                if (y <= i+k && i+k < n) ans++;
                else if (i+k < y) ans += 2;
                continue;
            }
            int j1 = x+d, j2 = y-d, j3 = y+d;
            if (j1 >= n || j3 >= n) continue;
            
            if (j1 == j2) ans += 2;
            else if (j1 < j2) ans += 3;
            else if (j2 < j1 && j1 < j3) ans += 1;
            else if (j2 == j1 && j1 < j3) ans += 2;
            else if (j2 < j1 && j1 == j3) ans += 1;
            else if (j3 <= j1) ans += 1;
        }
        rep2(i, x+1, y){
            int d = k;
            d -= i-x;
            if (d <= 0){
                if (i+k < n) ans++;
                continue;
            }
            d--;
            if (d == 0){
                if (y <= i+k && i+k < n) ans++;
                else if (i+k < y) ans += 2;
                continue;
            }
            int j1 = i+k, j2 = y-d, j3 = y+d;
            if (j1 >= n || j3 >= n) continue;
            if (j1 == j2) ans += 2;
            else if (j1 < j2) ans += 3;
            else if (j2 < j1 && j1 < j3) ans += 1;
            else if (j2 == j1 && j1 < j3) ans += 2;
            else if (j2 < j1 && j1 == j3) ans += 1;
            else if (j3 <= j1) ans += 1;
        }
        cout << ans << endl;
    }
    return 0;
}