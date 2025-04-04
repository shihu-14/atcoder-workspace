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
vector<T3> p2;
vector<int> Gi;
vector<T4> bounding_box;

vector<vector<int>> split_rect(){

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

    int div_size = 10;
    vector<vector<vector<int>>> div_groups(div_size, vector<vector<int>>(div_size));
    {
        int w = 10000/(div_size-1);
        vector<int> widx(n), widy(n);
        rep(i, n) widx[i] = p[i].fi/w; 
        rep(i, n) widy[i] = p[i].se/w; 
        rep(i, n){
            div_groups[widx[i]][widy[i]].emplace_back(i);   
        }
    }
    map<pii, int> mp;
    {
        int qi = 0;
        vector<int> g;
        rep(i, div_size-1)rep(j, div_size-1){
            for (int v: div_groups[i][j]) g.emplace_back(v);
            for (int v: div_groups[i][j+1]) g.emplace_back(v);
            for (int v: div_groups[i+1][j]) g.emplace_back(v);
            for (int v: div_groups[i+1][j+1]) g.emplace_back(v);
            if (g.size() < 3*L) continue;
            random_device rd; mt19937 gen(rd()); 
            uniform_int_distribution<int> dist(0, g.size()-1);
            int cnt = 0;
            while(qi < q && cnt < q/(div_size*div_size)){
                set<int> vs;
                while(vs.size() < L){
                    int v = g[dist(gen)];
                    vs.emplace(v);
                }
                cout << "? " << vs.size() << " ";
                for (int v: vs) cout << v << " ";
                cout << endl;
                rep(i, L-1){
                    int u, v; cin >> u >> v;
                    if (u > v) swap(u, v);
                    mp[{u, v}]++;
                }
                qi++;
                cnt++;
            }
        }
    }
    vector<int> id(n);
    iota(id.begin(), id.end(), 0);
    {
        for (auto [k, force]: mp){
            auto [u, v] = k;
            pull_gravity(u, v, force);
        }
        vector<int> widx(n), widy(n);
        int w = 10000/(div_size-1);
        rep(i, n) widx[i] = p[i].fi/w; 
        rep(i, n) widy[i] = p[i].se/w;
        sort(rng(id), [&](int i, int j){
            if (widx[i] != widx[j]) return widx[i] < widx[j];
            if (widx[i]&1) return widy[i] > widy[j];
            else return widy[i] < widy[j];
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
    // cerr << "enter-query-process" << endl;
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