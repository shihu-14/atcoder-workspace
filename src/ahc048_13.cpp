#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <random>
#include <algorithm>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <atcoder/all>

using namespace std;
using namespace atcoder;

/*------------------ 基本型 ------------------*/
using ll = long long;
#define rep(i,n) for (ll i=0;i<(ll)(n);++i)
#define all(v)   (v).begin(),(v).end()

/*------------------ 色ユーティリティ ------------------*/
struct Color{ double c,m,y; };
inline double dist2(const Color& a,const Color& b){
    double dc=a.c-b.c , dm=a.m-b.m , dy=a.y-b.y;
    return dc*dc + dm*dm + dy*dy;
}

/*------------------ 全 multiset 列挙 ------------------*/
void dfs_ms(int K,int S,int last,vector<int>&cur,vector<vector<int>>&out){
    if((int)cur.size()==S) return;
    for(int i=last;i<K;++i){
        cur.push_back(i);
        out.push_back(cur);
        dfs_ms(K,S,i,cur,out);
        cur.pop_back();
    }
}
vector<vector<int>> gen_ms(int K,int S){
    vector<vector<int>> res; vector<int> cur; dfs_ms(K,S,0,cur,res); return res;
}

/*------------------ レシピ ------------------*/
struct Recipe{ Color col; vector<int> tubes; };

Color average(const vector<int>& t,const vector<Color>& ink){
    Color c{0,0,0};
    for(int id:t){ c.c+=ink[id].c; c.m+=ink[id].m; c.y+=ink[id].y; }
    double d = max<int>(1,(int)t.size());
    c.c/=d; c.m/=d; c.y/=d;
    return c;
}

/*======================================================*/
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* ---------- 入力 ---------- */
    int N,K,H,T; double D;
    if(!(cin>>N>>K>>H>>T>>D)) return 0;
    vector<Color> ink(K), tgt(H);
    rep(i,K) cin>>ink[i].c>>ink[i].m>>ink[i].y;
    rep(i,H) cin>>tgt[i].c>>tgt[i].m>>tgt[i].y;

    /* ---------- パレット : 全 0 で 1 大ウェル ---------- */
    rep(i,N){ rep(j,N-1) cout<<0<<(j+1==N-1?'\n':' '); }
    rep(i,N-1){ rep(j,N) cout<<0<<(j+1==N?'\n':' '); }

    /* ---------- レシピ全集合 (≤6 shot, uniq ≤3) ---------- */
    const int MAX_SHOT = min(6, T/(2*H));          // 操作上限
    vector<Recipe> recs;
    for(int s=1;s<=MAX_SHOT;++s){
        for(auto ms: gen_ms(K,s)){
            vector<int> uniq = ms;
            uniq.erase(unique(all(uniq)), uniq.end());
            if((int)uniq.size()>3) continue;
            recs.push_back({ average(ms,ink), move(ms) });
        }
    }
    if(recs.empty()) recs.push_back({ink[0],{0}}); // フェイルセーフ

    /* ---------- ループ本体 ---------- */
    const int X=0,Y=0;
    ll ops=0, Vinj=0; long double Esum=0;

    auto inject =[&](int k){ cout<<1<<' '<<X<<' '<<Y<<' '<<k<<'\n'; ++ops; ++Vinj; };
    auto extract=[&](){     cout<<2<<' '<<X<<' '<<Y<<'\n'; ++ops; };
    auto discard=[&](){     cout<<3<<' '<<X<<' '<<Y<<'\n'; ++ops; };

    rep(i,H){
        int remain = H - i - 1;
        ll  ops_min_future = 2LL * remain;
        ll  ops_left       = T - ops;

        const Recipe* best = &recs[0];
        long double   bestScore = 1e100;

        for(const Recipe& r: recs){
            int cnt = (int)r.tubes.size();
            ll need_ops = 2LL * cnt;             // inj+disc+extract
            if(ops_left - need_ops < ops_min_future) continue;

            long double sc = D * (long double)(cnt - 1)
                            + 1e4L * dist2(r.col, tgt[i]);
            if(sc < bestScore){ bestScore = sc; best = &r; }
        }

        int cnt = (int)best->tubes.size();
        if(ops_left - 2LL*cnt < ops_min_future){
            /* 超過しそうなら単色フォールバック */
            double be=1e100; int bid=0;
            rep(k,K){ double d=dist2(ink[k],tgt[i]); if(d<be){be=d; bid=k;} }
            inject(bid); extract();
            Esum += be;
            continue;
        }

        /* 注入 → 抽出 → 廃棄 */
        for(int id: best->tubes) inject(id);
        extract();
        rep(j,cnt-1) discard();

        Esum += dist2(best->col, tgt[i]);
    }

    assert(ops <= T && "operation limit exceeded");

    long long   errTerm = llround(1e4L * Esum);
    long double score   = 1.0L + D * (long double)(Vinj - H) + (long double)errTerm;

    cerr<<fixed<<setprecision(0)
        <<"#V="<<Vinj<<' '
        <<"E="<<setprecision(6)<<(double)Esum<<' '
        <<setprecision(0)<<"score="<<score<<'\n';
    return 0;
}
