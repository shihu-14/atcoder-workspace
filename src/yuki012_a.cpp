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

int op(int a, int b){return a|b;}
int e(){return 0;}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    vector<int> a(10), c(10);
    rep(i, n){
        int x; cin >> x;
        a[x]++;
    }
    int m = 5000000;
    vector<int> prime = {0};
    vector<bool> used(m+1);
    for(int i=2; i<=m; i++){
        if(used[i]) continue;
        prime.emplace_back(i);
        for(int j=i; j<=m; j+=i) used[j] = true;
    }
    prime.emplace_back(m+1);
    int n2 = prime.size();
    int ans = -1;

    auto f = [&]() -> int{
        int flag = 0;
        rep(i, 10){
            if (a[i] == 1 && c[i] == 0) flag = -1;
            if (a[i] == 0 && c[i] >= 1) return 1;
        }
        return flag;
    };

    auto add = [&](int x){
        int p = prime[x];
        while(p){
            c[p%10]++;
            p /= 10;
        }
    };

    auto del = [&](int x){
        int p = prime[x];
        while(p){
            c[p%10]--;
            p /= 10;
        }
    };

    int l = 1,  r = 1;
    add(1);
    while(l < n2-1){
        if (l > r && r < n2-2){
            r++;
            add(r);
            continue;
        }
        if (f() == 1){
            del(l);
            l++;
        }
        else if (f() == 0){
            chmax(ans, prime[r+1]-prime[l-1]-2);
            if (r < n2-2){
                r++;
                add(r);
            }
            else{
                del(l);
                l++;
            }
        }
        else{
            if (r < n2-2){
                r++;
                add(r);
            }
            else{
                del(l);
                l++;
            }
        }
    }
    cout << ans << endl;
    return 0;
}