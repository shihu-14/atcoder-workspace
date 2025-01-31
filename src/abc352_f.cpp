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
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<vector<pii>> g(n);
    rep(i, m){
        int a, b, c; cin >> a >> b >> c;
        a--, b--;
        g[a].emplace_back(b, -c);
        g[b].emplace_back(a, +c);
    }
    vector<vector<int>> rank;
    vector<vector<pii>> who;
    vector<bool> used(n); 
    auto f = [&](auto f, int u, int s, vector<int> &id, vector<pii> &id2) -> void{
        used[u] = true;
        id.emplace_back(s);
        id2.emplace_back(s, u);
        for (auto [v, w]: g[u]){
            if (used[v]) continue;
            f(f, v, s+w, id, id2);
        }    
    };

    rep(i, n){
        if (used[i]) continue;
        rank.emplace_back(vector<int>());
        who.emplace_back(vector<pii>());
        f(f, i, 0, rank.back(), who.back());
    }

    int sz = rank.size();
    rep(i, sz){
        int mn = MIN(rank[i]);
        rep(j, rank[i].size()){
            rank[i][j] -= mn;
        }
        sort(rng(who[i]));
    }
   
    vector<int> blk(sz);
    rep(i, sz){
        for (auto u: rank[i]){
            blk[i] |= 1<<u;
        }
    }
    vector<int> ans(n, -1);
    rep(si, sz){
        vector<bool> dp(1<<n, false); dp[0] = true;
        int num = 0;
        rep(i, sz){
            if (i == si) continue;
            vector<bool> pre(1<<n, false); swap(dp, pre);
            rep(j, 1<<n) if (num == popcount(j)){
                if (!pre[j]) continue;
                for (int tmp = blk[i]; tmp < 1<<n; tmp <<= 1){
                    if ((j&tmp) == 0){
                        dp[j|tmp] = true;    
                    }
                }
            }
            num += rank[i].size();
        }
        int cnt = 0, shift = 0;
        rep(i, 1<<n){
            for (int j = 0, tmp = blk[si]; j < n && tmp < 1<<n; j++, tmp <<= 1){
                if (dp[i] && (i&tmp) == 0) shift = j, cnt++;
            }
        }
        if (cnt == 1){
            int tmp = blk[si];
            tmp <<= shift;
            int j = 0;
            rep(i, n){
                if (tmp>>i&1) ans[who[si][j].se] = i+1, j++;
            }
        }
    }
    rep(i, n) cout << ans[i] << " "; cout << endl;
    return 0;
}