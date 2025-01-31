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
    int T; cin >> T;
    rep(t, T){
        int n, k; cin >> n >> k;
        vector<int> a(n), b(n);
        rep(i, n) cin >> a[i];
        rep(i, n) cin >> b[i];
        rep(i, n) a[i]--, b[i]--;
        set<int> st;
        vector<int> c(n);
        rep2(i, 1, k+1){
            c[a[i]]++;
            st.insert(a[i]);
        }
        if (a[0] != b[0] && !st.count(b[0])){
            cout << "No" << endl;
            continue;
        }
        c[a[0]]++;
        st.insert(a[0]);
        bool flag = true;
        rep2(i, 1, n){
            if (i > k){
                c[a[i-k-1]]--;
                if (c[a[i-k-1]] == 0) st.erase(a[i-k-1]);
            }
            c[a[i]]--;
            if (c[a[i]] == 0) st.erase(a[i]);
            if(i+k < n){
                c[a[i+k]]++;
                st.insert(a[i+k]);
            }
            if (a[i] != b[i] && !st.count(b[i])){
                cout << i << endl;
                printv(c);
                cout << "No" << endl;
                flag = false;
                break;
            }
            c[a[i]]++;
            st.insert(a[i]);
        }
        if (flag) cout << "Yes" << endl;
    }
    return 0;
}