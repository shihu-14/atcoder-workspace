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
    int n = 26;
    int D; cin >> D;
    vector<int> c(n), last(n);
    rep(i, n) cin >> c[i];
    vector<vector<int>> s(D, vector<int>(n));
    rep(i, D)rep(j, n) cin >> s[i][j]; 
    vector<int> ans; 
    int max_finalcost = -INF;
    // t日後先の最適解を検討する。
    rep(t, 1, 13){
        vector<int> tmp;
        int finalcost = 0; // t日後の最終コストを見て最終日程を判断する。
        // 全ての日付をシミュレーションする。
        rep(d, D){
            int max_res = INF, max_id = -1, res = 0;
            // 使わないコンテストを選び、t日後先の満足度を求める。
            rep(none, n){
                int tmp_res = s[d][none];
                rep(dt, d, min(d+t, D)){
                    rep(j, n){
                        int tmp_res = s[d][j];
                        rep(k, n){
                            if (k == j) continue;
                            res -= c[k]*(d+1-last[k]);
                        }
                        if (res > max_res){
                            max_res = res;
                            max_id = j;
                        }
                    }
                }
            }
            rep(j, n){
                if (j == max_id) continue;
                res -= c[j]*(d+1-last[j]);
            }
            res += s[d][max_id];
            last[max_id] = d+1;
            finalcost += res;
            tmp.emplace_back(max_id);
        }
        if (max_finalcost < finalcost){
            max_finalcost = finalcost;
            ans = tmp
        }
    }
    rep(i, D) cout << ans[i]+1 << endl;
    return 0;
}