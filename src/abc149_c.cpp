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
// n以下の素因数分解/約数列挙
struct Eratosthenes{
    vector<int> min_pf, prime; // min_pf[i]: iの最小の素因数, prime: 素数リスト
    // コンストラクタにおいて、ふるいを回す。
    Eratosthenes(int n) : min_pf(n+1, -1) {
        for(int i = 2; i <= n; i++){
            if(min_pf[i] != -1) continue;
            prime.emplace_back(i);
            for(int j = i; j <= n; j+=i){
                if (min_pf[j] == -1) min_pf[j] = i;
            }
        }
    }
    // nの素因数分解を返す関数。
    vector<pair<int, int>> factorize(int n){
        vector<pair<int,int>> res;
        while (n > 1) {
            int p = min_pf[n];
            int exp = 0;
            while (min_pf[n] == p) {
                n /= p;
                ++exp;
            }
            res.emplace_back(p, exp);
        }
        return res;
    }  

    // nの約数を列挙する関数。
    vector<int> divisor(int n) {
        vector<int> res({1});
        auto pf = factorize(n);
        for (auto p : pf) {
            int s = (int)res.size();
            for (int i = 0; i < s; ++i) {
                int v = 1;
                for (int j = 0; j < p.second; ++j) {
                    v *= p.first;
                    res.push_back(res[i] * v);
                }
            }
        }
        sort(res.begin(), res.end());
        return res;
    }
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int x; cin >> x;
    Eratosthenes e(1000000);
    int it = lower_bound(e.prime.begin(), e.prime.end(), x) - e.prime.begin();
    cout << e.prime[it] << endl;
    return 0;
}