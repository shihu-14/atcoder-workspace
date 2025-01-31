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

double calc_score(vector<Node> &put_box, int h, int w, int p, int r, int d, int b){
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
        return (double)max_x*max_y;
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
        return (double)max_x*max_y;
    }
}


vector<T> simulator_init(vector<pii> &box, double &final_occ, double sum_area){
    int n = box.size();
    vector<T> res;
    vector<Node> put_box;
    put_box.emplace_back(-1, 0, 0, 0, 0, 0, 0);
    rep(p, n){
        auto [w, h] = box[p];
        int min_score = INF, tmp_r = 0, tmp_d = 0, tmp_b = -1;
        Node tmp_node;
        rep(r, 2)rep(d, 2)rep2(b, -1, p){
            int score = calc_score(put_box, h, w, p, r, d, b+1);
            if (score < min_score){
                min_score = score;
                tmp_r = r, tmp_d = d, tmp_b = b;
                tmp_node = get_node(put_box);
                continue;
            }
            get_node(put_box);
        }
        if (p == n-1) final_occ = (double)sum_area*100.0/min_score;
        put_box.emplace_back(tmp_node);
        res.emplace_back(p, tmp_r, (tmp_d ? 'L' : 'U'), tmp_b);
    }
    return res;
}

double simulator(vector<pii> &box, vector<T> &op_list, double sum_area){
    int n = box.size();
    vector<Node> put_box;
    put_box.emplace_back(-1, 0, 0, 0, 0, 0, 0);
    rep(p, n){
        auto [w, h] = box[p];
        auto [_, r, d, b] = op_list[p];
        double score = calc_score(put_box, h, w, p, r, d, b+1);
        if (p == n-1) return sum_area*100.0/score;
    }
    return INF;
}

double output(vector<T> &ans, double sum_area){
    int n = ans.size();
    cout << n << endl;
    rep(i, n){
        auto [p, r, d, b] = ans[i];
        cout << p << " " << r << " " << d << " " << b << endl;
    }
    double h, w; cin >> h >> w;
    return sum_area*100.0/h*w; 
}

vector<T> randop(vector<T> &box){
    int n = box.size();
    std::random_device rd; 
    std::mt19937 gen(rd());
    rep(t, 10){
        std::uniform_int_distribution<> dist1(0, n-1);
        int id = dist1(gen);
        std::uniform_int_distribution<> dist2(-1, id-1);
        auto [p, r, d, b] = box[id];
        box[id] = {p, !r, (d == 'L' ? 'U' : 'L'), dist2(gen)};
    }
    return box;
}


int main(){
    auto start = std::chrono::high_resolution_clock::now();
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, t, sigma; cin >> n >> t >> sigma;
    vector<pii> box;
    double sum_area = 0;
    rep(i, n){
        int w, h; cin >> w >> h;
        box.emplace_back(w, h);
        sum_area += (double)w*h;
    }
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> rand01(0.0, 1.0);

    double T_now = 100000; // edit
    double T_rate = 0.995; // edit
    int proc = 0, proc1 = 0;

    double now_occ = 100.0;
    vector<T> now_op = simulator_init(box, now_occ, sum_area);
    // simulation
    while(1){
        vector<T> tmp_op = randop(now_op);
        double tmp_occ = simulator(box, tmp_op, sum_area);
        double d_occ = now_occ-tmp_occ;
        d_occ *= 10.0; // edit
        // if (proc > 1000 && proc < 1500) cerr << exp(d_occ/T_now) << " " << T_now << endl; // for debug

        if (d_occ > 0){
            now_occ = tmp_occ;
            now_op = tmp_op;
        }
        else if (exp(d_occ/T_now) > rand01(gen)){
            now_occ = tmp_occ;
            now_op = tmp_op;
            proc1++;
        }
        if (proc%100 == 0){
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            if (elapsed.count() > 2900) break;
        }
        T_now *= T_rate;
        proc++;
    }
    // output
    while(t--){
        vector<T> tmp_op = randop(now_op);
        double tmp_occ = output(tmp_op, sum_area);
        double d_occ = now_occ-tmp_occ;
        d_occ *= 10.0;
        if (d_occ > 0){
            now_occ = tmp_occ;
            now_op = tmp_op;
        }
        else if (exp(d_occ/T_now) > rand01(gen)){
            now_occ = tmp_occ;
            now_op = tmp_op;
            proc1++;
        }
        T_now *= T_rate;
        proc++;
    }
    cerr << proc << " " << proc1 << endl;
    cerr << (double)proc1*100/proc << endl;
    return 0;
}