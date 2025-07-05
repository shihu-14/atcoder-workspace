/*************************************************
 *  AHC048 – single-well, rational-ratio recipes
 *  clang++ -std=c++17 -O2 でビルド可
 *************************************************/
#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <cassert>

using namespace std;

/* ---------- 基本型 ---------- */
using ll = long long;
#define rep(i,n) for (ll i = 0; i < (ll)(n); ++i)
#define all(v)   (v).begin(), (v).end()

/* ---------- 色ユーティリティ ---------- */
struct Col{ double c,m,y; };
inline double dist2(const Col& a,const Col& b){
    double dc=a.c-b.c, dm=a.m-b.m, dy=a.y-b.y;
    return dc*dc + dm*dm + dy*dy;
}
inline Col operator+(const Col& a,const Col& b){ return {a.c+b.c, a.m+b.m, a.y+b.y}; }
inline Col operator*(const Col& a,double k){ return {a.c*k, a.m*k, a.y*k}; }
inline Col operator/(const Col& a,double d){ return {a.c/d, a.m/d, a.y/d}; }

/* ---------- Recipe ---------- */
struct Recipe{
    Col           col;      // 平均色
    vector<int>   tubes;    // multiset (昇順) |tubes| = shots
};

/* ---------- 重み付き平均 ---------- */
Col avg_weighted(const vector<pair<int,int>>& w,
                 const vector<Col>& ink,int shots){
    Col sum{0,0,0};
    for(auto [id,cnt]:w) sum = sum + ink[id]*cnt;
    return sum/(double)shots;
}

/* ============================================================= */
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* ---- input ---- */
    int N,K,H,T; double D;
    if(!(cin>>N>>K>>H>>T>>D)) return 0;

    vector<Col> ink(K), tgt(H);
    rep(i,K) cin>>ink[i].c>>ink[i].m>>ink[i].y;
    rep(i,H) cin>>tgt[i].c>>tgt[i].m>>tgt[i].y;

    /* ---- palette (single well) ---- */
    rep(i,N){ rep(j,N-1) cout<<0<<(j+1==N-1?'\n':' '); }
    rep(i,N-1){ rep(j,N) cout<<0<<(j+1==N?'\n':' '); }

    /* ---- recipe enumeration ---- */
    const int MAX_SHOTS = min(6, T/(2*H));
    vector<Recipe> recs;
    unordered_set<string> seen; seen.reserve(40000);

    auto add_rec=[&](const vector<pair<int,int>>& w,int shots){
        string key;
        for(auto [id,cnt]:w){ key+=char(id); key+=':'; key+=char(cnt); key+=','; }
        if(!seen.insert(key).second) return;
        vector<int> tubes;
        for(auto [id,cnt]:w) tubes.insert(tubes.end(),cnt,id);
        sort(all(tubes));
        recs.push_back({avg_weighted(w,ink,shots), move(tubes)});
    };

    /* 1-color recipes */
    for(int a=0;a<K;++a) add_rec({{a,1}},1);

    /* 2-color recipes */
    for(int a=0;a<K;++a) for(int b=a+1;b<K;++b)
        for(int w1=1;w1<=4;++w1)
        for(int w2=1;w2<=4;++w2){
            int s=w1+w2; if(s>MAX_SHOTS) continue;
            add_rec({{a,w1},{b,w2}},s);
        }

    /* 3-color recipes */
    for(int a=0;a<K;++a) for(int b=a+1;b<K;++b) for(int c=b+1;c<K;++c)
        for(int w1=1;w1<=4;++w1)
        for(int w2=1;w2<=4;++w2)
        for(int w3=1;w3<=4;++w3){
            int s=w1+w2+w3; if(s>MAX_SHOTS) continue;
            add_rec({{a,w1},{b,w2},{c,w3}},s);
        }

    if(recs.empty()) recs.push_back({ink[0],{0}});   // safety

    /* ---- main loop ---- */
    const int X=0,Y=0;          // single well
    ll ops=0,Vinj=0; long double Esum=0;
    const long double C_DIST=1e4;

    auto inject =[&](int k){ cout<<1<<' '<<X<<' '<<Y<<' '<<k<<'\n'; ++ops; ++Vinj; };
    auto extract=[&](){        cout<<2<<' '<<X<<' '<<Y<<'\n';       ++ops;        };
    auto discard=[&](){        cout<<3<<' '<<X<<' '<<Y<<'\n';       ++ops;        };

    rep(idx,H){
        int remain=H-idx-1;
        ll min_future=2LL*remain;
        ll left=T-ops;

        const Recipe* best=&recs[0];
        long double best_val=1e100;

        for(const auto& r:recs){
            int s=r.tubes.size();
            ll need=2LL*s;
            if(left-need<min_future) continue;
            long double v=D*(long double)(s-1)+C_DIST*dist2(r.col,tgt[idx]);
            if(v<best_val){best_val=v; best=&r;}
        }

        if(left-2LL<min_future){          // fallback single ink
            double bestd=1e100; int id=0;
            rep(k,K){ double d=dist2(ink[k],tgt[idx]); if(d<bestd){bestd=d; id=k;} }
            inject(id); extract(); Esum+=bestd;
            continue;
        }

        for(int id:best->tubes) inject(id);
        extract();
        rep(j,(int)best->tubes.size()-1) discard();

        Esum+=dist2(best->col,tgt[idx]);
    }

    assert(ops<=T && "operation limit exceeded");

    long long err=llround(C_DIST*Esum);
    long double score=1.0L + D*(long double)(Vinj-H) + (long double)err;

    cerr<<fixed<<setprecision(0)
        <<"#V="<<Vinj<<' '<<"E="<<setprecision(6)<<(double)Esum<<' '
        <<setprecision(0)<<"score="<<score<<'\n';
    return 0;
}
