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
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define rep2(i, a, b) for (int i = a; i < (b); ++i)
#define rrep2(i, a, b) for (int i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (int i = a; i < (b); i+=c)
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
const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, 1, 0, -1};
const char dc[] = {'U', 'R', 'D', 'L'};
const char dir[] = {'L', '.', 'R', 'R'};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

struct Board{
    int n, m, res_ball, put_ball; // res_ball:まだpullしていないボールの数, put_ball:すでにpull->putしているボールの数
    vector<vector<bool>> start, goal;
    vector<pii> query;
    vector<bool> query_used, output_used;
    map<pii, pii> to;
    map<pii, int> id;
    Board() {}
    Board(int n, int m): n(n), m(m), res_ball(m), put_ball(0){
        start.resize(n, vector<bool>(n));
        goal.resize(n, vector<bool>(n));
        vector<pii> ss, ts;
        mcf_graph<int, int> mcf(2*m+2);
        int s = 2*m, t = 2*m+1, cnt = 0;
        rep(i, n)rep(j, n){ char c; cin >> c; if (c == '1') start[i][j] = true, ss.emplace_back(i, j); }
        rep(i, n)rep(j, n){ char c; cin >> c; if (c == '1') goal[i][j] = true, ts.emplace_back(i, j); } 
        for (auto [x, y]: ss) mcf.add_edge(s, cnt++, 1, 0);
        for (auto [x, y]: ts) mcf.add_edge(cnt++, t, 1, 0);
        cnt = 0;
        for (auto [x, y]: ss){
            int cnt2 = m;
            for (auto [nx, ny]: ts) mcf.add_edge(cnt, cnt2++, 1, abs(x-nx)+abs(y-ny));
            cnt++;
        }
        mcf.flow(s, t);
        for (auto e: mcf.edges()){
            if (e.from == s || e.to == t || e.flow == 0) continue;
            to[ss[e.from]] = ts[e.to-m];
        }
        make_query();
    }
    // クエリの探索 (edit here)
    void make_query(){
        query_used.resize(m);
        output_used.resize(m);
        rep(i, n){
            if (i&1){
                rrep2(j, n, 0)if(start[i][j]){
                    id[{i, j}] = query.size();
                    query.emplace_back(i, j);
                }
            }
            else{
                rep(j, n) if(start[i][j]){
                    id[{i, j}] = query.size();
                    query.emplace_back(i, j);
                }
            }
        }
    }
};

struct Node{
    int x, y, r, d, ball;
    bool caught;
    Node() {}
    Node(int x, int y, int r, int d, int ball, bool caught): 
        x(x), y(y), r(r), d(d), ball(ball), caught(caught) {}
};

struct Tree{
    int tn, pulling_ball;
    vector<Node> tree;
    vector<vector<vector<double>>> fcost;
    Board *board;
    Tree() {}
    Tree(int n, int rx, int ry, Board *board) : tn(n), board(board), pulling_ball(0){
        tree.resize(tn);
        fcost.resize(board->n, vector<vector<double>>(board->n, vector<double>(4, INF)));
        output_tree_info(rx, ry);
        init_tree(rx, ry);
    }

    void output_tree_info(int sx, int sy){
        cout << tn << endl;
        rep(i, tn-1){
            cout << 0 << " " << (i+4)/4 << endl;
        }
        cout << sx << " " << sy << endl;
    }
    void init_tree(int rx, int ry){
        string res;
        res += ".";
        tree[0] = {rx, ry, 0, -1, -1, false};
        rep(t, tn-1){
            tree[t+1] = {rx+dx[t%4], ry+dy[t%4], (t+4)/4, t%4, -1, false};
            res += dir[t%4];
        }
        rep(i, tn) res += ".";
        cout << res << endl;
        res = ".";
        rep(t, tn-1){
            res += (t%4 == 3 ? "R" : ".");
        }
        rep(i, tn) res += ".";
        cout << res << endl;
    }
    void move(int t){
        if (t == 4){
            cout << "."; return;
        }
        int rx = tree[0].x+dx[t], ry = tree[0].y+dy[t];
        if (rx < 0 || rx >= board->n || ry < 0 || ry >= board->n){
            cout << "."; return;
        }
        rep(i, tn){
            tree[i].x += dx[t];
            tree[i].y += dy[t];
        }
        cout << dc[t];
    }
    void rotate(int c){
        int rx = tree[0].x, ry = tree[0].y;
        string res;
        rep2(i, 1, tn){
            int nd = ((tree[i].d+c)%4+4)%4;
            tree[i] = {rx+dx[nd]*tree[i].r, ry+dy[nd]*tree[i].r, tree[i].r, nd, tree[i].ball, tree[i].caught};
            res += (c == 0 ? "." : c == 1 ? "R" : "L");
        }
        cout << res;
    }

    void pull(){
        string res = ".";
        rep2(i, 1, tn){ // ノードの位置が盤面に収まっていて、今ボールを掴んでいないかつ、そこにボールがある場合、pullする。
            int x = tree[i].x, y = tree[i].y;
            if (x < 0 || x >= board->n || y < 0 || y >= board->n){
                res += ".";
                continue;
            }
            if (!board->start[x][y] || tree[i].caught){
                res += ".";
                continue;
            }
            res += "P";
            board->start[x][y] = false;
            tree[i].caught = true;
            tree[i].ball = board->id[{x, y}];
            board->query_used[board->id[{x, y}]] = true;
            board->res_ball--;
            pulling_ball++;
        }
        cout << res << endl;
    }

    void put(){
        string res = ".";
        rep2(i, 1, tn){
            if(tree[i].caught){ // ボールを掴んでいるノードのうち、目的地が今のノードの位置と一致するものはputする。
                int x = tree[i].x, y = tree[i].y;
                auto [nx, ny] = board->to[board->query[tree[i].ball]];
                if (x == nx && y == ny){
                    res += "P";
                    board->goal[x][y] = false;
                    tree[i].caught = false;
                    tree[i].ball = -1;
                    board->output_used[board->id[{x, y}]] = true;
                    board->put_ball++;
                    pulling_ball--;
                }
                else res += ".";
            }
            else res += ".";
        }
        cout << res << endl;
    }

    double calc_g(){return 1;}

    double calc_f(int op1, int op2, bool ispull){
        vector<Node> cord = tree;
        // ここから、
        int xx = 0, yy = 0, dd = 0;
        if (op1 == 0) xx = -1;    
        else if (op1 == 2) xx = 1;
        else if (op1 == 3) yy = -1;
        else if (op1 == 1) yy = 1;
        rep(i, tn) cord[i].x += xx, cord[i].y += yy;
        if (cord[0].x < 0 || cord[0].x >= board->n || cord[0].y < 0 || cord[0].y >= board->n) return INF;
        if (op2 == -1) dd = -1;
        else if (op2 == 1) dd = 1;
        rep2(i, 1, tn) cord[i].d = ((cord[i].d+dd)%4+4)%4;
        rep2(i, 1, tn){
            cord[i].x = cord[0].x+dx[cord[i].d]*cord[i].r;
            cord[i].y = cord[0].y+dy[cord[i].d]*cord[i].r;
        }
        // ここまで、与えられた操作を仮想的にtreeに反映させている。
        double res = 0;
        // 評価関数の設計(edit here)
        if (ispull){ // ボールを引き上げる時において、
            rep2(i, 1, tn){ // すでに掴んでいないノードに対して、そのノードが掴むべきマスを前から順番に見ていく。
                if (cord[i].caught) continue;
                int x = cord[i].x, y = cord[i].y;
                for (int j = i; j < board->m; j += tn){
                    if (board->query_used[j]) continue;
                    auto [nx, ny] = board->query[j];
                    res += sqrt((x-nx)*(x-nx) + (y-ny)*(y-ny))/i;
                    break;
                }
            }
        }
        else{ // ボールを置く時において、
            rep2(i, 1, tn){ // ボールを掴んでいるノードに対して、そのボールの置く場所までのコストを計算する。
                if (!cord[i].caught) continue;
                int x = cord[i].x, y = cord[i].y;
                auto [nx, ny] = board->to[board->query[cord[i].ball]];
                res += sqrt((x-nx)*(x-nx) + (y-ny)*(y-ny))/i;
            }
        }
        chmin(fcost[cord[0].x][cord[0].y][cord[1].d], res+calc_g());
        return fcost[cord[0].x][cord[0].y][cord[1].d];
    }
    pii next_step(int ispull){
        int op1 = 5, op2 = 0; double min_cost = INF;
        rep(i, 5)rep2(j, -1, 2){ // UDLR. かつ LR.を総当たりして最小コストの操作を行う。
            if (i == 4 && j == 0) continue; 
            double res = calc_f(i, j, ispull);
            if (min_cost > res){
                op1 = i, op2 = j, min_cost = res;
            }
        }
        return {op1, op2};
    }

    void cash_clear(){
        rep(i, board->n)rep(j, board->n)rep(k, 4) fcost[i][j][k] = INF;
    }

    // toput/topullで打ち切り
    bool toput(int limit){
        if (limit <= pulling_ball) return true;
        return board->res_ball == 0;
    }
    bool topull(){
        return pulling_ball == 0;
    }
    bool isfinish(){
        return board->put_ball == board->m;
    }

    void tree_print(){
        rep(i, tn){
            printf("(x, y) = (%d, %d), r=%d, dir=%d, ball=%d, caught=%d\n", tree[i].x, tree[i].y, tree[i].r, tree[i].d, tree[i].ball, tree[i].caught);
        }
        cout << "----------------------------------------------------------------" << endl;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, V; cin >> n >> m >> V;
    Board board(n, m);
    Tree tree(V, 0, 0, &board);
    while(!tree.isfinish()){
        tree.cash_clear();
        while(!tree.toput(V-1)){
            auto [op1, op2] = tree.next_step(1);
            tree.move(op1);
            tree.rotate(op2);
            tree.pull();
        }
        tree.cash_clear();
        while(!tree.topull()){
            auto [op1, op2] = tree.next_step(0);
            tree.move(op1);
            tree.rotate(op2);
            tree.put();
        }
    }
    return 0;
}