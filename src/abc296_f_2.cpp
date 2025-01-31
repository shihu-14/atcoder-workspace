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
    int n; cin >> n;
    vector<int> a(n), b(n);
    rep(i, n) cin >> a[i];
    rep(i, n) cin >> b[i];
    rep(i, n) a[i]--, b[i]--;
    vector<int> c1(n), c2(n);
    rep(i, n) c1[a[i]]++, c2[b[i]]++;
    bool ok = false, flag = false;
    rep(i, n){
        if (c1[i] != c2[i]){
            ok = true;
        }
        if (c1[i] > 1) flag = true;
    }
    if (ok){
        cout << "No" << endl;
        return 0;
    }
    else if (flag){
        cout << "Yes" << endl;
        return 0;
    }

    fenwick_tree<int> fw1(n), fw2(n);
    ll t1 = 0, t2 = 0;
    rep(i, n){
        t1 += fw1.sum(a[i]+1, n);
        t2 += fw2.sum(b[i]+1, n);
        fw1.add(a[i], 1);
        fw2.add(b[i], 1);
    }
    t1 %= 2;
    t2 %= 2;
    cout << ((t1==t2 ? "Yes" : "No")) << endl;

    return 0;
}