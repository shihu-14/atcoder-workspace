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
#define rrep2(i, a, b) for (ll i = a; i > b; --i)
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
ll gcd(ll a, ll b){ return (b ? gcd(b, a%b) : a); }
ll lcm(ll a, ll b){ return a/gcd(a, b)*b; }

// ax+by=gとなるg=gcd(a, b), x, yを求める拡張gcd
tuple<ll, ll, ll> extgcd(ll a, ll b) {
    if (b == 0) return {a, 1, 0};
    ll g, x, y;
    tie(g, x, y) = extgcd(b, a%b);
    return {g, y, x-a/b*y};
}



int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll a, b; cin >> a >> b;
    ll g = gcd(a, b);
    vector<ll> divi1, divi2, divi3;
    for(ll i = 1; i*i <= a; i++){
        if (a%i) continue;
        if (i*i == a) divi1.push_back(i);
        else divi1.push_back(i), divi1.push_back(a/i);
    }
    for(ll i = 1; i*i <= b; i++){
        if (b%i) continue;
        if (i*i == b) divi2.push_back(i);
        else divi2.push_back(i), divi2.push_back(b/i);
    }

    divi3.emplace_back(1);
    for(ll i = 2; i*i <= g; i++){
        if(g%i) continue;
        divi3.emplace_back(i);
        while(g%i == 0) g /= i;
    }
    if (g != 1) divi3.emplace_back(g);
    
    ll ans = divi3.size();
    for(auto d: divi1){
        bool flag = true;
        for(auto p: divi3){
            if (d%p == 0){
                flag = false;
                break;
            }
        }
        if (flag) ans++;
    }
    for(auto d: divi2){
        bool flag = true;
        for(auto p: divi3){
            if (d%p == 0){
                flag = false;
                break;
            }
        }
        if (flag) ans++;
    }

    cout << ans << endl;
    return 0;
}