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
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

// Convex Hull Trick (for max/min query)
struct CHT {
    struct line { // 一次関数を扱う構造体。
        ll a, b;
        line(ll a=0, ll b=0): a(a), b(b) {}
        ll operator()(ll x) const { return a*x+b;}
    };
    bool XisMonotone; // クエリに単調性があるかどうか。
    function<bool(ll a, ll b)> cmp; // 最大値クエリはgreaterを、最小値クエリはlessを指定すること。
    CHT(bool flag, function<bool(ll a, ll b)> cmp) : XisMonotone(flag), cmp(cmp) {}

    vector<line> ls;
    void add(ll a, ll b) { // 傾きa、切片bの直線を追加する。
        line l(a,b);
        assert(ls.size() == 0 || ls.back().a <= l.a); // 追加クエリの直線の傾きが単調であることが条件。
        while (ls.size() >= 2) {
            const line& l1 = ls[ls.size()-2];
            const line& l2 = ls[ls.size()-1];
            // この条件を満たすとき、削除不要。そうでなければ、新しい直線を追加することで末尾の直線が不要になる。
            if ((l.a-l2.a)*(l1.b-l2.b) < (l2.a-l1.a)*(l2.b-l.b)) break;
            ls.pop_back();
        }
        ls.emplace_back(l);
    }
    ll operator()(ll x) { // 与えられたクエリを処理する。
        if (XisMonotone){ // クエリに単調性がある場合は、クエリ全体をO(N)で処理できる。
            static int head = 0;
            while (head <= ls.size()-2 && !cmp(ls[head](x), ls[head+1](x))) { // クエリの先頭から二番目までを見てクエリを処理する。
                head++;
            }
            return ls[head](x);
        }
        else{ // クエリに単調性がない場合は、各クエリにO(logN)かけて処理をする。
            int wa = -1, ac = ls.size()-1;
            while (ac-wa > 1) { // 二分探索
                int wj = (wa+ac)/2;
                if (!cmp(ls[wj](x), ls[wj+1](x))) wa = wj;
                else ac = wj;
            }
            return ls[ac](x);
        }
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m; cin >> n >> m;
    vector<ll> b(n), ans(m);
    vector<pll> c(m);
    rep(i, n) cin >> b[i];
    rep(i, m) cin >> c[i].fi, c[i].se = i;
    sort(rrng(b));
    sort(rng(c));
    CHT cht(true, greater());
    rep2(i, 1, n+1){
        cht.add(i, i*b[i-1]);
    }
    rep(i, m){
        ans[c[i].se] = cht(c[i].fi);
    }
    printv(ans);
    return 0;
}