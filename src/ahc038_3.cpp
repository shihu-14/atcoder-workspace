// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;
struct Node{
    int x, y, d;
    bool caught, isleaf;
    Node() {}
    Node(int x=0, int y=0, int d=0, bool f=false, bool g = false): 
        x(x), y(y), d(d), caught(f), isleaf(g) {}
};


void move(char c, Node &v, vector<string> &s){
    s.push_back(string(1, c));
    if (c == 'U') v.x--;
    if (c == 'D') v.x++;
    if (c == 'L') v.y--;
    if (c == 'R') v.y++;
}
void rotate(char c, Node &v, vector<string> &s){
    s.push_back(string(1, c));
}
void pull(Node &v, bool f, vector<string> &s){
    if (!f){
        s.push_back(".\n");
        return;
    }
    s.push_back("P\n");
    if (v.caught) v.caught = false;
    else v.caught = true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, V; cin >> n >> m >> V;
    vector<vector<bool>> gs(n, vector<bool>(n)), gt(n, vector<bool>(n)); // 初期の盤面
    vector<pii> ss, ts; // 初期配置と目標配置の座標
    mcf_graph<int, int> mcf(2*m+2); // 最小費用
    int s = 2*m, t = 2*m+1;
    rep(i, n)rep(j, n){ 
        char c; cin >> c; 
        if (c == '1'){
            gs[i][j] = true;
            ss.emplace_back(i, j);
        }
    }
    rep(i, n)rep(j, n){ 
        char c; cin >> c; 
        if (c == '1'){
            gt[i][j] = true;
            ts.emplace_back(i, j);
        }
    }
    // cout << ss.size() << " " << ts.size() << endl;
    int cnt = 0;
    for (auto [x, y]: ss){
        mcf.add_edge(s, cnt++, 1, 0);
    }
    for (auto [x, y]: ts){
        mcf.add_edge(cnt++, t, 1, 0);
    }
    cnt = 0;
    for (auto [x, y]: ss){
        int cnt2 = m;
        for (auto [nx, ny]: ts){
            mcf.add_edge(cnt, cnt2++, 1, abs(x-nx)+abs(y-ny));
        }
        cnt++;
    }
    mcf.flow(s, t);
    vector<pii> query;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (gs[i][j]) query.emplace_back(i, j);   
        }
    }
    map<pii, pii> mp;
    for (auto e: mcf.edges()){
        if (e.from == s || e.to == t || e.flow == 0) continue;
        mp[ss[e.from]] = ts[e.to-m];
    }

    vector<string> ans; // 答えの操作を格納
    int ans_id = 0, min_turn = INF;
    for (int start = 0; start < m; start++){
        vector<bool> used(m, false);
        vector<string> ret;
        int turn = 0;
        int p = start, res = m;
        Node root = {query[start].fi, query[start].se, 0, true, true};
        while(res){
            auto [x, y] = query[p]; used[p] = true; res--;
            auto [gx, gy] = mp[{x, y}];
            // cout << x << " " << y << "->" << gx << " " << gy << endl;
            while(root.x != gx || root.y != gy){
                // cout << root.x << " " << root.y << endl;
                if (root.x < gx) move('D', root, ret);
                else if (root.x > gx) move('U', root, ret);
                else if (root.y < gy) move('R', root, ret);
                else if (root.y > gy) move('L', root, ret);
                // rotate('.', root);
                pull(root, false, ret);
                turn++;
            }
            ret.emplace_back(".P\n");
            turn++;
            if (res == 0) break;
            int min_dist = INF, min_id = -1;
            rep(i, m){
                if (used[i]) continue;
                int tmp_d = abs(gx - query[i].fi) + abs(gy - query[i].se);
                if (min_dist > tmp_d){
                    min_dist = tmp_d;
                    min_id = i;
                }
            }
            p = min_id;
            auto [nx, ny] = query[p];
            while(root.x != nx || root.y != ny){
                if (root.x < nx) move('D', root, ret);
                else if (root.x > nx) move('U', root, ret);
                else if (root.y < ny) move('R', root, ret);
                else if (root.y > ny) move('L', root, ret);
                // rotate('.', root);
                pull(root, false, ret);
                turn++;
            }
            ret.emplace_back(".P\n");
            turn++;
        }
        if (turn < min_turn){
            min_turn = turn;
            ans_id = start;
            ans = ret;
        }
    }
    cout << 1 << endl;
    cout << query[ans_id].fi << " " << query[ans_id].se << endl;
    cout << ".P\n";
    rep(i, ans.size()) cout << ans[i];
    return 0;
}