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
            for (auto [nx, ny]: ts) mcf.add_edge(cnt, cnt2++, 1, 2*(abs(x-nx)+abs(y-ny)));
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
    int tn, pulling_ball = 0;
    vector<Node> tree;
    // res_stk: 残っているクエリのindex集合 [0,m) 
    // pulling_stk: 現在pull中のTreeのindex集合 [1, tn)
    vector<int> res_stk; deque<int> pulling_stk;
    Board *board;
    Tree() {}
    Tree(int n, int rx, int ry, Board *board) : tn(n), board(board){
        tree.resize(tn);
        rrep2(i, board->m, 0) res_stk.emplace_back(i);
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
        tree[0] = {rx, ry, 0, -1, -1, true};
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

    void p(){
        string res = ".";
        rep2(i, 1, tn){
            int x = tree[i].x, y = tree[i].y;
            if (x < 0 || x >= board->n || y < 0 || y >= board->n){
                res += ".";
                continue;
            }
            if (tree[i].caught){
                int x = tree[i].x, y = tree[i].y;
                auto [nx, ny] = board->to[board->query[tree[i].ball]];
                if (x == nx && y == ny){
                    res += "P";
                    board->goal[x][y] = false;
                    tree[i].caught = false;
                    tree[i].ball = -1;
                    board->put_ball++;
                    pulling_ball--;
                    pulling_stk.erase(find(rng(pulling_stk), i));
                }
                else res += ".";   
            }
            else{
                if (board->start[x][y]){
                    res += "P";
                    board->start[x][y] = false;
                    tree[i].caught = true;
                    tree[i].ball = board->id[{x, y}];
                    board->res_ball--;
                    pulling_ball++;
                    res_stk.erase(find(rng(res_stk), board->id[{x, y}]));
                    pulling_stk.emplace_back(i);
                }
                else res += ".";
            }
        }
        cout << res << endl;
    }

    int search_query(){
        int min_id = res_stk.back(), min_dist = INF;
        int rx = tree[0].x, ry = tree[0].y;
        for (auto u: res_stk){
            int d = abs(rx-board->query[u].fi)+abs(ry-board->query[u].se);
            if (min_dist > d){
                min_dist = d;
                min_id = u;
            }
        }
        return min_id;
    }

    int search_goal(){
        int min_id = pulling_stk.front(), min_dist = INF;
        int rx = tree[0].x, ry = tree[0].y;
        for (auto u: pulling_stk){
            auto [x, y] = board->to[board->query[tree[u].ball]];
            int d = abs(rx-x)+abs(ry-y);
            if (min_dist > d){
                min_dist = d;
                min_id = u;
            }
        }
        return min_id;
    }

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
        cout << "----------------------------------------------------------------" << endl;
        rep(i, tn){
            printf("(x, y) = (%d, %d), r=%d, dir=%d, ball=%d, caught=%d\n", tree[i].x, tree[i].y, tree[i].r, tree[i].d, tree[i].ball, tree[i].caught);
        }
        cout << "----------------------------------------------------------------" << endl;
    }
};



const int max_times = 200000;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, V; cin >> n >> m >> V;
    Board board(n, m);
    Tree tree(V, 0, 0, &board);
    int times = 0;
    while(!tree.isfinish() && times <= max_times){
        // cout << "!1" << endl;
        while(!tree.toput(V-1) && !tree.res_stk.empty() && times <= max_times){
            int q = tree.search_query(), qr = q%tree.tn;
            for(; tree.tree[qr].caught; qr = (qr+1)%tree.tn);
            int rx = tree.tree[0].x, ry = tree.tree[0].y;
            int nx = board.query[q].fi, ny = board.query[q].se;
            int tx = rx+nx-tree.tree[qr].x, ty = ry+ny-tree.tree[qr].y;
            while((tx < 0 || tx >= n || ty < 0 || ty >= n) && times <= max_times){
                // cout << "!2" << endl;
                tree.move(4);
                tree.rotate(1);
                tree.p();
                tx = rx+nx-tree.tree[qr].x, ty = ry+ny-tree.tree[qr].y;
                times++;
            }
            
            while((nx != tree.tree[qr].x || ny != tree.tree[qr].y) && times <= max_times){
                // cout << "!3" << endl;
                if (tree.tree[qr].x < nx) tree.move(2);
                else if (tree.tree[qr].x > nx) tree.move(0);
                else if (tree.tree[qr].y < ny) tree.move(1);
                else if (tree.tree[qr].y > ny) tree.move(3);
                else tree.move(4);
                tree.rotate(0);
                tree.p();
                times++;
            }
            times++;
            // cout << "!!!" << endl;
        }
        // cout << "!4" << endl;
        while(!tree.topull() && !tree.pulling_stk.empty() && times <= max_times){
            int i = tree.search_goal();
            int rx = tree.tree[0].x, ry = tree.tree[0].y;
            auto [nx, ny] = board.to[board.query[tree.tree[i].ball]];
            int tx = rx+nx-tree.tree[i].x, ty = ry+ny-tree.tree[i].y;
            // cout << "!5" << endl;
            while((tx < 0 || tx >= n || ty < 0 || ty >= n) && times <= max_times){
                // cout << "!6" << endl;
                tree.move(4);
                tree.rotate(1);
                tree.p();
                tx = rx+nx-tree.tree[i].x, ty = ry+ny-tree.tree[i].y;
                times++;
            }
            while((nx != tree.tree[i].x || ny != tree.tree[i].y ) && times <= max_times){
                // cout << "!7" << endl;
                if (tree.tree[i].x < nx) tree.move(2);
                else if (tree.tree[i].x > nx) tree.move(0);
                else if (tree.tree[i].y < ny) tree.move(1);
                else if (tree.tree[i].y > ny) tree.move(3);
                else tree.move(4);
                tree.rotate(0);
                tree.p();
                times++;
            }
            tree.move(4);
            tree.rotate(0);
            tree.p();
            times++;
        }
    }
    // cerr << "finish!!" << endl;
    // cerr << "times= " << times << endl;
    // cerr << board.to.size() << endl;
    return 0;
}

// seed: 4803, 32871