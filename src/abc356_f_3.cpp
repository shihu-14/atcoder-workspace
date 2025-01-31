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
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 7001002003004005006ll;
const int INF = 1001001001;

template<typename T=int>
struct CC {
    bool initialized;
    vector<T> xs;
    CC(): initialized(false) {}
    void add(T x) { xs.push_back(x);}
    void init() {
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(),xs.end()),xs.end());
        initialized = true;
    }
    int operator()(T x) {
        if (!initialized) init();
        return lower_bound(xs.begin(), xs.end(), x) - xs.begin();
    }
    T operator[](int i) {
        if (!initialized) init();
        return xs[i];
    }
    int size() {
        if (!initialized) init();
        return xs.size();
    }
};

using S=pair<int,int>;
S op(S x,S y){return {x.first+y.first,x.second+y.second};}
S e(){return {0,0};}
bool f(S x){return x.first==0;}
bool g(S x){return x.first==0;}
bool h(S x){return x.first==x.second;}

int main(){
	int q;
	ll k;
	cin >> q >> k;
	vector query(q, vector<int>(2));
    CC<ll> ss;
	rep(i, q) cin >> query[i][0] >> query[i][1];
    rep(i, q) ss.add(query[i][1]);
    ss.add(-LINF), ss.add(LINF);

	vector<int>r(ss.size());
	// r[i]=min{x | ss[x]>ss[i]+k}
	int n=ss.size();
	rep(i, n)r[i]=ss(ss[i]+k+1);
	r[0]=0;
	
	atcoder::segtree<S,op,e>seg(n);
	seg.set(0,{1,0});
	seg.set(n-1,{1,0});

	auto next=[&](int ii){
		//ii以降
		return seg.max_right<f>(ii);
	};
	auto prev=[&](int ii){
		//iiより前
		return seg.min_left<g>(ii)-1;
	};
	
	rep(i, q){
		int ii=ss(query[i][1]);
		if(query[i][0]==1){
			if(seg.get(ii).first){
				//del
				seg.set(ii,{0,0});
				int p=prev(ii);
				int n=next(ii);
				if(n<r[p])seg.set(p,{1,1});
				else seg.set(p,{1,0});
			}else{
				//add
				int p=prev(ii);
				int n=next(ii);
				if(n<r[ii])seg.set(ii,{1,1});
				else seg.set(ii,{1,0});
				if(ii<r[p])seg.set(p,{1,1});
				else seg.set(p,{1,0});
			}
		}else{
			int l=seg.min_left<h>(ii);
			int r=seg.max_right<h>(ii);
			cout << seg.prod(l,r).first+1 << endl;
		}
	}
}
