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

ll sum(ll a, ll d, ll n){
    if (n != 0 && 2*a+(n-1)*d > LINF/n) return -1;
    return (2*a+(n-1)*d)*n/2;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; ll h; cin >> n >> h;
    map<ll, ll> mp;
    rep(i, n){
        ll t, d; cin >> t >> d;
        chmax(mp[t], d);
    } 
    vector<pll> p, st1, st2;
    for (auto [t, d]: mp) p.emplace_back(t, d);
    sort(rng(p));
    p.emplace_back(LINF, 0);
    {
        set<pll> tmp_st1 = {{0, 0}}, tmp_st2;
        for (auto [t, d]: p) tmp_st2.emplace(d, t);
        rep(i, p.size()){
            auto [t, d] = p[i];
            auto [td1, w] = *tmp_st1.rbegin();
            auto [d2, t2] = *tmp_st2.rbegin();
            st1.emplace_back(td1, w);
            st2.emplace_back(d2, t2);
            tmp_st1.emplace(t*d, t);
            tmp_st2.erase({d, t});
        }
    }
    ll wa = 0, ac = h;
    while(ac-wa>1){
        auto f = [&](ll T) -> bool{
            T++;
            ll res = h, now = 1; 
            rep(i, p.size()){
                auto [t, d] = p[i];
                chmin(t, T);
                auto [td1, w] = st1[i];
                auto [d2, t2] = st2[i];
                if (d2 != 0) w = (td1+d2-1)/d2;
                else w = LINF;
                chmax(w, now);
                chmin(w, t);
                if (td1 != 0 && w-now > LINF/td1) return true;
                res -= (w-now)*td1;
                now = w;
                if (T <= now || res <= 0) break;
                ll s = sum(d2*now, d2, t-now);
                if (s == -1) return true;
                res -= s;
                now = t;
                if (T <= now || res <= 0) break;
            }
            return res <= 0;
        };
        ll wj = (ac+wa)/2;
        if (f(wj)) ac = wj;
        else wa = wj;
    }
    cout << ac << endl;
    return 0;
}