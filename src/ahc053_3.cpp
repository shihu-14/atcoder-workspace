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
using namespace std;
using namespace atcoder;
using mint = modint998244353;
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

static inline ll clamp_ll(ll x, ll lo, ll hi){ if(x<lo) return lo; if(x>hi) return hi; return x; }
static inline ll iabs(ll x){ return x>=0? x : -x; }
static inline unsigned __int128 cdiv128(unsigned __int128 a, unsigned __int128 b){ return (a + b - 1) / b; }

void solve(){
    ll N,M,L,U; if(!(cin>>N>>M>>L>>U)) return;
    ll K = 0;
    if(M>0){ ll t = (ll)(N/M) - 2; if(t<0) t=0; if(t>50) t=50; K=t; }
    ull pow2K = (K? (1ull<<K) : 1ull);
    ll s = 0;
    if(K){
        unsigned __int128 d = (unsigned __int128)(U-L);
        unsigned __int128 De = cdiv128(d, (unsigned __int128)M);
        unsigned __int128 num = d + De;
        while(K){
            pow2K = (1ull<<K);
            unsigned __int128 den = (unsigned __int128)pow2K - 1u;
            unsigned __int128 sv = cdiv128(num, den);
            if(sv==0) sv=1;
            unsigned __int128 top = sv * (1ull<<(K-1));
            if(top <= (unsigned __int128)U){ s = (ll)sv; break; }
            --K;
        }
        if(!K) s = 0;
    }
    vector<ll> A; A.reserve(N);
    vector<int> typ; typ.reserve(N);
    vector<int> par; par.reserve(N);
    auto mu = [&](ll j)->ll{
        __int128 num = (__int128)(2*j-1) * (__int128)(U-L);
        __int128 den = (__int128)2*M;
        return L + (ll)(num/den);
    };
    unsigned __int128 C=0,halfC=0;
    if(K){ C = (unsigned __int128)s * ((unsigned __int128)pow2K - 1u); halfC = C/2u; }
    vector<pair<ll,int>> anchors; anchors.reserve(2*M);
    rep(j,M){
        ll v = K? clamp_ll((ll)((__int128)mu(j+1) - (__int128)halfC),1LL,U) : clamp_ll(mu(j+1),1LL,U);
        A.push_back(v); typ.push_back(0); par.push_back((int)j);
        anchors.emplace_back(v,(int)A.size()-1);
    }
    rep(j,M){
        ll v = K? clamp_ll((ll)((__int128)mu(j+1) + (__int128)halfC),1LL,U) : clamp_ll(mu(j+1),1LL,U);
        A.push_back(v); typ.push_back(1); par.push_back((int)j);
        anchors.emplace_back(v,(int)A.size()-1);
    }
    vector<vector<int>> coin_ids(K);
    vector<ll> coin_val(K,0);
    rep(k,K){
        unsigned __int128 v128 = (unsigned __int128)s * (1ull<<k);
        ll v = (ll)v128; if(v<1) v=1; if(v>U) v=U;
        coin_val[k]=v;
        rep(j,M){ A.push_back(v); typ.push_back(2+k); par.push_back(k); coin_ids[k].push_back((int)A.size()-1); }
    }
    while((ll)A.size()<N){ A.push_back(1); typ.push_back(100); par.push_back(-1); }
    rep(i,N){ if(i) cout<<' '; cout<<A[i]; } cout<<'\n'<<flush;

    vector<ll> B(M); rep(i,M) cin>>B[i];

    sort(rng(anchors));
    vector<int> used(anchors.size(),0), anchor_of(M,-1);
    vector<pair<ll,int>> Bs; Bs.reserve(M);
    rep(j,M) Bs.emplace_back(B[j],(int)j);
    sort(rng(Bs));
    int p=0; vector<int> avail;
    rep(ti,M){
        ll bj=Bs[ti].fi; int j=Bs[ti].se;
        while(p<(int)anchors.size() && anchors[p].fi<=bj){ avail.push_back(p); ++p; }
        int pick=-1;
        while(!avail.empty()){ int idx=avail.back(); avail.pop_back(); if(!used[idx]){ pick=idx; break; } }
        if(pick==-1){
            int q=p; while(q<(int)anchors.size() && used[q]) ++q;
            if(q==(int)anchors.size()){ q=0; while(q<(int)anchors.size() && used[q]) ++q; }
            pick=q;
        }
        used[pick]=1; anchor_of[j]=anchors[pick].se;
    }

    vector<ll> S(M,0);
    vector<int> X(N,0);
    rep(j,M){ int id=anchor_of[j]; if(id>=0){ X[id]=j+1; S[j]+=A[id]; } }

    for(int k=(int)K-1;k>=0;--k){
        ll v=coin_val[k];
        vector<pair<ll,int>> needs; needs.reserve(M);
        rep(j,M){ needs.emplace_back(B[j]-S[j],j); }
        sort(rrng(needs));
        int ptr=0;
        rep(idx,M){
            if(ptr==(int)coin_ids[k].size()) break;
            ll need=needs[idx].fi; int j=needs[idx].se;
            if(need>=v){
                int id=coin_ids[k][ptr++];
                X[id]=j+1; S[j]+=v;
            }
        }
    }

    auto improve_bit = [&](int k){
        ll v=coin_val[k];
        vector<int> id_at(M,-1);
        vector<int> unused;
        for(int id: coin_ids[k]){
            if(X[id]>0) id_at[X[id]-1]=id; else unused.push_back(id);
        }
        while(true){
            vector<ll> r(M); rep(j,M) r[j]=S[j]-B[j];
            ll bestGain=0; int act=0, pidx=-1, qidx=-1;
            rep(j,M) if(id_at[j]!=-1){
                ll g = iabs(r[j]) - iabs(r[j]-v);
                if(g>bestGain){ bestGain=g; act=1; pidx=j; }
            }
            if(!unused.empty()){
                rep(j,M) if(id_at[j]==-1){
                    ll g = iabs(r[j]) - iabs(r[j]+v);
                    if(g>bestGain){ bestGain=g; act=2; qidx=j; }
                }
            }
            rep(pj,M) if(id_at[pj]!=-1){
                rep(qj,M) if(id_at[qj]==-1){
                    ll g = (iabs(r[pj]) - iabs(r[pj]-v)) + (iabs(r[qj]) - iabs(r[qj]+v));
                    if(g>bestGain){ bestGain=g; act=3; pidx=pj; qidx=qj; }
                }
            }
            if(bestGain<=0) break;
            if(act==1){
                int id=id_at[pidx]; id_at[pidx]=-1; X[id]=0; S[pidx]-=v; unused.push_back(id);
            }else if(act==2){
                int id=unused.back(); unused.pop_back(); id_at[qidx]=id; X[id]=qidx+1; S[qidx]+=v;
            }else{
                int id=id_at[pidx]; id_at[pidx]=-1; S[pidx]-=v;
                id_at[qidx]=id; X[id]=qidx+1; S[qidx]+=v;
            }
        }
    };

    rep(k,K) improve_bit(k);

    rep(i,N){ if(i) cout<<' '; cout<<X[i]; } cout<<'\n'<<flush;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    while(t--){
        solve();
    }
    return 0;
}
