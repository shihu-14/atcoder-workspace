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
using T6 = tuple<int, int, int, int, int, int>;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w; cin >> h >> w;
    vector<vector<int>> A(h, vector<int>(w));
    vector<vector<pii>> F(1000010);
    int Fmax = 0;
    rep(i, h)rep(j, w){
        cin >> A[i][j];
        F[A[i][j]].emplace_back(i, j);
        chmax(Fmax, A[i][j]);
    } 
    int q; cin >> q;
    vector<T6> query;
    rep(i, q){
        int a, b, y, c, d, z; cin >> a >> b >> y >> c >> d >> z;
        a--, b--, c--, d--;
        query.emplace_back(a, b, y, c, d, z);
    }
    vector<pii> query2;
    rep(i, q) query2.emplace_back(1, Fmax+1);
    vector<int> ans(q, -1);
    int t = 30;
    while(t--){
        vector<vector<int>> mid(Fmax+1);
        rep(i, q){
            auto [ac, wa] = query2[i];
            if (ans[i] != -1) continue;
            if (wa-ac == 1){
                auto [a, b, y, c, d, z] = query[i];
                if (y >= ac && z >= ac) ans[i] = abs(y-ac)+abs(z-ac);
                else ans[i] = abs(y-z);
            }
            else{
                int wj = (ac+wa)/2;
                mid[wj].emplace_back(i);
            }
        }
        dsu uf(h*w);
        vector<vector<bool>> used(h, vector<bool>(w));
        rrep2(f, Fmax+1, 1){
            for(auto [i, j]: F[f]){
                used[i][j] = true;
            }
            for (auto [i, j]: F[f]){
                rep(t, 4){
                    int ni = i+dx[t], nj = j+dy[t];
                    if (ni < 0 || ni >= h || nj < 0 || nj >= w) continue;
                    if (used[ni][nj] == false) continue;
                    uf.merge(i*w+j, ni*w+nj);
                }
            }
            for (int i: mid[f]){
                auto [a, b, y, c, d, z] = query[i];
                auto [ac, wa] = query2[i];
                if (uf.same(a*w+b, c*w+d)) ac = f;
                else wa = f;
                query2[i] = {ac, wa};
            }
        }
    }
    rep(i, q) cout  << ans[i] << endl;
    return 0;
}