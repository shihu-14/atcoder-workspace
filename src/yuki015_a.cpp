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
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]+1<<" \n"[i==v.size()-1];}
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
    int n; ll s; cin >> n >> s;
    vector<ll> p(n);
    rep(i, n) cin >> p[i];
    vector<ll> pl, pr;
    rep(i, n/2) pl.emplace_back(p[i]);
    rep2(i, n/2, n) pr.emplace_back(p[i]);
    int n1 = pl.size(), n2 = pr.size();
    vector<pll> suml, sumr;
    rep(i, 1<<n1){
        ll sum = 0;
        rep(j, n1){
            if(i>>j&1) sum += pl[j];
        }
        suml.emplace_back(sum, i);
    }
    rep(i, 1<<n2){
        ll sum = 0;
        rep(j, n2){
            if(i>>j&1) sum += pr[j];
        }
        sumr.emplace_back(sum, i);
    }
    sort(rng(suml));
    sort(rng(sumr));
    vector<vector<int>> ans;
    rep(i, 1<<n1){
        int it = lower_bound(rng(sumr), pll(s-suml[i].fi, -1)) - sumr.begin();
        if (it != (1<<n2) && sumr[it].fi == s-suml[i].fi){
            for(int j2 = it; sumr[j2].fi == s-suml[i].fi; j2++){
                vector<int> res;
                rep(j, n1) if (suml[i].se>>j&1) res.emplace_back(j);                    
                rep(j, n2) if (sumr[j2].se>>j&1) res.emplace_back(n1+j);
                ans.emplace_back(res);
            }
        }
    }

    sort(rng(ans));
    for(auto v: ans) printv(v);
    return 0;
}