#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < n; ++i)
#define rep2(i, a, b) for (ll i = a; i < b; ++i)
#define rep3(i, a, b, c) for (ll i = a; i < b; i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
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
    int h, w, t; cin >> h >> w >> t;
    vector<string> s(h);
    rep(i, h) cin >> s[i];
    int ans = INF;
    rep(i, 1<<(h-1)){
        vector<bool> divied(h);
        int cnt = 0;
        rep(j, h-1){
            if ((i>>j)&1){
                divied[j] = true;
                cnt++;
            }
        }
        vector<vector<int>> a(cnt+1, vector<int>(w));
        rep(j, w){
            int tmp = 0, cnt2 = 0;
            rep(k, h){
                if (s[k][j] == '1') tmp++;
                if (divied[k]){
                    a[cnt2++][j] = tmp;
                    tmp = 0;
                }
            }
            a[cnt2][j] = tmp;
        }
        bool ok = false;
        rep(j, w){
            rep(k, cnt+1) if (a[k][j] > t){
                ok = true;
                break;
            }
        }
        if (ok) continue;
        int res = cnt;
        vector<ll> tmp(cnt+1, 0); // それぞれの分割領域のホワイトチョコの累積個数
        rep(j, cnt+1) tmp[j] = a[j][0];
        rep2(j, 1, w){
            bool flag = false;
            rep(k, cnt+1){
                if (tmp[k] + a[k][j] > t){
                    flag = true;
                    break;
                }
                tmp[k] += a[k][j];
            }
            if (flag){
                res++;
                rep(k, cnt+1) tmp[k] = a[k][j];
            }
        }
        chmin(ans, res);
    }
    cout << ans << endl;
    return 0;
}