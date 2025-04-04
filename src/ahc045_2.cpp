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


void pull_gravity(vector<pii> &edges){
    int m = edges.size();
    G g(n);
    rep(i, m){
        auto [i1, i2] = edges[i];
        auto [x1, y1] = p[i1];
        auto [x2, y2] = p[i2];
        double d12 = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
        p[i1].fi += (x2-x1)/d12;
        p[i1].se += (y2-y1)/d12;
        p[i2].fi += (x1-x2)/d12;
        p[i2].se += (y1-y2)/d12;
        g[i1].emplace_back(i2);
        g[i2].emplace_back(i1);
    }
    auto dfs = [&](auto f, int u, int p1=-1, int p2=-1) -> void{
        if (p1 != -1 && p2 != -1){
            double d12 = (p[p1].fi-p[p2].fi)*(p[p1].fi-p[p2].fi) + (p[p1].se-p[p2].se)*(p[p1].se-p[p2].se);
            double d1u = (p[p1].fi-p[u].fi)*(p[p1].fi-p[u].fi) + (p[p1].se-p[u].se)*(p[p1].se-p[u].se);
            double d2u = (p[p2].fi-p[u].fi)*(p[p2].fi-p[u].fi) + (p[p2].se-p[u].se)*(p[p2].se-p[u].se);
            if (d2u < d12 && d2u < d1u){
                d2u = sqrt(d2u);
                p[u].fi -= (p[p2].fi-p[u].fi)/d2u;
                p[u].se -= (p[p2].se-p[u].se)/d2u;
                p[p2].fi -= (p[u].fi-p[p2].fi)/d2u;
                p[p2].se -= (p[u].se-p[p2].se)/d2u;
            }           
        }
        for (int v: g[u]){
            if (v == p1 || v == p2) continue;
            f(f, v, u, p1);
        }
    };
    dfs(dfs, edges[0].fi);
    rep(i, n){
        auto [lx, ly, rx, ry] = bounding_box[i];
        chmin(p[i].fi, (double)rx); chmax(p[i].fi, (double)lx);
        chmin(p[i].se, (double)ry); chmax(p[i].se, (double)ly);
    }
}

vector<int> k_means(){
    vector<int> group_id(n), group_size(m);
    vector<double> central_x(m), central_y(m);
    { // init
        random_device rd; mt19937 gen(rd()); 
        uniform_int_distribution<int> dist(0, 10000);
        rep(i, m){
            central_x[i] = dist(gen);
            central_y[i] = dist(gen);
        }
    }
    int max_iter = 2000, iter = 0;
    while(iter++ < max_iter){
        bool flag = true;
        rep(i, n){
            auto [x, y] = p[i];
            double min_dist = LINF; 
            int min_id = -1;
            rep(j, m){
                double d = (x-central_x[j])*(x-central_x[j]) + (y-central_y[j])*(y-central_y[j]);
                if (d < min_dist){
                    min_dist = d;
                    min_id = j;
                }
            }
            assert(min_id != -1);
            if (min_id != group_id[i]) flag = false;
            group_id[i] = min_id;
        }
        if (flag) break;
        vector<double> new_central_x(m), new_central_y(m);
        vector<int> new_group_size(m);
        rep(i, n){
            auto [x, y] = p[i];
            new_central_x[group_id[i]] += x;
            new_central_y[group_id[i]] += y;
            new_group_size[group_id[i]] += 1.0;
        }
        rep(i, m){
            if (new_group_size[i] == 0){
                continue;
            }
            new_central_x[i] /= new_group_size[i];
            new_central_y[i] /= new_group_size[i];
            central_x[i] = new_central_x[i];
            central_y[i] = new_central_y[i];
            group_size[i] = new_group_size[i];
        }
    }
    return group_id;
}
void arrange(vector<int> &group_id){
    vector<vector<int>> groups(m);
    rep(i, n) groups[group_id[i]].emplace_back(i);
    vector<int> bigger, smaller;
    rep(i, m){
        if (groups[i].size() > Gi[i]) bigger.emplace_back(i);
        else if (groups[i].size() < Gi[i]) smaller.emplace_back(i);
    }
    int r = 0;
    rep(i, bigger.size()){
        int bi = bigger[i];
        while(groups[bi].size() > Gi[bi]){
            int v = groups[bi].back();
            groups[bi].pop_back();
            int si = smaller[r];
            groups[si].emplace_back(v);
            if (groups[si].size() == Gi[si]) r++;
        }
        if (r == smaller.size()) break;
    }
    rep(i, m){
        for (int j: groups[i]) group_id[j] = i;
    }
}

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
const int A = 10001;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = std::chrono::high_resolution_clock::now();
    int q; cin >> n >> m >> q >> L >> W;
    Gi.resize(m);
    bounding_box.resize(n);
    p.resize(n);
    rep(i, m) cin >> Gi[i];
    rep(i, n){
        double lx, rx, ly, ry; cin >> lx >> rx >> ly >> ry;
        bounding_box[i] = {lx, rx, ly, ry};
        p[i] = {(lx+rx)/2.0, (ly+ry)/2.0};
    }
    random_device rd; mt19937 gen(rd()); 
    int map_id = 0;
    rep(qi, q){
        vector<int> vs;
        {
            int sect = 3;
            vector<vector<int>> map(sect*sect);
            {
                rep(i, n){
                    int x, y;
                    tie(x, y) = p[i];
                    int h = x*sect/A, w = y*sect/A;
                    assert(h < sect && w < sect);
                    map[h*sect+w].emplace_back(i);
                }
            }
            while(map[map_id].size() < 2) map_id = (map_id+1)%(sect*sect);
            vector<bool> used(n);
            uniform_int_distribution<int> dist(0, map[map_id].size()-1);
            while(vs.size() < min(L, (int)map[map_id].size()/2)){
                int j = map[map_id][dist(gen)];
                if(used[j]) continue;
                used[j] = true;
                vs.emplace_back(j);
            }
            map_id = (map_id+1)%(sect*sect);
        }
        cout << "? " << L << " ";
        for (int j: vs) cout << j << " ";
        cout << endl;
        vector<pii> edges;
        rep(i, L-1){
            int u, v; cin >> u >> v;
            edges.emplace_back(u, v);
        }
        pull_gravity(edges);
    }

    double score = 0;
    vector<double> sum_dist(m);
    vector<vector<int>> groups(m);
    vector<int> group_id(n);
    {
        group_id  = k_means();
        arrange(group_id);
        rep(i, n) groups[group_id[i]].emplace_back(i);
        rep(i, m) krauskal(groups[i], sum_dist[i]);
        rep(i, m) score += sum_dist[i];
    }

    uniform_int_distribution<int> dist(0, n-1);
    vector<int> final_group_id(n);
    double final_score = LINF;
    int times = 0, update = 0;
    // cerr << "first-score: " << score << endl;
    while(1){
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (elapsed.count() > 1900) break;
        times++;
        int i1 = dist(gen), i2 = dist(gen);
        if (group_id[i1] == group_id[i2]) continue;
        auto it = find(rng(groups[group_id[i1]]), i1);
        if (it != groups[group_id[i1]].end()) groups[group_id[i1]].erase(it);
        it = find(rng(groups[group_id[i2]]), i2);
        if (it != groups[group_id[i2]].end()) groups[group_id[i2]].erase(it);
        groups[group_id[i1]].emplace_back(i2);
        groups[group_id[i2]].emplace_back(i1);
        double tmp_sum1 = sum_dist[group_id[i1]];
        double tmp_sum2 = sum_dist[group_id[i2]];
        score -= tmp_sum1+tmp_sum2;
        krauskal(groups[group_id[i1]], sum_dist[group_id[i1]]);
        krauskal(groups[group_id[i2]], sum_dist[group_id[i2]]);
        score += sum_dist[group_id[i1]];
        score += sum_dist[group_id[i2]];
        if (score < final_score){
            swap(group_id[i1], group_id[i2]);
            final_score = score;
            final_group_id = group_id;
            update++;
            continue;
        }
        score -= sum_dist[group_id[i1]];
        score -= sum_dist[group_id[i2]];
        score += tmp_sum1+tmp_sum2;
        sum_dist[group_id[i1]] = tmp_sum1;
        sum_dist[group_id[i2]] = tmp_sum2;
        groups[group_id[i1]].pop_back();
        groups[group_id[i2]].pop_back();
        groups[group_id[i1]].emplace_back(i1);
        groups[group_id[i2]].emplace_back(i2);
    }

    // { // debug
    //     cerr << "score: " << final_score << endl;
    //     cerr << "times: " << times << endl;
    //     cerr << "update: " << update << endl;
    //     auto end = std::chrono::high_resolution_clock::now();
    //     auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    //     cerr << "elapsed: " << elapsed.count() << endl;
    // }
    { // ans-output
        cout << "!" << endl;
        rep(i, m){
            double dummy = 0;
            auto ans = krauskal(groups[i], dummy);
            for (int j: groups[i]) cout << j << " "; cout << endl;
            for (auto [u, v]: ans) cout << u << " " << v << endl;
        }
    }
    return 0;
}