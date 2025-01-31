// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
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
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
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
    int h, w, q; cin >> h >> w >> q;
    vector<set<int>> row(h), col(w);
    rep(i, h)rep(j, w){
        row[i].insert(j);
        col[j].insert(i);
    }
    vector<vector<bool>> used(h, vector<bool>(w, false));
    rep(i, q){
        int r, c; cin >> r >> c; r--, c--;
        if (!used[r][c]){
            used[r][c] = true;
            row[r].erase(c);
            col[c].erase(r);
            continue;
        }
        vector<pii> tmp;
        auto itr = row[r].lower_bound(c);
        if (itr != row[r].end()){
            tmp.emplace_back(r, *itr);
        }
        if (itr != row[r].begin()){
            itr = prev(itr);
            tmp.emplace_back(r, *itr);
        }

        itr = col[c].lower_bound(r);
        if (itr != col[c].end()){
            tmp.emplace_back(*itr, c);
        }
        if (itr != col[c].begin()){
            itr = prev(itr);
            tmp.emplace_back(*itr, c);
        }
        for (auto [r, c]: tmp){
            used[r][c] = true;
            row[r].erase(c);
            col[c].erase(r);
        }

    }
    int ans = 0;
    rep(i, h) rep(j, w) if (!used[i][j]) ans++;
    cout << ans << endl;
    return 0;
}