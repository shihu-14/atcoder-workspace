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
// #define rng(a) a.begin(),a.end()
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


/* ---------- RNG (xoroshiro128+) ---------- */
static inline uint64_t rotl64(uint64_t x,int k){ return (x<<k)|(x>>(64-k)); }
struct FastRNG{
    uint64_t s[2];
    FastRNG(uint64_t seed){
        uint64_t z=seed+=0x9e3779b97f4a7c15ULL;
        z=(z^(z>>30))*0xbf58476d1ce4e5b9ULL; z=(z^(z>>27))*0x94d049bb133111ebULL;
        s[0]=z^(z>>31);
        z=seed+=0x9e3779b97f4a7c15ULL;
        z=(z^(z>>30))*0xbf58476d1ce4e5b9ULL; z=(z^(z>>27))*0x94d049bb133111ebULL;
        s[1]=z^(z>>31);
    }
    uint64_t next(){uint64_t a=s[0],b=s[1];s[0]=b; a^=a<<23; return s[1]=a^b^(a>>17)^(b>>26), s[1]+b;}
    uint32_t r32(uint32_t m){return uint32_t(next()%m);}
} rnd(chrono::steady_clock::now().time_since_epoch().count());

/* ---------- colour utilities ---------- */
struct Color{ double c,m,y; };
inline double d2(const Color&a,const Color&b){
    double dc=a.c-b.c, dm=a.m-b.m, dy=a.y-b.y; return dc*dc+dm*dm+dy*dy;
}
inline Color add(const Color&a,const Color&b){ return {a.c+b.c, a.m+b.m, a.y+b.y}; }
inline Color div2(const Color&a){ return {a.c*0.5, a.m*0.5, a.y*0.5}; }
inline Color div3(const Color&a){ return {a.c/3.0, a.m/3.0, a.y/3.0}; }

/* ---------- config per target ---------- */
struct Cfg{ array<int,3> k; int cnt; };   // k[0..cnt-1] 有効

int main(){
    ios::sync_with_stdio(false); cin.tie(nullptr);

    /* ---- input ---- */
    int N,K,H,Tlim,D; if(!(cin>>N>>K>>H>>Tlim>>D)) return 0;
    vector<Color> tube(K), target(H);
    rep(i,K) cin>>tube[i].c>>tube[i].m>>tube[i].y;
    rep(i,H) cin>>target[i].c>>target[i].m>>target[i].y;

    /* ---- initial selection: best of {1, 1+1, 1+1+1(equal)} ---- */
    vector<Cfg> cfg(H);
    rep(i,H){
        double bestErr = 1e100;
        Cfg best{{{-1,-1,-1}},1};

        /* single */
        rep(k,K){
            double err=d2(tube[k],target[i]);
            if(err<bestErr){ bestErr=err; best={{(int)k,-1,-1},1}; }
        }

        /* pairs 1:1 */
        rep(k1,K){
            rep2(k2,k1+1,K){
                Color mix = div2(add(tube[k1],tube[k2]));
                double err = d2(mix,target[i]);
                if(err<bestErr){
                    bestErr=err; best={{(int)k1,(int)k2,-1},2};
                }
            }
        }

        /* triples 1:1:1 (only from best 4 singles for speed) */
        vector<pair<double,int>> singles;
        rep(k,K) singles.push_back({d2(tube[k],target[i]),k});
        nth_element(singles.begin(), singles.begin()+4, singles.end());
        rep(a,4) rep2(b,a+1,4) rep2(c,b+1,4){
            int k1=singles[a].second, k2=singles[b].second, k3=singles[c].second;
            Color mix = div3(add(add(tube[k1],tube[k2]), tube[k3]));
            double err = d2(mix,target[i]);
            if(err<bestErr){
                bestErr=err; best={{k1,k2,k3},3};
            }
        }
        cfg[i]=best;
    }

    /* ---- quick hill-climb: swap one tube if improves ---- */
    const double TIME_LIMIT = 1.5;                           // seconds
    auto t0 = chrono::steady_clock::now();
    vector<Color> curMix(H);
    rep(i,H){
        const auto& c=cfg[i];
        if(c.cnt==1) curMix[i]=tube[c.k[0]];
        else if(c.cnt==2) curMix[i]=div2(add(tube[c.k[0]],tube[c.k[1]]));
        else curMix[i]=div3(add(add(tube[c.k[0]],tube[c.k[1]]),tube[c.k[2]]));
    }
    while(chrono::duration<double>(chrono::steady_clock::now()-t0).count()<TIME_LIMIT){
        int id=rnd.r32(H);
        Cfg cand=cfg[id];
        int pos=rnd.r32(cand.cnt);
        int nk=rnd.r32(K);
        if(nk==cand.k[pos]) continue;
        cand.k[pos]=nk;

        Color mix;
        if(cand.cnt==1) mix=tube[cand.k[0]];
        else if(cand.cnt==2) mix=div2(add(tube[cand.k[0]],tube[cand.k[1]]));
        else mix=div3(add(add(tube[cand.k[0]],tube[cand.k[1]]),tube[cand.k[2]]));

        if(d2(mix,target[id]) < d2(curMix[id],target[id])){
            cfg[id]=cand;
            curMix[id]=mix;
        }
    }

    /* ---- walls: 2-cell alternating vertical ---- */
    rep(i,N){
        rep(j,N-1) cout<<(j%2==0?0:1)<<(j==N-2?'\n':' ');
    }
    rep(i,N-1){
        rep(j,N) cout<<1<<(j==N-1?'\n':' ');
    }

    /* ---- operations ---- */
    vector<string> ops; ops.reserve(6*H);
    int idx=0, perRow=N/2;
    rep(i,H){
        int r = idx/perRow;
        int c = (idx%perRow)*2;
        idx = (idx+1)%(perRow*N);
        const auto& cf = cfg[i];

        rep(k,cf.cnt){
            ostringstream ss; ss<<"1 "<<r<<' '<<c<<' '<<cf.k[k]; ops.push_back(ss.str());
        }
        {
            ostringstream ss; ss<<"2 "<<r<<' '<<c; ops.push_back(ss.str());
        }
        if(cf.cnt>1){
            ostringstream ss; ss<<"3 "<<r<<' '<<c; ops.push_back(ss.str());
        }
    }
    if((int)ops.size()>Tlim) ops.resize(Tlim);
    for(auto& s:ops) cout<<s<<'\n';
    return 0;
}
