// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    string s; cin >> s;
    int q; cin >> q;
    vector<fenwick_tree<int>> cnt(26, fenwick_tree<int>(n));
    fenwick_tree<int> fw(n);
    rep(i, n) cnt[s[i]-'a'].add(i, 1);
    rep(i, n-1){
        if (s[i] > s[i+1]) fw.add(i, 1);
    }
    rep(qi, q){
        int type; cin >> type;
        if (type == 1){
            int x; char c; cin >> x >> c; x--;
            cnt[s[x]-'a'].add(x, -1);
            s[x] = c;
            cnt[s[x]-'a'].add(x, 1);
            if (x){
                int v = s[x-1] > s[x];
                fw.add(x-1, -fw.sum(x-1, x)+v);
            }
            if (x+1 < n){
                int v = s[x] > s[x+1];
                fw.add(x, -fw.sum(x, x+1)+v);
            }
        }
        else {
            int l, r; cin >> l >> r; l--;
            int Lc = s[l]-'a', Rc = s[r-1]-'a';
            bool flag = true;
            for (int c = Lc+1; c < Rc; c++){
                if (cnt[c].sum(0, l)+cnt[c].sum(r, n) > 0){
                    flag = false;
                    break;
                }
            }
            if (fw.sum(l, r-1) != 0) flag = false;
            if (flag) cout << "Yes" << endl;
            else cout << "No" << endl;
        }
    }
    return 0;
}