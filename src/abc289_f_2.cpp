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
    int sx, sy, gx, gy; cin >> sx >> sy >> gx >> gy;
    int a, b, c, d; cin >> a >> b >> c >> d;
    if ((sx%2) != (gx%2) || (sy%2) != (gy%2)) {
        cout << "No" << endl;
        return 0;
    }

    int flag = 0;
    if (a == b){
        flag += 1;
    }
    if (c == d){
        flag += 2;
    }
    if (flag > 0){
        if (flag == 1){
            if (sx != gx && 2*a-sx != gx){
                cout << "No" << endl;
                return 0;
            }
            cout << "Yes" << endl;
            if (sx != gx){
                sx = 2*a-sx;
                sy = 2*c-sy;
                cout << a << " " << c << endl;
            }
            while(sy != gy){
                if (sy < gy){
                    cout << a << " " << c << endl;
                    cout << a << " " << c+1 << endl;
                    sy += 2;
                }
                else {
                    cout << a << " " << c+1 << endl;
                    cout << a << " " << c << endl;
                    sy -= 2;
                }
            }
        }
        else if(flag == 2){
            if (sy != gy && 2*c-sy != gy){
                cout << "No" << endl;
                return 0;
            }
            cout << "Yes" << endl;
            if (sy != gy){
                sx = 2*a-sx;
                sy = 2*c-sy;
                cout << a << " " << c << endl;
            }
            while(sx != gx){
                if (sx < gx){
                    cout << a << " " << c << endl;
                    cout << a+1 << " " << c << endl;
                    sx += 2;
                }
                else {
                    cout << a+1 << " " << c << endl;
                    cout << a << " " << c << endl;
                    sx -= 2;
                }
            }
        }
        else{
            if ((sx != gx || sy != gy) && (2*a-sx != gx || 2*c-sy != gy)){
                cout << "No" << endl;
                return 0;
            }
            cout << "Yes" << endl;
            if (sx != gx){
                sx = 2*a-sx;
                sy = 2*c-sy;
                cout << a << " " << c << endl;
            }
        }
        return 0;
    }
    cout << "Yes" << endl;
    while(sx != gx){
        if (sx < gx){
            cout << a << " " << c << endl;
            cout << a+1 << " " << c << endl;
            sx += 2;
        }
        else {
            cout << a+1 << " " << c << endl;
            cout << a << " " << c << endl;
            sx -= 2;
        }
    }
    while(sy != gy){
        if (sy < gy){
            cout << a << " " << c << endl;
            cout << a << " " << c+1 << endl;
            sy += 2;
        }
        else {
            cout << a << " " << c+1 << endl;
            cout << a << " " << c << endl;
            sy -= 2;
        }
    }
    return 0;
}