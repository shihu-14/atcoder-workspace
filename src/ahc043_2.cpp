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

vector<vector<int>> make_list_for_station(vector<pii> &start, vector<pii> &goal, int n){
    vector<vector<int>> dist(n*n, vector<int>(n*n));
    rep(i, start.size()){
        auto [x1, y1] = start[i];
        auto [x2, y2] = goal[i];
        rep2(dx1, -2, 2)rep2(dy1, -2, 2){
            rep2(dx2, -2, 2)rep2(dy2, -2, 2){
                int nx1 = x1+dx1, ny1 = y1+dy1;
                int nx2 = x2+dx2, ny2 = y2+dy2;
                if (abs(nx1-x1)+abs(ny1-y1) > 2) continue;
                if (abs(nx2-x2)+abs(ny2-y2) > 2) continue;
                if (nx1<0 || nx1>=n || ny1<0 || ny1>=n) continue;
                if (nx2<0 || nx2>=n || ny2<0 || ny2>=n) continue;
                dist[nx1*n+ny1][nx2*n+ny2] += abs(x1-x2)+abs(y1-y2);
            }
        }
    }
    return dist;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, K, T; cin >> n >> m >> K >> T;
    vector<pii> start, goal;
    rep(i, m){
        int sx, sy, gx, gy; cin >> sx >> sy >> gx >> gy;
        start.emplace_back(sx, sy);
        goal.emplace_back(gx, gy);
    }
    auto dist = make_list_for_station(start, goal, n);
    int max_money = -INF, max_x1 = -1, max_y1 = -1, max_x2 = -1, max_y2 = -1;
    rep(x1, n)rep(y1, n)rep(x2, n)rep(y2, n){
        int d = abs(x1-x2)+abs(y1-y2);
        int money = K, turn = T;
        turn -= d-1;
        turn -= 2;
        money -= (d-1)*100;
        money -= 2*5000;
        if (money < 0) continue;
        money += (dist[x1*n+y1][x2*n+y2]+dist[x2*n+y2][x1*n+y1])*turn;
        if (money > max_money){
            max_money = money;
            max_x1 = x1, max_y1 = y1;
            max_x2 = x2, max_y2 = y2;
        }
    }
    // 5*10^8/2500=2*10^5

    // 答えの出力
    cout << 0 << " " << max_x1 << " " << max_y1 << endl;
    cout << 0 << " " << max_x2 << " " << max_y2 << endl;
    if (max_x1 > max_x2){
        swap(max_x1, max_x2);
        swap(max_y1, max_y2);
    }
    int d = abs(max_x1-max_x2)+abs(max_y1-max_y2);
    while(max_x1 != max_x2){
        max_x1++;
        if (max_x1 != max_x2) cout << 2 << " " << max_x1 << " " << max_y1 << endl;
        else{
            if (max_y1 < max_y2) cout << 5 << " " << max_x1 << " " << max_y1 << endl;
            else if (max_y1 > max_y2) cout << 4 << " " << max_x1 << " " << max_y1 << endl;
        }
    }
    while(max_y1 != max_y2){
        if (max_y1 < max_y2) max_y1++;
        else max_y1--;
        if (max_y1 != max_y2) cout << 1 << " " << max_x1 << " " << max_y1 << endl;
    }
    rep(t, T-d-1) cout << -1 << endl;
    return 0;
}