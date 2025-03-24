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

int calc(int L, const vector<int> &T, vector<pii> ans){
    int n = T.size();
    vector<int> cnt(n);
    int now = 0;
    rep(i, L){
        auto [a, b] = ans[now];
        cnt[now]++;
        if (cnt[now] & 1) {
            now = a;
        } 
        else {
            now = b;
        }
    }
    int res = 0;
    rep(i, n){
        res += abs(cnt[i]-T[i]);
    }
    return res;
}

vector<pii> make_ans(vector<int> &vs, vector<int> &T, vector<bool> &issmall, int bd){
    int n = T.size();
    int m = vs.size();
    int max_ti = 0, max_id = -1;
    vector<pii> res(n);
    rep(i, m){
        int v = vs[i], u = vs[(i+1)%m];
        if(T[v] >= bd){
            res[v] = {v, u};
        }
        else {
            res[v] = {u, u};
        }
        if (T[v] > max_ti){
            max_ti = T[v];
            max_id = v;
        }
    }
    rep(i, n) if (issmall[i]) res[i] = {i, i};
    if (issmall[0]){
        res[0] = {vs[max_id], vs[max_id]};
    }
    return res;
}
int main(){
    auto start = std::chrono::high_resolution_clock::now();
    int n, L; 
    cin >> n >> L;
    vector<int> T(n);
    rep(i, n) cin >> T[i];
    vector<int> small, ave;
    rep3(i, 1000, 2200, 30) small.emplace_back(i);
    rep3(i, 5000, 5800, 100) ave.emplace_back(i);
    // rep(i, ave.size()) cerr << ave[i] << " "; cerr << endl;
    vector<pii> final_ans;
    int min_score = INF;
    int min_ave = 0, min_small = 0;
    int times = 0,  update = 0;
    for (auto av: ave){
        for (auto sm: small){
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            if (elapsed.count() > 1900) break;
            times++;
            vector<int> vs;
            vector<bool> issmall(n);
            rep(i, n){
                if (T[i] <= sm){
                    issmall[i] = true;
                }
                else{
                    vs.emplace_back(i);
                }
            }
            auto tmp_ans = make_ans(vs, T, issmall, av);
            auto score = calc(L, T, tmp_ans);
            if (min_score > score){
                update++;
                min_score = score;
                min_ave = av;
                min_small = sm;
                final_ans = tmp_ans;
            }
        }
    }    
    cerr << 1000000-min_score << endl;
    cerr << times << " " << update << endl;
    cerr << min_ave << " " << min_small << endl;
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cerr << elapsed.count() << endl;
    }
    for (auto [a, b]: final_ans){
        cout << a << " " << b << endl;
    }
    return 0;
}
