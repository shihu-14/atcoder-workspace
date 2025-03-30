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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N; cin >> N;
    string S, T; cin >> S >> T;

    // 1. 不可能判定
    //   S[i] = S[j] なら T[i] = T[j] でなければ -1
    //   O(N) で走査できるように，
    //   位置リストをまとめる or 直接照合 など方法はいろいろ
    {
        // たとえば，各文字ごとに出現 indices を集め，
        // そこですべて T の値が同じかを確認する
        // (ここでは簡単に map で)
        static const int ALPH = 26;
        vector<vector<int>> pos(ALPH);
        for(int i=0;i<N;i++){
            pos[S[i]-'a'].push_back(i);
        }
        for(int c=0;c<ALPH;c++){
            // 同じ文字 c を持つ全 i について，
            // T[i] が全て一致しているか？
            if(pos[c].empty()) continue;
            char firstT = T[pos[c][0]];
            for(int idx : pos[c]){
                if(T[idx] != firstT){
                    cout << -1 << "\n";
                    return 0;
                }
            }
        }
    }

    // 2. グラフ構築 (x->y) (x != y) のみ
    //    26頂点 (0~25)
    atcoder::scc_graph graph(26);
    set<pair<int,int>> edges;  // 重複防止
    for(int i=0;i<N;i++){
        int x = S[i]-'a';
        int y = T[i]-'a';
        if(x != y){
            edges.insert({x,y});
        }
    }
    for(auto &e : edges){
        graph.add_edge(e.first, e.second);
    }

    // 3. SCC 分解して，サイクル数を数える
    //   scc() はトポ順に強連結成分を返す
    auto scc = graph.scc();

    int cyc = 0;
    for(auto &comp : scc){
        if(comp.size() == 1){
            // 頂点 v の自己ループがあるかを判定
            // すなわち (v->v) があればサイクル扱い
            int v = comp[0];
            // edges の中に (v->v) があるか？
            // あるいは元の g[v] に v があるかでもよい
            // ここでは set を使うなら
            if(edges.count({v,v})){
                cyc++;
            }
        } else {
            // サイズ 2 以上は必ずサイクル
            cyc++;
        }
    }

    // 4. 答え = 変換辺の総数 + サイクル成分数
    int ans = (int)edges.size() + cyc;
    cout << ans << "\n";

    return 0;
}
