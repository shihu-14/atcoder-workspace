// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
#include <chrono>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
using namespace std::chrono;
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


vector<T> simulator(vector<pii> &box, int &final_score){
    int n = box.size();
    vector<T> res;
    vector<Node> put_box;
    put_box.emplace_back(-1, 0, 0, 0, 0, 0, 0);
    rep(p, n){
        auto [w, h] = box[p];
        vector<tuple<int, int, int, int, int>> score_list;
        vector<Node> node_list;

        std::random_device rd; 
        std::mt19937 gen(rd());
        int rr = (p == n-1 ? 0 : min(2*2*(p+1)-1, 15LL));
        std::uniform_int_distribution<> dist(0, rr);

        rep(r, 2)rep(d, 2)rep2(b, -1, p){
            int score = calc_score(put_box, h, w, p, r, d, b+1);
            score_list.emplace_back(score, r, d, b, node_list.size());
            node_list.emplace_back(get_node(put_box));
        }
        sort(rng(score_list));
        auto [score, tmp_r, tmp_d, tmp_b, tmp_id] = score_list[dist(gen)];
        if (p == n-1) final_score = score;
        put_box.emplace_back(node_list[tmp_id]);
        res.emplace_back(p, tmp_r, (tmp_d ? 'L' : 'U'), tmp_b);
    }
    return res;
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
    auto start = std::chrono::high_resolution_clock::now();
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, t, sigma; cin >> n >> t >> sigma; t--;
    vector<pii> box;
    rep(i, n){
        int w, h; cin >> w >> h;
        box.emplace_back(w, h);
    }
    vector<T> final_ans;
    int final_score = INF;
    while(t--){
        int dummy = 0;
        final_ans = simulator(box, dummy);
        output(final_ans);
    }
    while(1){
        int tmp_score = 0;
        auto res = simulator(box, tmp_score);
        if (final_score > tmp_score){
            final_score = tmp_score;
            final_ans = res;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (elapsed.count() > 2900) break;
    }
    output(final_ans);
    return 0;
}