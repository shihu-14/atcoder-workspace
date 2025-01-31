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

struct combination{
    vector<mint> fact, factinv;
    combination(int n): fact(n+1), factinv(n+1){
        fact[0] = 1;
        for(int i=1; i<=n; i++) fact[i] = fact[i-1]*i;
        factinv[n] = fact[n].inv();
        for(int i=n; i>0; i--) factinv[i-1] = factinv[i]*i;
    }
    mint operator()(int n, int k){
        if(n < 0 || k < 0 || k > n) return 0;
        return fact[n]*factinv[k]*factinv[n-k];
    }

    mint power(mint a, ll b){
        mint res = 1;
        for(; b; b>>=1, a*=a) if(b&1) res *= a;
        return res;
    }
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int h, w, k; cin >> h >> w >> k;
    if(k == 1){
        cout << 1 << endl;
        return 0;
    }

    combination C(h*w);
    auto f = [&](int a, int b) -> mint{
        int n = 4;
        mint res = C(a*b, k);
        rep(i, (1<<n)){
            if (i == 0) continue;
            int na = a, nb = b;
            rep(j, n){
                if(i>>j&1){
                    if (j&1) nb--;
                    else na--;
                }
            }
            res += (popcount(i)&1 ? -1 : 1)*C(na*nb, k);
        }
        return res;
    };
    mint ans = 0;
    rep2(a, 1, h+1)rep2(b, 1, w+1){
        if (a*b < k) continue;
        ans += a*b*(h-a+1)*(w-b+1)*f(a, b);
    }
    ans /= C(h*w, k);
    cout << ans.val() << endl;
    return 0;
}