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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int k, n; cin >> k >> n;
    vector<int> prime;
    vector<bool> used(n+1);
    for(int i=2; i<=n; i++){
        if(used[i]) continue;
        if (k <= i) prime.emplace_back(i);
        for(int j=i; j<=n; j+=i) used[j] = true;
    }
    auto f = [&](auto f, int d) -> int{
        int res = 0;
        while(d){
            res += d%10;
            d /= 10;
        }
        if (res <= 9) return res;
        else return f(f, res);
    };
    int m = prime.size();
    vector<int> h(m);
    rep(i, m) h[i] = f(f, prime[i]);
    
    int max_s = 0, max_p = -1;
    for(int i = m-1; i >= 0; i--){
        set<int> st = {h[i]};
        for(int j = 1; j < 9; j++){
            if (i+j >= m) break;
            if (st.count(h[i+j])){
                if (max_s < st.size()){
                    max_s = st.size();
                    max_p = prime[i];
                }
                break;
            }
            st.insert(h[i+j]);
        }
        if (max_s < st.size()){
            max_s = st.size();
            max_p = prime[i];
        }
    }
    cout << max_p << endl;
    return 0;
}