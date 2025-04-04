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
using pdd = pair<double, double>;
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
using T4 = tuple<int, int, int, int>;
// global

int n, m, L, W;
vector<pdd> p;
vector<int> Gi;
vector<T4> bounding_box;

//

vector<pii> krauskal(vector<int> vs, double &sum_w){
    vector<pii> res;
    sum_w = 0;
    int sz = vs.size();
    if (sz <= 1) return res;
    dsu uf(vs.size());
    vector<tuple<double, int, int>> edges;
    rep(i, sz)rep2(j, i+1, sz){
        double w2 = (p[vs[i]].fi-p[vs[j]].fi)*(p[vs[i]].fi-p[vs[j]].fi) + (p[vs[i]].se-p[vs[j]].se)*(p[vs[i]].se-p[vs[j]].se);
        edges.emplace_back(w2, i, j);
    }
    sort(rng(edges));
    for (auto [w2, i, j]: edges){
        if (uf.same(i, j)) continue;
        uf.merge(i, j);
        if (vs[i] > vs[j]) swap(i, j);
        res.emplace_back(vs[i], vs[j]);
        sum_w += sqrt(w2);
        if (res.size() == sz-1) break;
    }
    return res;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = std::chrono::high_resolution_clock::now();
    int q; cin >> n >> m >> q >> L >> W; L = 3;
    Gi.resize(m);
    bounding_box.resize(n);
    p.resize(n);
    rep(i, m) cin >> Gi[i];
    rep(i, n){
        double lx, rx, ly, ry; cin >> lx >> rx >> ly >> ry;
        bounding_box[i] = {lx, rx, ly, ry};
        p[i] = {(lx+rx)/2.0, (ly+ry)/2.0};
    }
    vector<int> widx(n), widy(n);
    int div_size = 150;
    {
        int w = 10000/(div_size-1);
        rep(i, n) widx[i] = p[i].fi/w; 
        rep(i, n) widy[i] = p[i].se/w; 
    }
    vector<vector<vector<int>>> div_groups(div_size, vector<vector<int>>(div_size));
    rep(i, n){
        div_groups[widx[i]][widy[i]].emplace_back(i);
    }
    random_device rd; mt19937 gen(rd()); 
    uniform_int_distribution<int> dist(0, div_size-1);
    bool flag = false;
    rep(qi, q){
        uniform_int_distribution<int> dist_dir(0, 3);
        int ri, rj, dir, sz;
        while(1){
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            if (elapsed.count() > 1900){
                flag = true;
                break;
            }
            ri = dist(gen), rj = dist(gen), dir = dist_dir(gen);
            if (div_groups[ri][rj].empty()) continue;
            int cnt = 1;
            int i = ri, j = rj;
            while(cnt < L){
                int ni = i+dx[dir], nj = j+dy[dir];
                if (ni < 0 || ni >= div_size || nj < 0 || nj >= div_size || div_groups[ni][nj].empty()) break;
                i = ni, j = nj;
                cnt++;
            }
            sz = cnt;
            if (cnt < 3) continue;
            break;
        }
        if (flag) break;
        vector<int> vs;
        vector<pii> div_id;
        {
            int i = ri, j = rj;
            rep(t, sz){
                uniform_int_distribution<int> dist1(0, div_groups[i][j].size()-1);
                vs.emplace_back(div_groups[i][j][dist1(gen)]);
                div_id.emplace_back(i, j);
                i += dx[dir]; j += dy[dir];
            }
            assert(vs.size() == sz);
        }
        assert(2 <= sz && sz <= L);
        cout << "? " << sz << " ";
        rep(i, sz) cout << vs[i] << " ";
        cout << endl;
        vector<vector<int>> g(n);
        rep(i, sz-1){
            int u, v; cin >> u >> v;
            g[u].emplace_back(v);
            g[v].emplace_back(u);
        }

        vector<int> depth(n);
        auto dfs = [&](auto f, int u, int p=-1, int d=0) -> void{
            depth[u] = d;
            for (auto v: g[u]) if (v != p){
                f(f, v, u, d+1);
            }
        };
        dfs(dfs, vs[0]);
        rep(t, sz){
            int u = vs[t], d = depth[u];
            auto [i, j] = div_id[t];
            int ni = ri + dx[dir]*d, nj = rj + dy[dir]*d;
            div_groups[i][j].erase(find(rng(div_groups[i][j]), u));
            div_groups[ni][nj].emplace_back(u);
            widx[u] = ni;
            widy[u] = nj;
        }
    }
    // cerr << "enter-output" << endl;

    vector<int> id(n);
    iota(id.begin(), id.end(), 0);
    {
        int resize = 12;
        int div = (div_size+resize-1)/resize;
        rep(i, n) widx[i] /= div;
    }
    sort(rng(id), [&](int i, int j){
        if (widx[i] != widx[j]) return widx[i] < widx[j];
        if (widx[i]&1) return widy[i] > widy[j];
        else return widy[i] < widy[j];
    });
    vector<int> group_id(n);
    {   
        int k = 0;
        rep(i, m){
            rep(j, Gi[i]){
                group_id[id[k++]] = i;
            }
        }
    }
    vector<vector<int>> groups(m);
    rep(i, n){
        assert(group_id[i] < m);
        groups[group_id[i]].emplace_back(i);
    }
    { // ans-output
        cout << "!" << endl;
        rep(i, m){
            assert(groups[i].size() == Gi[i]);
            double dummy = 0;
            auto ans = krauskal(groups[i], dummy);
            assert(ans.size() == Gi[i]-1);
            for (int j: groups[i]) cout << j << " "; cout << endl;
            for (auto [u, v]: ans){
                assert(find(rng(groups[i]), u) != groups[i].end() && find(rng(groups[i]), v) != groups[i].end());
                cout << u << " " << v << endl;
            }
        }
    }
    return 0;
}