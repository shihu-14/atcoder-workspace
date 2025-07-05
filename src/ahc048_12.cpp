#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cfloat>
#include <limits>
#include <utility>
#include <atcoder/all>

using namespace std;
using namespace atcoder;

/* ---------- 基本型 ---------- */
using ll = long long;
#define rep(i,n) for (ll i=0;i<(ll)(n);++i)
#define all(v)   (v).begin(),(v).end()

/* ---------- 色 & 二乗距離 ---------- */
struct Color{ double c,m,y; };
inline double dist2(const Color&a,const Color&b){
    double dc=a.c-b.c, dm=a.m-b.m, dy=a.y-b.y;
    return dc*dc + dm*dm + dy*dy;
}

/* ---------- k-means(++ 初期, multi-start) ---------- */
static std::mt19937 rng(123456789);

pair<vector<int>,vector<Color>>
k_means_once(const vector<Color>& pts,int K,int it=25){
    const int n=(int)pts.size();
    K = min(K,n);
    std::uniform_int_distribution<int> uni(0,n-1);

    /* ++ 初期化 */
    vector<Color> cent; cent.reserve(K);
    vector<double> d2(n,1e100);
    cent.push_back(pts[uni(rng)]);
    while((int)cent.size()<K){
        double sum=0;
        rep(i,n){ d2[i]=min(d2[i],dist2(pts[i],cent.back())); sum+=d2[i]; }
        if(sum==0) break;
        std::uniform_real_distribution<double> ur(0.0,sum);
        double r=ur(rng),acc=0; int id=0;
        while(id<n && (acc+=d2[id])<r) ++id;
        cent.push_back(pts[id]);
    }

    vector<int> asg(n,-1);
    rep(loop,it){
        bool upd=false;
        rep(i,n){
            double best=1e100; int gid=0;
            rep(g,cent.size()){
                double d=dist2(pts[i],cent[g]);
                if(d<best){best=d; gid=g;}
            }
            if(asg[i]!=gid){asg[i]=gid; upd=true;}
        }
        if(!upd) break;

        const int k=cent.size();
        vector<double> sc(k),sm(k),sy(k); vector<int> cnt(k);
        rep(i,n){ int g=asg[i]; sc[g]+=pts[i].c; sm[g]+=pts[i].m; sy[g]+=pts[i].y; ++cnt[g]; }
        rep(g,k){
            if(cnt[g]==0) cent[g]=pts[uni(rng)];
            else{
                cent[g].c=sc[g]/cnt[g]; cent[g].m=sm[g]/cnt[g]; cent[g].y=sy[g]/cnt[g];
            }
        }
    }
    return {asg,cent};
}

pair<vector<int>,vector<Color>>
k_means(const vector<Color>& pts,int K,int reps=6,int it=25){
    double best=1e100;
    pair<vector<int>,vector<Color>> ret;
    rep(r,reps){
        auto res=k_means_once(pts,K,it);
        double w=0; rep(i,pts.size()) w+=dist2(pts[i],res.second[res.first[i]]);
        if(w<best){best=w; ret=move(res);}
    }
    return ret;
}

/* ---------- 多重集合列挙 (≤6 shot, ≤3 色) ---------- */
void dfs_ms(int K,int S,int last,vector<int>&cur,vector<vector<int>>&out){
    if((int)cur.size()==S) return;
    for(int i=last;i<K;++i){
        cur.push_back(i); out.push_back(cur);
        dfs_ms(K,S,i,cur,out); cur.pop_back();
    }
}
vector<vector<int>> gen_ms(int K,int S){
    vector<vector<int>> res; vector<int> cur; dfs_ms(K,S,0,cur,res); return res;
}

/* ---------- Recipe ---------- */
struct Recipe{ Color col; vector<int> tubes; };

Color avg_col(const vector<int>& t,const vector<Color>& inks){
    if(t.empty()) return inks[0];              // フェイルセーフ
    Color c{0,0,0};
    for(int id:t){ c.c+=inks[id].c; c.m+=inks[id].m; c.y+=inks[id].y; }
    double d=(double)t.size();
    c.c/=d; c.m/=d; c.y/=d;
    return c;
}

/* ---------- 微調整 (追加 ≤3 本) ----------
   - 追加本数だけを返し、Vinj は呼び出し側でまとめて更新 */
int micro_adjust(Color cur,int n,vector<int>&tubes,
                 const Color& tgt,const vector<Color>&inks,
                 double D,ll ops,ll Tlim,Color& out){
    const int K=inks.size();
    const int EXTRA=3;
    double err=dist2(cur,tgt);
    auto gain=[&](int k){
        Color nx{ (cur.c*n+inks[k].c)/(n+1),
                  (cur.m*n+inks[k].m)/(n+1),
                  (cur.y*n+inks[k].y)/(n+1) };
        return make_pair(err-dist2(nx,tgt),nx);
    };

    int added=0;
    rep(step,EXTRA){
        /* 残り手数を保守的にチェック (最悪 2 ops/注入+抽出1) */
        if(ops + (ll)tubes.size()*2 + 3 > Tlim) break;
        double best=0; int bestId=-1; Color bestCol;
        rep(k,K){
            auto [g,nx]=gain(k);
            if(g>best){best=g; bestId=k; bestCol=nx;}
        }
        if(bestId!=-1 && 1e4*best > D){
            tubes.push_back(bestId);
            cur=bestCol; ++n; err-=best; ++added;
        }else break;
    }
    out=cur;
    return added;
}

/* ============================================================ */
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* -------- 入力 -------- */
    int N,K,H,T; double D;
    if(!(cin>>N>>K>>H>>T>>D)) return 0;
    vector<Color> own(K); rep(i,K) cin>>own[i].c>>own[i].m>>own[i].y;
    vector<Color> tgt(H); rep(i,H) cin>>tgt[i].c>>tgt[i].m>>tgt[i].y;

    /* -------- パレット仕切り (全 0 で安全) -------- */
    rep(i,N){ rep(j,N-1) cout<<0<<(j+1==N-1?'\n':' '); }
    rep(i,N-1){ rep(j,N) cout<<0<<(j+1==N?'\n':' '); }

    /* -------- レシピ列挙 -------- */
    const int MAX_SHOT=min(6, T/(2*H));
    vector<Recipe> recs;
    for(int s=1;s<=MAX_SHOT;++s){
        for(auto ms:gen_ms(K,s)){
            vector<int> uniq=ms; uniq.erase(unique(all(uniq)),uniq.end());
            if((int)uniq.size()>3) continue;
            recs.push_back({avg_col(ms,own),move(ms)});
        }
    }
    if(recs.empty()) recs.push_back({own[0],{0}});        // 保険

    /* -------- クラスタ数自動探索 -------- */
    int bestCL=1; long double bestEst=LDBL_MAX;
    vector<int> cidBest; vector<Color> centBest;
    const int CLMAX=min(20,H);
    rep(kc,CLMAX){
        auto [cid,cent]=k_means(tgt,kc+1,4,25);
        vector<const Recipe*> repr(kc+1,nullptr);
        rep(c,kc+1){
            double be=1e100; const Recipe* br=nullptr;
            for(const auto&r:recs){
                double d=dist2(r.col,cent[c]);
                if(d<be){be=d;br=&r;}
            }
            repr[c]=br;
        }
        ll v=0; long double e=0;
        rep(i,H){ v+=repr[cid[i]]->tubes.size(); e+=dist2(repr[cid[i]]->col,tgt[i]); }
        long double est=1.0 + D*(long double)(v-H) + 1e4L*e;
        if(est<bestEst){
            bestEst=est; bestCL=kc+1;
            cidBest=move(cid); centBest=move(cent);
        }
    }

    /* -------- サブクラスタ (≤3) -------- */
    vector<const Recipe*> repSub;
    vector<int> subid(H,0); int subCnt=0;
    rep(c,bestCL){
        vector<int> idx; rep(i,H) if(cidBest[i]==c) idx.push_back(i);
        if(idx.empty()) continue;
        int SUBK=min(3,(int)idx.size());
        if(SUBK==1){
            double be=1e100; const Recipe* br=&recs[0];
            for(const auto&r:recs){
                double d=dist2(r.col,centBest[c]);
                if(d<be){be=d;br=&r;}
            }
            for(int id:idx) subid[id]=subCnt;
            repSub.push_back(br); ++subCnt;
        }else{
            vector<Color> loc; for(int id:idx) loc.push_back(tgt[id]);
            auto [lcid,lcent]=k_means(loc,SUBK,4,25);
            vector<const Recipe*> repS(SUBK,nullptr);
            rep(sc,SUBK){
                double be=1e100; const Recipe* br=&recs[0];
                for(const auto&r:recs){
                    double d=dist2(r.col,lcent[sc]);
                    if(d<be){be=d;br=&r;}
                }
                repS[sc]=br;
            }
            rep(j,idx.size()) subid[idx[j]]=subCnt+lcid[j];
            rep(sc,SUBK) repSub.push_back(repS[sc]);
            subCnt+=SUBK;
        }
    }
    if(repSub.empty()) repSub.push_back(&recs[0]);         // フォールバック

    /* -------- 操作出力ループ -------- */
    const int X=0,Y=0;
    ll ops=0, Vinj=0; long double Esum=0;

    auto inject=[&](int k){ cout<<1<<' '<<X<<' '<<Y<<' '<<k<<'\n'; ++ops; ++Vinj; };
    auto extract=[&](){     cout<<2<<' '<<X<<' '<<Y<<'\n'; ++ops; };
    auto discard=[&](){     cout<<3<<' '<<X<<' '<<Y<<'\n'; ++ops; };

    rep(i,H){
        const int remain = H - i - 1;

        /* --- D が非常に大きいときは単色近似 --- */
        if(D >= 1000){
            double be=1e100; int id=0;
            rep(k,K){ double d=dist2(own[k],tgt[i]); if(d<be){be=d; id=k;} }
            inject(id); extract(); discard();        // 毎回捨てて浄化
            Esum += dist2(own[id], tgt[i]);
            continue;
        }

        /* --- 基準レシピ & チューブ列 --- */
        const Recipe* base = repSub[subid[i]];
        vector<int> tubes = base->tubes;

        /* --- 予算に合わせてチューブ数を制限 --- */
        int slack = T - ops;
        int maxTube = (slack - 1 - 2*remain) / 2;   // inj+disc で2, extract1
        maxTube = max(1,maxTube);
        if((int)tubes.size() > maxTube) tubes.resize(maxTube);

        /* --- 微調整 (追加 ≤3) --- */
        Color cur = avg_col(tubes,own), fin;
        int add = micro_adjust(cur,(int)tubes.size(),tubes,
                               tgt[i],own,D,ops,T-2*remain,fin);
        /* Vinj は後でまとめて加算 → add は無視 */

        /* --- 最終安全チェック：手数を確保 --- */
        while(ops + (ll)tubes.size()*2 + 1 > T - 2*remain)
            tubes.pop_back(), fin = avg_col(tubes,own);

        /* --- 実注入 --- */
        for(int id:tubes) inject(id);
        extract();
        rep(j,(int)tubes.size()-1) discard();       // 浄化
        Esum += dist2(fin, tgt[i]);
    }

    assert(ops <= T && "operation budget exceeded");

    long long errTerm = llround(1e4L * Esum);
    long double score = 1.0L + D * (long double)(Vinj - H) + (long double)errTerm;

    cerr << fixed << setprecision(0)
         << "#V=" << Vinj << ' '
         << "E=" << setprecision(6) << (double)Esum << ' '
         << setprecision(0) << "score=" << score << '\n';
    return 0;
}
