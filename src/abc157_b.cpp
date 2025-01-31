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
    vector<vector<int>> a(3, vector<int>(3)), used(3, vector<int>(3));
    rep(i, 3) rep(j, 3) cin >> a[i][j];
    int n; cin >> n;
    rep(i, n){
        int b; cin >> b;
        rep(j, 3)rep(k, 3) if (a[j][k] == b) used[j][k] = 1;
    }
    rep(i, 3){
        int tmp = 0;
        rep(j, 3){
            if (used[i][j]) tmp++;
        }
        if (tmp == 3){
            cout << "Yes" << endl;
            return 0;
        }
    }
    rep(i, 3){
        int tmp = 0;
        rep(j, 3){
            if (used[j][i]) tmp++;
        }
        if (tmp == 3){
            cout << "Yes" << endl;
            return 0;
        }
    }
    int tmp2 = 0, tmp3 = 0;
    rep(i, 3) if (used[i][i]) tmp2++;
    rep(i, 3) if (used[i][2-i]) tmp3++;
    if (tmp2 == 3 || tmp3 == 3){
        cout << "Yes" << endl;
        return 0;
    }
    cout << "No" << endl;
    return 0;
}