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

#define MAX_CNT_Y 500000

vector<T> calc_score(vector<pii> &box, int width, int &score){
    int n = box.size();
    int sum_x = 0, max_y = 0, now_id = 0, max_x_id = -1;
    vector<T> res(n);
    while(now_id < n){
        int max_x = 0, sum_y = 0, tmp_max_x_id = max_x_id;
        while(now_id < n){
            int p = now_id, r = 0, d = 'U', b = max_x_id;
            auto [h, w] = box[now_id];
            if (h > w){
                swap(h, w);
                r ^= 1;
            }
            if (w <= max_x){
                swap(h, w);
                r ^= 1;
            }
            if (sum_y+w > width){
                if (sum_y == 0) width = w;
                break;
            }
            if (max_x < h){
                max_x = h;
                tmp_max_x_id = p;
            }
            sum_y += w;
            res[p] = {p, r, d, b};
            now_id++;
        }
        max_x_id = tmp_max_x_id;
        sum_x += max_x;
        chmax(max_y, sum_y);
    }
    score = max_y+sum_x;
    return res;
}

vector<T> simulator(vector<pii> &box){
    int n = box.size();
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, n-1);

    vector<T> res;
    int dt = -1, width = 3*box[dist(gen)].se, pre_score = 0;
    rep(i, MAX_CNT_Y){  
        int now_score = 0;
        auto tmp_op = calc_score(box, width, now_score);
        if (pre_score < now_score) dt = -dt;
        else res = tmp_op;
        pre_score = now_score;
        width = width + box[dist(gen)].se*dt;
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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, t, sigma; cin >> n >> t >> sigma;
    vector<pii> box;
    rep(i, n){
        int w, h; cin >> w >> h;
        box.emplace_back(w, h);
    }
    auto res = simulator(box);
    while(t--){
        output(res);
    }
    return 0;
}