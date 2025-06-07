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

    
    constexpr int N = 36;              // #strings
    constexpr int M = 12;              // #states
    constexpr int MINLEN = 6;          // |S_i|≥6
    constexpr int MAXLEN = 12;         // state upper bound
    using ll = long long;
    
    struct RNG {                       // 64-bit xorshift
        uint64_t x=88172645463325252ULL;
        inline uint32_t next_u32(){
            x ^= x<<7;  x ^= x>>9; return uint32_t(x);
        }
        inline int randint(int l,int r){return int(next_u32()%(r-l+1))+l;}
        inline double rand01(){return double(next_u32())/double(0xffffffffu);}
    } rng;
    
    int main(){
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
    
        int n, dm, L_unused;
        if(!(cin>>n>>dm>>L_unused)) return 0;
    
        vector<string> S(n);
        vector<int> P(n);
        for(int i=0;i<n;i++) cin>>S[i]>>P[i];
    
        /* ---------- evaluation ---------- */
        auto score_of = [&](const string& w){
            string circ = w + w;       // circular
            int tot = 0;
            for(int i=0;i<n;i++)
                if(circ.find(S[i])!=string::npos) tot += P[i];
            return tot;
        };
    
        /* ---------- initial w : greedy shortest superstring of top-4 ---------- */
        vector<int> ord(n); iota(ord.begin(),ord.end(),0);
        sort(ord.begin(),ord.end(),[&](int a,int b){return P[a]>P[b];});
        string w = S[ord[0]];
        auto merge = [](string a,string b){
            if(a.find(b)!=string::npos) return a;
            if(b.find(a)!=string::npos) return b;
            int la=a.size(), lb=b.size();
            for(int k=min(la,lb);k;k--)
                if(a.substr(la-k)==b.substr(0,k)) return a+b.substr(k);
            for(int k=min(la,lb);k;k--)
                if(b.substr(lb-k)==a.substr(0,k)) return b+a.substr(k);
            return a+b;
        };
        for(int k=1;k<4;k++){
            w = merge(w,S[ord[k]]);
            if((int)w.size()>MAXLEN){ w.resize(MAXLEN); break; }
        }
        if((int)w.size()<MINLEN){
            while((int)w.size()<MINLEN) w.push_back('a'+rng.randint(0,5));
        }
    
        int curScore = score_of(w);
        int bestScore = curScore;
        string bestW = w;
    
        /* ---------- simulated annealing ( ≈1.9 s ) ---------- */
        auto start = chrono::steady_clock::now();
        const double TIMELIM = 1.9;                    // seconds
        const double T0 = 120.0, T_end = 1.0;          // temperature range
    
        long long iter = 0;
        while(true){
            double t = chrono::duration<double>(chrono::steady_clock::now()-start).count();
            if(t > TIMELIM) break;
            double progress = t / TIMELIM;
            double Temp = T0 + (T_end - T0)*progress;
    
            string nxt = w;
            int op = rng.randint(0,2);
            if(op==0){                                 // change a character
                int pos = rng.randint(0,(int)nxt.size()-1);
                char old = nxt[pos];
                char nc = 'a' + rng.randint(0,5);
                if(nc==old) nc = 'a' + ((nc-'a'+1)%6);
                nxt[pos] = nc;
            }else if(op==1){                           // insert / delete
                if((int)nxt.size()<MAXLEN && rng.randint(0,1)){
                    int pos = rng.randint(0,(int)nxt.size());
                    char nc = 'a' + rng.randint(0,5);
                    nxt.insert(nxt.begin()+pos,nc);
                }else if((int)nxt.size()>MINLEN){
                    int pos = rng.randint(0,(int)nxt.size()-1);
                    nxt.erase(nxt.begin()+pos);
                }
            }else{                                     // swap two positions
                if(nxt.size()>=2){
                    int i = rng.randint(0,(int)nxt.size()-1);
                    int j = rng.randint(0,(int)nxt.size()-1);
                    if(i!=j) swap(nxt[i],nxt[j]);
                }
            }
    
            int nxtScore = score_of(nxt);
            int diff = nxtScore - curScore;
            if(diff >= 0 || rng.rand01() < exp(diff/Temp)){
                w.swap(nxt); curScore = nxtScore;
                if(curScore > bestScore){
                    bestScore = curScore;
                    bestW = w;
                }
            }
            ++iter;
        }
        // cerr<<"iter="<<iter<<" best="<<bestScore<<"\n";
    
        /* ---------- output model ---------- */
        int m = (int)bestW.size();
        array<char,M> C;
        vector<vector<int>> A(M,vector<int>(M,0));
        for(int i=0;i<M;i++) C[i]='a';                 // default
    
        for(int i=0;i<m;i++){
            C[i]=bestW[i];
            A[i][(i+1)%m]=100;
        }
        for(int i=m;i<M;i++){
            A[i][i]=100;                               // sink
        }
    
        for(int i=0;i<M;i++){
            cout<<C[i];
            for(int j=0;j<M;j++) cout<<' '<<A[i][j];
            cout<<'\n';
        }
        return 0;
    }
    