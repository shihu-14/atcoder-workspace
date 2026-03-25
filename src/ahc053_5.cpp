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
template<class T>
using G = vector<vector<T>>;
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

static inline ull ceil128(__int128 a, __int128 b){ return a<=0?0ULL:(ull)((a+b-1)/b); }

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N,M; ull L,U;
    if(!(cin>>N>>M>>L>>U)) return 0;

    __int128 width = ( __int128)U - ( __int128)L;
    ull d = ceil128(width, 512); if(d==0) d=1;

    ll sets = (N >= 10LL*M ? M : min<ll>(M, N/10));
    auto idx_anchor = [&](ll j)->ll{ return j; };
    auto idx_piece  = [&](ll j, ll k)->ll{ return sets + j*9 + k; };

    vector<ull> A(max<ll>(0,N), 1);
    for(ll j=0;j<sets;++j){
        if(idx_anchor(j) < N) A[idx_anchor(j)] = L;
        for(ll k=0;k<9;++k){
            __int128 piece128 = ( (__int128)d << k );
            ull piece = (ull)(piece128>(__int128)U?U:piece128);
            ll id = idx_piece(j,k);
            if(id < N) A[id] = piece;
        }
    }
    printv(A);
    cout.flush();

    vector<ull> B(M,0);
    for(ll j=0;j<M;++j) if(!(cin>>B[j])) B[j]=L;

    vector<ll> X_round(N,0), X_glob(N,0);
    unsigned __int128 E_round=0, E_glob=0;

    for(ll j=0;j<sets;++j){
        ll ia=idx_anchor(j);
        if(0<=ia && ia<N){ X_round[ia]=(int)(j+1); X_glob[ia]=(int)(j+1); }
    }

    for(ll j=0;j<sets;++j){
        ull Bj=B[j];
        ull r = (Bj>=L?Bj-L:0ULL);
        ull t = (r + d/2)/d; if(t>511ULL) t=511ULL;
        for(ll k=0;k<9;++k){
            if(t & (1ULL<<k)){
                ll id=idx_piece(j,k);
                if(0<=id && id<N) X_round[id]=(int)(j+1);
            }
        }
        unsigned long long Sj=(unsigned long long)(L + t*d);
        unsigned long long e=(Bj>=Sj?Bj-Sj:Sj-Bj);
        E_round += (unsigned __int128)e;
    }

    vector<vector<int>> pool(9);
    for(int k=0;k<9;++k){
        pool[k].reserve(sets);
        for(ll s=0;s<sets;++s){ ll id=idx_piece(s,k); if(0<=id && id<N) pool[k].push_back((int)id); }
    }
    vector<int> ptr(9,0), stock(9,0);
    for(int k=0;k<9;++k) stock[k]=(int)pool[k].size();

    vector<long long> r(M,0);
    for(ll j=0;j<M;++j) r[j]=(long long)B[j] - (long long)L;

    auto gain_of = [&](long long rr, ull v)->long long{
        long long before = llabs(rr);
        long long after  = llabs(rr - (long long)v);
        return before - after;
    };

    struct Entry{ long long gain; int k; int j; int ver; };
    struct Cmp{ bool operator()(const Entry& a, const Entry& b) const { return a.gain < b.gain; } };
    priority_queue<Entry, vector<Entry>, Cmp> pq;
    vector<int> ver(M,0);
    ull v[9]; for(int k=0;k<9;++k) v[k]=(ull)((__int128)d<<k);

    for(int k=0;k<9;++k) if(stock[k]>0)
        for(int j=0;j<(int)M;++j){
            long long g = gain_of(r[j], v[k]);
            if(g>0) pq.push({g,k,j,ver[j]});
        }

    while(!pq.empty()){
        auto cur=pq.top(); pq.pop();
        if(cur.ver!=ver[cur.j]) continue;
        if(stock[cur.k]<=0) continue;
        long long gnow = gain_of(r[cur.j], v[cur.k]);
        if(gnow!=cur.gain){
            if(gnow>0) pq.push({gnow,cur.k,cur.j,ver[cur.j]});
            continue;
        }
        int k=cur.k, j=cur.j;
        int id = pool[k][ptr[k]++];
        stock[k]--;
        X_glob[id]=j+1;
        r[j]-=(long long)v[k];
        ver[j]++;
        for(int kk=0;kk<9;++kk) if(stock[kk]>0){
            long long g2 = gain_of(r[j], v[kk]);
            if(g2>0) pq.push({g2,kk,j,ver[j]});
        }
    }
    for(ll j=0;j<M;++j) E_glob += (unsigned __int128)llabs(r[j]);

    const vector<ll>& X = (E_glob<=E_round ? X_glob : X_round);
    rep(i, N){ if(i) cout<<' '; cout<<X[i]; } cout<<'\n';
    cout.flush();
    return 0;
}
