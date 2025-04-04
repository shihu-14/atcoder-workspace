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
vector<T3> p2;
vector<int> Gi;
vector<T4> bounding_box;
const int split_num = 10;
// const int tol_dist = 10;
const int tol_area = 200;
const int div_size = 15;
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

int calc_krauskal(vector<int> &vs, set<pii> &ans){
    int sz = vs.size();
    assert(sz >= 2);
    dsu uf(vs.size());
    vector<tuple<double, int, int>> edges;
    rep(i, sz)rep2(j, i+1, sz){
        double w2 = (p[vs[i]].fi-p[vs[j]].fi)*(p[vs[i]].fi-p[vs[j]].fi) + (p[vs[i]].se-p[vs[j]].se)*(p[vs[i]].se-p[vs[j]].se);
        edges.emplace_back(w2, i, j);
    }
    int res = sz-1, cnt = 0;
    sort(rng(edges));
    for (auto [w2, i, j]: edges){
        if (uf.same(i, j)) continue;
        cnt++;
        uf.merge(i, j);
        if (vs[i] > vs[j]) swap(i, j);
        if (ans.count({vs[i], vs[j]})) res--;
        if (cnt == sz-1) break;
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

vector<int> decide_base_point(int si, vector<bool> &used){
    vector<pii> min_dist;
    rep(i, n){
        if (used[i]){
            int d = (p[si].fi-p[i].fi)*(p[si].fi-p[i].fi) + (p[si].se-p[i].se)*(p[si].se-p[i].se);
            min_dist.emplace_back(d, i);
        }
    }
    sort(rng(min_dist));
    vector<int> res; 
    rep(i, min((int)min_dist.size(), L)) res.push_back(min_dist[i].se); 
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = std::chrono::high_resolution_clock::now();
    int q; cin >> n >> m >> q >> L >> W;
    Gi.resize(m);
    bounding_box.resize(n);
    p.resize(n);
    rep(i, m) cin >> Gi[i];
    vector<pii> pa;
    map<int, int> id2area;
    rep(i, n){
        double lx, rx, ly, ry; cin >> lx >> rx >> ly >> ry;
        bounding_box[i] = {lx, rx, ly, ry};
        int area = (rx-lx+1)*(ry-ly+1);
        pa.emplace_back(area, i);
        id2area[i] = area;
        p[i] = {(lx+rx)/2.0, (ly+ry)/2.0};
    }
    vector<bool> used(n);
    vector<int> not_used;
    { // decide two 
        sort(rng(pa));
        for (auto [area, id]: pa){
            if (area > tol_area) break;
            used[id] = true;
        }
        int ri = pa[0].se;
        vector<pii> dist_list;
        {
            rep(i, n){
                if (used[i]) continue; 
                int d = (p[ri].fi-p[i].fi)*(p[ri].fi-p[i].fi)+(p[ri].se-p[i].se)*(p[ri].se-p[i].se);
                dist_list.emplace_back(d, i);
            }
            sort(rng(dist_list));
        }
        for (auto [_, id]: dist_list){
            not_used.emplace_back(id);
        }
        reverse(rng(not_used));
    }
    // cerr << not_used.size() << endl;
    // cerr << "enter-query" << endl;    
    { // Q * (N + L + 100 + 100*100*L*L)
        int qi = 0;
        while(qi++ < q && not_used.size()){
            int roi = not_used.back(); not_used.pop_back(); 
            auto vs = decide_base_point(roi, used);
            int query_size = min((int)vs.size(), L);
            cout << "? " << query_size << " ";
            rep(i, query_size) cout << vs[i] << " "; cout << endl;
            set<pii> edges;
            rep(i, query_size-1){
                int u, v; cin >> u >> v;
                edges.emplace(u, v);
            }
            auto grid = split_rect(roi);
            int min_error = INF, min_i = -1, min_j = -1;
            // cerr << "enter-d-potision" << endl;    
            // auto s3 = std::chrono::high_resolution_clock::now();
            rep(i, split_num){
                rep(j, split_num){045
                    auto [nx, ny] = grid[i][j];
                    p[roi] = {nx, ny};
                    int error = calc_krauskal(vs, edges);
                    if (error < min_error){
                        min_error = error;
                        min_i = i;
                        min_j = j;
                    }
                    if (error == 0) break;
                }
                if (min_error == 0) break;
            }
            assert(min_i != -1 && min_j != -1);
            p[roi] = grid[min_i][min_j];
            used[roi] = true;
        }
    }
    // cerr << "enter-grouping" << endl;
    vector<int> id(n);
    iota(id.begin(), id.end(), 0);
    {
        vector<int> wid(n);
        int w = 10000/(div_size-1);
        rep(i, n) wid[i] = p[i].fi/w; 
        sort(id.begin(), id.end(), [&](int i, int j) { 
            if (wid[i] != wid[j]) return wid[i] < wid[j]; 
            if (wid[i]&1) return p[i].se > p[j].se; 
            else return p[i].se < p[j].se; 
        });
    }
    vector<int> group_id(n);
    vector<vector<int>> groups(m);
    {   
        int k = 0;
        rep(i, m){
            rep(j, Gi[i]){
                group_id[id[k++]] = i;
            }
        }
        rep(i, n){
            assert(group_id[i] < m);
            groups[group_id[i]].emplace_back(i);
        }
    }
    // cerr << "enter--output" << endl;
    { // ans-output
        cout << "!" << endl;
        rep(i, m){
            auto ans = krauskal(groups[i]);
            for (int j: groups[i]) cout << j << " "; cout << endl;
            for (auto [u, v]: ans){
                cout << u << " " << v << endl;
            }
        }
    }
    return 0;
}