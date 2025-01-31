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
#define rrep2(i, a, b) for (ll i = a; i > (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(all(v))
#define MAX(v) *max_element(all(v))
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

void output_int128(__int128 value){
    if (value == 0){ cout << "0" << endl; return; } 
    __int128 num = value;
    if (num < 0){ cout << "-"; num = -num; } 
    string result;
    while (num > 0) {
        result += '0'+(num%10);
        num /= 10;
    }
    reverse(result.begin(), result.end());
    cout << result << endl;
}
__int128 input_int128(const string& s){
    __int128 result = 0;
    bool is_negative = false;
    int i = 0;
    if (s[0] == '-') { is_negative = true; i = 1;}
    for (;i<s.size(); i++){ result = result*10 + (s[i]-'0'); } 
    if (is_negative){ result = -result;}
    return result;
}

mint power(mint a, ll n){
    mint res = 1;
    for(; n; n>>=1, a*=a) if(n&1) res *= a;
    return res;
}

__int128 power2(__int128 a, ll n, __int128 mod){
    __int128 res = 1;
    for(; n; n>>=1, a=(a*a)%mod) if(n&1) res = (res*a)%mod;
    return res;
}




int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll a, x, m; cin >> a >> x >> m;
    if (a == 1){
        cout << x%m << endl;
        return 0;
    }
    string s1 = to_string(a); 
    string s2 = to_string(a-1);
    string t = to_string(m);
    __int128 a1 = input_int128(s1), a2 = input_int128(s2), a3 = input_int128(t);
    output_int128(((power2(a1, x, a2*a3)-1)/a2%a3+a3)%a3);
    return 0;
}