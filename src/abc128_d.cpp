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
    int n, k; cin >> n >> k;
    vector<int> a(n);
    rep(i,n) cin >> a[i];
    int ans = -INF;
    vector<int> tmp;
    auto f = [&](auto f, int now, int t) -> void{
        if (tmp.size() == 2){
            int l = tmp[0]-1, r = tmp[1]-tmp[0]-1, c = t+2-tmp[1];
            if (l+r > n) return;
            int res = 0;
            vector<int> tmp2;
            rep2(i, 0, l){
                res += a[i];
                tmp2.emplace_back(a[i]);
            }
            rrep2(i, n-1, n-1-r){
                res += a[i];
                tmp2.emplace_back(a[i]);
            }
            sort(rng(tmp2));
            rep(i, min(c, (int)tmp2.size())) res -= tmp2[i];
            chmax(ans, res);
            return;
        }
        for(int to = now+1; to <= t+2; to++){
            tmp.emplace_back(to);
            f(f, to, t);
            tmp.pop_back();
        }
    };
    rep2(i, 1, k+1){
        f(f, 0, i);
    }
    cout << ans << endl;
    return 0;
}