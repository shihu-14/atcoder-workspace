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
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    string s; cin >> s;
    vector<int> c1(n+1), c2(n+1), sla;
    rep(i, n){
        if (s[i] == '1') c1[i+1] = 1;
        else if (s[i] == '2') c2[i+1] = 1;
        else sla.emplace_back(i+1);
    }
    rep(i, n) c1[i+1] += c1[i];
    rep(i, n) c2[i+1] += c2[i];


    rep(qi, q){
        int L, R; cin >> L >> R;
        {
            int l = lower_bound(rng(sla), L) - sla.begin();
            int r = upper_bound(rng(sla), R) - sla.begin();
            r--;
            if (l > r){
                cout << 0 << endl;
                continue;
            }
        }

        auto f = [&](int w) -> bool{
            int l = lower_bound(rng(c1), c1[L-1]+w) - c1.begin();
            // if (qi == 0 && w == 2) cout << "debug::  " << l << endl;
            if (l > R) return false;
            int m = lower_bound(rng(sla), l+1) - sla.begin();
            if (m == sla.size()) return false;
            m = sla[m];
            // if (qi == 0 && w == 2) cout << "debug::  " << m << endl;

            if (m > R) return false;
            int r = lower_bound(rng(c2), c2[m]+w) - c2.begin();
            // if (qi == 0 && w == 2) cout << "debug::  " << r << endl;
            return r <= R;
        };

        int ac = 0, wa = INF;
        while(wa-ac  > 1){
            int wj = (wa+ac)/2;
            if (f(wj)) ac = wj;
            else wa = wj;
        }
        cout << 2*ac+1 << endl;
    }
    cout.flush();
    return 0;
}