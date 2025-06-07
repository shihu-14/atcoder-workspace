#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <climits>
#include <cfloat>
#include <cassert>
#include <ctime>
#include <cctype>
#include <cwctype>
#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <utility>
#include <bitset>
#include <vector>
#include <deque>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <tuple>
#include <array>
#include <new>
#include <memory>
#include <limits>
#include <random>
#include <exception>
#include <stdexcept>
#include <regex>
#include <complex>
#include <chrono>
#include <future>
#include <thread>
#include <mutex>
#include <atomic>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using mint = modint;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;

/* ----- color utilities ----- */
struct Color{double c,m,y;};
inline double d2(const Color&a,const Color&b){
    double dc=a.c-b.c,dm=a.m-b.m,dy=a.y-b.y;
    return dc*dc+dm*dm+dy*dy;
}
inline Color add(const Color&a,const Color&b){return {a.c+b.c,a.m+b.m,a.y+b.y};}
inline Color div2(const Color&a){return {a.c*0.5,a.m*0.5,a.y*0.5};}
inline Color div3(const Color&a){return {a.c/3.0,a.m/3.0,a.y/3.0};}

/* ----- chosen config ----- */
struct Chosen{array<int,3> k{};int cnt=1;};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* --- input --- */
    int N,K,H,T_lim,D;
    if(!(cin>>N>>K>>H>>T_lim>>D)) return 0;   // N=20  H=1000
    vector<Color> tube(K),target(H);
    rep(i,K)cin>>tube[i].c>>tube[i].m>>tube[i].y;
    rep(i,H)cin>>target[i].c>>target[i].m>>target[i].y;

    /* --- decide max tubes per colour (2 or 3) --- */
    int k_max=min(3,T_lim/(2*H));      // each tube adds 1 turn
    if(k_max<2)k_max=2;

    /* --- exhaustive best choice per colour --- */
    vector<Chosen> choose(H);
    rep(idx,H){
        double bestS=1e100; Chosen best;
        /* k=1 */
        rep(k1,K){
            double s=1e4*d2(tube[k1],target[idx]);
            if(s<bestS){bestS=s; best={{{(int)k1,-1,-1}},1};}
        }
        /* k=2 equal */
        rep(k1,K) rep2(k2,k1+1,K){
            Color mix=div2(add(tube[k1],tube[k2]));
            double s=D+1e4*d2(mix,target[idx]);
            if(s<bestS){bestS=s; best={{(int)k1,(int)k2,-1},2};}
        }
        /* k=3 equal */
        if(k_max==3){
            rep(k1,K) rep2(k2,k1+1,K) rep2(k3,k2+1,K){
                Color mix=div3(add(add(tube[k1],tube[k2]),tube[k3]));
                double s=2*D+1e4*d2(mix,target[idx]);
                if(s<bestS){bestS=s; best={{(int)k1,(int)k2,(int)k3},3};}
            }
        }
        choose[idx]=best;
    }

    /* --- wall pattern (horizontal wells幅 = k_max) --- */
    int G=k_max;
    rep(i,N){
        rep(j,N-1){
            cout<<((j%G==G-1)?1:0)<<(j==N-2?'\n':' ');
        }
    }
    rep(i,N-1){
        rep(j,N) cout<<1<<(j==N-1?'\n':' ');
    }

    /* --- operations --- */
    vector<string> ops; ops.reserve((G+2)*H);
    const int groupsPerRow = N/G;          // 10 (G=2)  or 6 (G=3)
    ll gid=0, totalGroups = 1LL*groupsPerRow*N;   // 200 or 120

    rep(idx,H){
        /* wrap 行番号を N でラップ */
        int row = (gid/groupsPerRow) % N;
        int col = (gid%groupsPerRow)*G;
        ++gid; if(gid==totalGroups) gid=0;        // ぐるぐる回す

        const auto& c = choose[idx];

        /* add */
        rep(t,c.cnt){
            stringstream ss; ss<<"1 "<<row<<' '<<col<<' '<<c.k[t];
            ops.push_back(ss.str());
        }
        /* take 1 g */
        { stringstream ss; ss<<"2 "<<row<<' '<<col; ops.push_back(ss.str()); }
        /* discard */
        rep(t,c.cnt-1){
            stringstream ss; ss<<"3 "<<row<<' '<<col; ops.push_back(ss.str());
        }
    }
    if((int)ops.size()>T_lim) ops.resize(T_lim);
    for(auto&s:ops) cout<<s<<'\n';
    return 0;
}
