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
    int n; cin >> n;
    string s = to_string(n);
    int n1 = s.size();
    ll ans = 0;
    rep2(i, 1, n+1){
        // cout << (i-1) << " " << ans << endl;
        string t = to_string(i);
        int m = t.size();
        ll ten = 1;
        if (m >= 2 && t[0] == t[m-1]) ans++;
        rep2(j, min(2, m), n1){
            if (j == 0 || j == 1){
                ans += ten;
                continue;
            }
            ten *= 10;
            ans += ten;
        }
        if (t[m-1] == '0' || t[m-1] > s[0]) continue;
        if (n1 <= 2){
            reverse(rng(t));
            if (m == 1 && n1 != 1) t += t;
            int tmp = stoll(t);
            if (tmp <= n) ans++;
            continue;
        }
        if (t[m-1] < s[0]){
            ten *= 10;
            ans += ten;
        }        
        else if (t[0] <= s[n1-1]){
            string s2 = s.substr(1, n1-2);
            ans += stoll(s2)+1;
        }
        else if (t[0] > s[n1-1]){
            string s2 = s.substr(1, n1-2);
            ans += (stoll(s2) ? stoll(s2)-1 : 0);
        }
        
    }
    cout << ans << endl;
    return 0;
}