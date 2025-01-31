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
using T = tuple<int, int, char, int>;
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

#define MAX_PROC 10000000

struct Node {
    int id, h, w, lx, ly, rx, ry;
    Node(){}
    Node(int id, int h, int w, int lx, int ly, int rx, int ry): id(id), h(h), w(w), lx(lx), ly(ly), rx(rx), ry(ry) {}
};
Node get_node(vector<Node> &put_box){
    Node res = put_box.back();
    put_box.pop_back();
    return res;
}
void push_node(vector<Node> &put_box, int id, int h, int w, int lx, int ly, int rx, int ry){
    put_box.emplace_back(id, h, w, lx, ly, rx, ry);
}

void print_nodev(vector<Node> &put_box){
    int n = put_box.size();
    rep(i, n){
        auto [id, h, w, lx, ly, rx, ry] = put_box[i];
        cout << id << ": " << "(h, w) -> (" << h << ", " << w << ") / " << lx << " " << ly << " / " << rx << " " << ry << endl;
    }
}

int calc_score(vector<Node> &put_box, int h, int w, int p, int r, int d, int b){
    int n = put_box.size();
    if (r == 1) swap(h, w);
    if (d == 0){
        int x1 = put_box[b].rx+1, x2 = x1+w-1;
        int max_x = x2, max_y = 0, max_calc_y = 0;
        rep(i, n){
            auto [id, h2, w2, lx, ly, rx, ry] = put_box[i];
            if (lx <= x2 && x1 <= rx){
                chmax(max_calc_y, ry);
            }
            chmax(max_x, rx);
            chmax(max_y, ry);
        }
        push_node(put_box, p, h, w, x1, max_calc_y+1, x2, max_calc_y+h);
        chmax(max_y, max_calc_y+h);
        return max_x+max_y;
    }
    else{
        int y1 = put_box[b].ry+1, y2 = y1+h-1;
        int max_x = 0, max_calc_x = 0, max_y = y2;
        rep(i, n){
            auto [id, h2, w2, lx, ly, rx, ry] = put_box[i];
            if (ly <= y2 && y1 <= ry){
                chmax(max_calc_x, rx);
            }
            chmax(max_x, rx);
            chmax(max_y, ry);
        }
        push_node(put_box, p, h, w, max_calc_x+1, y1, max_calc_x+w, y2);
        chmax(max_x, max_calc_x+w);
        return max_x+max_y;
    }
}

int chid(int i, int j, int max_i, int n){
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, max_i);
    if (i == 0 || i == n/4 || i == n/2 || i == 3*n/4) return dist(gen);
    return j;
}

vector<vector<T>> simulator(vector<pii> &box, int beam_width){
    int n = box.size(), k = beam_width, pre_rr = 1;
    vector<vector<Node>> put_box_k(k);
    vector<vector<T>> op_list_k(k);
    rep(i, pre_rr) put_box_k[i].emplace_back(-1, 0, 0, 0, 0, 0, 0);
    rep(p, n){
        auto [w, h] = box[p];
        vector<tuple<int, int, int, int, int, int>> score_list;
        vector<Node> node_list;
        for(int i = 0; i < pre_rr; i++){
            rep(r, 2)rep(d, 2)rep2(b, -1, p){
                int score = calc_score(put_box_k[i], h, w, p, r, d, b+1);
                score_list.emplace_back(score, r, d, b, node_list.size(), i);
                node_list.emplace_back(get_node(put_box_k[i]));
            }
        }
        sort(rng(score_list));
        vector<vector<Node>> nxt_node_list;
        vector<vector<T>> nxt_op_list;
        int rr = min(k, (int)score_list.size());
        // 上位k個の状態を保持
        for (int i = 0; i < rr; i++){
            auto [score, tmp_r, tmp_d, tmp_b, tmp_id, tmp_pre_id] = score_list[chid(p, i, score_list.size()-1, n)];
            auto tmp_node = std::vector<Node>(put_box_k[tmp_pre_id]);
            auto tmp_op = std::vector<T>(op_list_k[tmp_pre_id]);
            tmp_node.emplace_back(node_list[tmp_id]);
            tmp_op.emplace_back(p, tmp_r, (tmp_d ? 'L' : 'U'), tmp_b);
            nxt_node_list.emplace_back(tmp_node);
            nxt_op_list.emplace_back(tmp_op);
        }
        // 更新
        put_box_k = nxt_node_list;
        op_list_k = nxt_op_list;
        pre_rr = rr;
    }
    return op_list_k;
}

pii output(vector<T> &ans){
    int n = ans.size();
    cout << n << endl;
    rep(i, n){
        auto [p, r, d, b] = ans[i];
        cout << p << " " << r << " " << d << " " << b << endl;
    }
    int h, w;
    cin >> h >> w;
    return {h, w}; 
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, t, sigma; cin >> n >> t >> sigma;
    vector<pii> box;
    rep(i, n){
        int w, h; cin >> w >> h;
        box.emplace_back(w, h);
    }
    int BW = MAX_PROC/(n*n*n);
    auto res = simulator(box, BW);
    
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, res.size()-1);
    for (int i = 0; i < t; i++){
        output(res[dist(gen)]);
    }
    return 0;
}