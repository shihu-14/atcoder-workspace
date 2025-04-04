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
using T4 = tuple<double, double, double, double>;
// global

int n, m, L, W;
vector<pdd> p;
vector<int> Gi;
vector<T4> bounding_box;
vector<pdd> central;
map<pii, int> mp_edges;

const int split_num = 12;
const int tol_neighbor = 15;
int tol_central_dist;
const vector<int> div_size = {15, 10, 7, 5, 2};
const vector<int> coef = {4, 14, 1};
const vector<int> edge_coef = {0, -10, -50, -100, -220, -380, -500, -800, -1000, -2000, -3000, -5000};

vector<vector<pdd>> split_rect(int i){
    vector<vector<pdd>> res(split_num, vector<pdd>(split_num));
    auto [lx, rx, ly, ry] = bounding_box[i];
    double dx = (rx-lx)/split_num;
    double dy = (ry-ly)/split_num;
    rep(j, split_num)rep(k, split_num){
        res[j][k] = {lx + dx*(j+0.5), ly + dy*(k+0.5)};
    }
    return res;
}
vector<pii> krauskal(vector<int> vs){
    vector<pii> res;
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
        if (res.size() == sz-1) break;
    }
    return res;
}

double calc_diff_score(int si, vector<vector<int>> &g){
    double dist_score = 0, edge_score = 0, central_score = 0;
    for (auto v: g[si]){
        edge_score += edge_coef[mp_edges[{si, v}]];
        edge_score += edge_coef[mp_edges[{v, si}]];
        int d = sqrt((p[si].fi-p[v].fi)*(p[si].fi-p[v].fi)+(p[si].se-p[v].se)*(p[si].se-p[v].se));
        dist_score += d;
        double cd = sqrt((central[si].fi-central[v].fi)*(central[si].fi-central[v].fi)+(central[si].se-central[v].se)*(central[si].se-central[v].se));
        if (cd > W) continue;
        central_score += d-cd;
    }
    double res = dist_score*coef[0]+edge_score*coef[1]+central_score*coef[2];
    return res;
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = std::chrono::high_resolution_clock::now();
    int q; cin >> n >> m >> q >> L >> W;
    tol_central_dist = W;
    Gi.resize(m);
    rep(i, m) cin >> Gi[i];
    rep(i, n){
        double lx, rx, ly, ry; cin >> lx >> rx >> ly >> ry;
        bounding_box.emplace_back(lx, rx, ly, ry);
        p.emplace_back((lx+rx)/2.0, (ly+ry)/2.0);
        central.emplace_back((lx+rx)/2.0, (ly+ry)/2.0);
    }
    { // mp_edges 
        int qi = 0;
        for (auto div: div_size){
            if (qi == q) break;
            vector<int> id(n);
            iota(id.begin(), id.end(), 0);
            {
                vector<int> wid(n);
                int w = 10000/(div-1);
                rep(i, n) wid[i] = p[i].fi/w; 
                sort(id.begin(), id.end(), [&](int i, int j){ 
                    if (wid[i] != wid[j]) return wid[i] < wid[j]; 
                    if (wid[i]&1) return p[i].se > p[j].se; 
                    else return p[i].se < p[j].se; 
                });
            }
            
            int j = 0;
            while(qi < q){
                vector<int> vs;
                if (j+L >= n) break;
                rep(i, L) vs.emplace_back(id[j+i]);
                cout << "? " << L << " ";
                for (auto v: vs) cout << v << " "; cout << endl;
                rep(i, L-1){
                    int u, v; cin >> u >> v; 
                    mp_edges[{u, v}]++;
                }
                qi++;
                j += L;
            }
        }
    }
    vector<vector<vector<pdd>>> mp_grid;
    vector<vector<int>> neighbor(n);
    {
        rep(i, n){
            auto grid = split_rect(i);
            mp_grid.emplace_back(grid);
        }
        rep(i, n){
            vector<pii> dist_list;
            rep(j, n){
                if (i == j) continue;
                int d = (p[i].fi-p[j].fi)*(p[i].fi-p[j].fi)+(p[i].se-p[j].se)*(p[i].se-p[j].se);
                dist_list.emplace_back(d, j);
            }
            sort(rng(dist_list));
            rep(j, tol_neighbor){
                neighbor[i].emplace_back(dist_list[j].se);
            }
        }
    }
    vector<int> group_id(n);
    vector<vector<int>> g(n);
    {
        vector<int> id(n);
        {
            iota(rng(id), 0);
            int init_div_size = 10;
            vector<int> wid(n);
            int w = 10000/(init_div_size-1);
            rep(i, n) wid[i] = p[i].fi/w; 
            sort(id.begin(), id.end(), [&](int i, int j) { 
                if (wid[i] != wid[j]) return wid[i] < wid[j]; 
                if (wid[i]&1) return p[i].se > p[j].se; 
                else return p[i].se < p[j].se; 
            });
        }
        {   
            int k = 0;
            rep(i, m){
                rep(j, Gi[i]){
                    group_id[id[k++]] = i;
                }
            }
            // assert(k == n);
        }
        {
            vector<vector<int>> groups(m);
            rep(i, n){
                groups[group_id[i]].emplace_back(i);
            }
            rep(i, m){
                auto edges = krauskal(groups[i]);
                for (auto [u, v]: edges){
                    g[u].emplace_back(v);
                    g[v].emplace_back(u);
                }
            }
        }
    }
    double score = 0;
    {   
        rep(i, n) score += calc_diff_score(i, g);
    }

    double min_score = LINF;
    int times = 0, update = 0;
    random_device rd; mt19937 gen(rd()); 
    uniform_int_distribution<int> dist(0, n-1);
    int op = 1;
    while(1){
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (elapsed.count() > 1900) break;
        op ^= 1;
        times++;
        double tmp_score = score;
        int tmp_i1 = -1, tmp_i2 = -1;
        pii tmp_p;
        vector<vector<int>> tmp_g;
        if (op){
            uniform_int_distribution<int> dist2(0, tol_neighbor-1);
            int i1 = dist(gen);
            int i2 = neighbor[i1][dist2(gen)];
            if (group_id[i1] == group_id[i2]) continue;
            tmp_i1 = i1, tmp_i2 = i2;
            tmp_g = g;
            score -= calc_diff_score(i1, g);
            score -= calc_diff_score(i2, g);
            for (auto v: g[i1]){
                auto it = find(rng(g[v]), i1);
                // assert(it != g[v].end());
                g[v].erase(it);
                g[v].emplace_back(i2);
            }
            for (auto v: g[i2]){
                auto it = find(rng(g[v]), i2);
                // assert(it != g[v].end());
                g[v].erase(it);
                g[v].emplace_back(i1);
            }
            swap(g[i1], g[i2]);
            swap(group_id[i1], group_id[i2]);
            score += calc_diff_score(i1, g);
            score += calc_diff_score(i2, g);
        }
        else {
            uniform_int_distribution<int> dist2(0, split_num-1);
            int i = dist(gen); 
            tmp_i1 = i, tmp_p = p[i];
            score -= calc_diff_score(i, g);
            int x = dist2(gen), y = dist2(gen);
            p[i] = mp_grid[i][x][y];
            score += calc_diff_score(i, g);
        }
        if (score < min_score){
            min_score = score;
            update++;
            continue;
        }
        score = tmp_score;
        if (op){
            g = tmp_g;
            swap(group_id[tmp_i1], group_id[tmp_i2]);
        }
        else{
            p[tmp_i1] = tmp_p;
        }
    }
    vector<vector<int>> groups(m);
    rep(i, n){
        // assert(group_id[i] < m);
        groups[group_id[i]].emplace_back(i);
    }
    // { // debug info
    //     cerr << "times: " << times << endl;
    //     cerr << "update: " << update << endl;
    // }
    { // ans-output
        cout << "!" << endl;
        rep(i, m){
            for (int j: groups[i]) cout << j << " "; cout << endl;
            // assert(groups[i].size() == Gi[i]);
            vector<pii> edges;
            for (int j: groups[i]){
                for (auto k: g[j]){
                    if (j >= k) continue;
                    edges.emplace_back(j, k);
                }
            }
            // assert(edges.size() == Gi[i]-1);
            for (auto [u, v]: edges){
                cout << u << " " << v << endl;
            }
        }
    }
    return 0;
}