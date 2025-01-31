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
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int start = clock();
    int D; cin >> D;
    int n = 26;
    vector<int> c(n);
    rep(i, n) cin >> c[i];
    vector<vector<int>> s(D, vector<int>(n));
    rep(i, D)rep(j, n) cin >> s[i][j]; 
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, D-1), dis2(0, n-1);
    vector<int> out(D);
    rep(i, D) out[i] = dis2(gen);
    auto f = [&](vector<int> date)->int{
        int res = 0;
        vector<int> last(n);
        rep(i, D){
            res += s[i][date[i]];
            rep(j, n){
                if (j == date[i]) continue;
                res -= c[j]*(i+1-last[j]);
            }
            last[date[i]] = i+1;
        }
        return res;
    };

    int now_cost = f(out);
    while((1.0*clock()-start)/CLOCKS_PER_SEC < 1.95){
        int d = dis(gen);
        int type = dis2(gen);
        int pre_type = out[d];
        out[d] = type;
        int next_cost = f(out);
        if (next_cost > now_cost){
            now_cost = next_cost;
        }else{
            out[d] = pre_type;
        }
    }
    rep(i, D) cout << out[i]+1 << endl;
    return 0;
}