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

const int N = 36, M = 12;
const ll  L = 1e9;

/* overlap-merge */
string mg(const string& a,const string& b){
    if (a.find(b)!=string::npos) return a;
    if (b.find(a)!=string::npos) return b;
    int la=a.size(), lb=b.size();
    for(int k=min(la,lb);k>=1;--k)
        if(a.substr(la-k)==b.substr(0,k)) return a+b.substr(k);
    for(int k=min(la,lb);k>=1;--k)
        if(b.substr(lb-k)==a.substr(0,k)) return b+a.substr(k);
    return a+b;
}

/* expected score (opt grad optional) */
double ev(const vector<string>& s,const vector<int>& p,
          const vector<char>& hit,const vector<double>& q,
          vector<double>* g=nullptr){
    double e=0;
    if(g) g->assign(6,0);
    rep(i,N){
        if(hit[i]) continue;
        vector<int> cnt(6,0);
        for(char c:s[i]) cnt[c-'a']++;
        double lg=0;
        rep(c,6) if(cnt[c]) lg+=cnt[c]*log(q[c]);
        double pr=exp(lg);
        double lam=(L-s[i].size()+1)*pr;
        double qi=1-exp(-lam);
        e+=p[i]*qi;
        if(g){
            double f=p[i]*exp(-lam)*(L-s[i].size()+1)*pr;
            rep(c,6) if(cnt[c]) (*g)[c]+=f*cnt[c]/q[c];
        }
    }
    return e;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,d1,d2; cin>>n>>d1>>d2;

    vector<string> s(N);
    vector<int>    p(N);
    rep(i,N) cin>>s[i]>>p[i];

    vector<int> id(N); iota(rng(id),0);
    sort(rng(id),[&](int a,int b){return p[a]>p[b];});

    string w=s[id[0]];
    int bs=0;
    {
        string w2=w+w;
        rep(i,N) if(w2.find(s[i])!=string::npos) bs+=p[i];
    }

    rep(i,min(N,10)) for(int j=i+1;j<min(N,10);++j){
        string cw=mg(s[id[i]],s[id[j]]);
        if(cw.size()>M) continue;
        string w2=cw+cw;
        int sc=0;
        rep(k,N) if(w2.find(s[k])!=string::npos) sc+=p[k];
        if(sc>bs){ bs=sc; w=cw; }
    }

    int lw=w.size();
    vector<char> hit(N,0);
    {
        string w2=w+w;
        rep(i,N) if(w2.find(s[i])!=string::npos) hit[i]=1;
    }

    int rem=M-lw;
    vector<double> q(6,1.0/6), m(6), g;
    double be=ev(s,p,hit,q);
    const int IT=200;
    rep(t,IT){
        ev(s,p,hit,q,&g);
        double lr=0.1*(1.0-double(t)/IT)+0.01;
        rep(c,6){
            m[c]=0.8*m[c]+0.2*g[c];
            q[c]=max(1e-9,q[c]+lr*m[c]);
        }
        double sm=accumulate(rng(q),0.0);
        rep(c,6) q[c]/=sm;
    }

    vector<vector<int>> a(M,vector<int>(M,0));
    rep(i,lw) a[i][(i+1)%lw]=100;

    vector<pair<double,int>> fr;
    rep(c,6) fr.push_back({rem*q[c],c});
    vector<int> cnt(6,0); int sum=0;
    rep(c,6){ cnt[c]=floor(fr[c].first); sum+=cnt[c]; }
    sort(rng(fr),[&](auto& x,auto& y){return x.first-floor(x.first)>y.first-floor(y.first);});
    rep(k,rem-sum) cnt[fr[k].second]++;

    int pos=lw;
    rep(c,6) rep(i,cnt[c]){
        if(pos<M){ w.push_back('a'+c); a[pos][pos]=100; ++pos; }
    }
    while(int(w.size())<M){ w.push_back('a'); a[pos][pos]=100; ++pos; }

    rep(i,M){
        cout<<w[i];
        rep(j,M) cout<<' '<<a[i][j];
        cout<<'\n';
    }
    return 0;
}
