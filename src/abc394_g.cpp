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
using T7 = tuple<int, int, int, int, int, int, int>;
using T8 = tuple<int, int, int, int, int, int, int, int>;


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w; cin >> h >> w;
    vector<vector<int>> A(h, vector<int>(w));
    vector<vector<pii>> F(1000010);
    rep(i, h)rep(j, w){
        int f; cin >> f;
        A[i][j] = f;
        F[f].emplace_back(i, j);
    } 
    int q; cin >> q;
    vector<T7> query;
    rep(i, q){
        int a, b, y, c, d, z; cin >> a >> b >> y >> c >> d >> z;
        a--, b--, c--, d--;
        if (A[a][b] < A[c][d]){
            swap(a, c);
            swap(b, d);
            swap(y, z);
        }
        query.emplace_back(a, b, y, c, d, z, i);
    }
    sort(rng(query), [&](T7 a, T7 b){
        return A[get<0>(a)][get<1>(a)] > A[get<0>(b)][get<1>(b)];
    });

    dsu uf(h*w);
    vector<vector<bool>> used(h, vector<bool>(w));
    set<T8> st;
    vector<int> ans(q);
    int counter = 0;
    rrep2(f, 1000001, 1){
        if (F[f].empty()) continue;
        for (auto [x, y] : F[f]){
            used[x][y] = true;
        }
        for (auto [x, y]: F[f]){
            rep(t, 4){
                int nx = x+dx[t], ny = y+dy[t];
                if (nx < 0 || nx >= h || ny < 0 || ny >= w) continue;
                if (used[nx][ny] == false) continue;
                uf.merge(x*w+y, nx*w+ny);
            }
        }   
        while(counter < q){
            auto [a, b, y, c, d, z, i] = query[counter];
            if (A[a][b] < f) break;
            st.emplace(-A[c][d], c, d, z, a, b, y, i);
            counter++;
        }
        vector<T8> tmp;
        for (auto [val, c, d, z, a, b, y, i]: st){
            if (A[c][d] < f) break;
            if (uf.same(c*w+d, a*w+b) == false) continue;
            if (y >= f && z >= f) ans[i] = abs(y-f)+abs(z-f);
            else ans[i] = abs(y-z);
            tmp.emplace_back(val, c, d, z, a, b, y, i);
        }
        rep(i, tmp.size()) st.erase(tmp[i]);
    }
    rep(i, q) cout << ans[i] << endl;
    return 0;
}