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
    vector<int> widx(n), widy(n);
    int div_size = 15;
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
    uniform_int_distribution<int> dist(0, div_size-2);
    rep(qi, q){
        int i = dist(gen), j = dist(gen);
        if (div_groups[i][j].empty() || div_groups[i][j+1].empty() || div_groups[i+1][j].empty()) continue;
        uniform_int_distribution<int> dist1(0, div_groups[i][j].size()-1);
        uniform_int_distribution<int> dist2(0, div_groups[i][j+1].size()-1);
        uniform_int_distribution<int> dist3(0, div_groups[i+1][j].size()-1);
        int a = div_groups[i][j][dist1(gen)], b = div_groups[i][j+1][dist2(gen)], c = div_groups[i+1][j][dist3(gen)];
        cout << "? " << 3 << " " << a << " " << b << " " << c << endl;
        auto f = [&](int u, int v){
            if ((u == b && v == c ) || (v == b && u == c)) return false;
            return true;
        };
        int u1, v1, u2, v2; cin >> u1 >> v1 >> u2 >> v2;
        if (f(u1, v1) || f(u2, v2)) continue;
        auto change = [&](int v, int f){
            div_groups[widx[v]][widy[v]].erase(find(rng(div_groups[widx[v]][widy[v]]), v));
            if (f) widy[v] = j+1;
            else widx[v] = i+1;
            div_groups[widx[v]][widy[v]].emplace_back(v);
        };
        if (f(u1, v1)){
            if (u1 == c) swap(u1, v1);
            if (u1 == u2 || u1 == v2){
                change(v1, 1);
            }
            else{
                change(u1, 0);
            }
        }
        else{
            if (u2 == c) swap(u2, v2);
            if (u2 == u1 || u2 == v1){
                change(v2, 1);
            }
            else{
                change(u2, 0);
            }
        }
    }
    vector<int> id(n);
    iota(id.begin(), id.end(), 0);
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