#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <atcoder/all>

using namespace std;
using namespace atcoder;

/* -------------------------------------------------- */
using ll = long long;
#define rep(i,n) for(ll i=0;i<(ll)(n);++i)
#define all(v)   (v).begin(),(v).end()
/* -------------------------------------------------- */
struct Color{ double c,m,y; };
inline double dist2(const Color&a,const Color&b){
    double dc=a.c-b.c, dm=a.m-b.m, dy=a.y-b.y;
    return dc*dc+dm*dm+dy*dy;
}
/* ---- k-means ------------------------------------------------ */
static std::mt19937 rng(123456789);
pair<vector<int>,vector<Color>>
k_means_once(const vector<Color>& pts,int K,int max_iter=25){
    const int n=(int)pts.size();
    K=min(K,n);
    std::uniform_int_distribution<int> uni(0,n-1);
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
    rep(iter,max_iter){
        bool ch=false;
        rep(i,n){
            double best=1e100; int gid=0;
            rep(g,(int)cent.size()){
                double d=dist2(pts[i],cent[g]);
                if(d<best){best=d;gid=g;}
            }
            if(asg[i]!=gid){asg[i]=gid; ch=true;}
        }
        if(!ch) break;
        int KK=(int)cent.size();
        vector<double> sc(KK),sm(KK),sy(KK); vector<int> cnt(KK);
        rep(i,n){ int g=asg[i]; sc[g]+=pts[i].c; sm[g]+=pts[i].m; sy[g]+=pts[i].y; ++cnt[g]; }
        rep(g,KK){
            if(cnt[g]==0) cent[g]=pts[uni(rng)];
            else{
                cent[g].c=sc[g]/cnt[g]; cent[g].m=sm[g]/cnt[g]; cent[g].y=sy[g]/cnt[g];
            }
        }
    }
    return {asg,cent};
}
pair<vector<int>,vector<Color>>
k_means(const vector<Color>&pts,int K,int reps=5,int it=25){
    double best=1e100;
    pair<vector<int>,vector<Color>> ret;
    rep(r,reps){
        auto res=k_means_once(pts,K,it);
        double w=0;
        rep(i,pts.size()) w+=dist2(pts[i],res.second[res.first[i]]);
        if(w<best){best=w;ret=move(res);}
    }
    return ret;
}
/* ---- multiset enumeration ---------------------------------- */
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
/* ---- recipe ------------------------------------------------ */
struct Recipe{ Color col; vector<int> tubes; };
/* ---- util -------------------------------------------------- */
Color avg_color(const vector<int>& t,const vector<Color>& inks){
    Color c{0,0,0};
    for(int id:t){ c.c+=inks[id].c; c.m+=inks[id].m; c.y+=inks[id].y; }
    double d=(double)t.size();
    c.c/=d; c.m/=d; c.y/=d;
    return c;
}
/* ---- micro adjustment (at most +2 inks) -------------------- */
vector<int> micro_adjust(Color cur,int n,const vector<int>&base,
                         const Color&target,const vector<Color>&inks,
                         double D,ll opsSoFar,ll Tlimit,Color&out){
    vector<int> tubes=base;
    double err=dist2(cur,target);
    const int K=(int)inks.size();
    auto try_gain=[&](int ink){
        Color nxt{ (cur.c*n+inks[ink].c)/(n+1),
                   (cur.m*n+inks[ink].m)/(n+1),
                   (cur.y*n+inks[ink].y)/(n+1) };
        return make_pair(err-dist2(nxt,target),nxt);
    };
    const int MAX_ADD=2;
    rep(add,MAX_ADD){
        /* 余裕 1 注入 = +2 ops (注入+廃棄) */
        if(opsSoFar + (ll)tubes.size()*2 + 2 + 1 > Tlimit) break;
        double bestGain=0; int bestInk=-1; Color bestCol;
        rep(k,K){
            auto [gain,nxt]=try_gain(k);
            if(gain>bestGain){bestGain=gain; bestInk=k; bestCol=nxt;}
        }
        if(bestInk!=-1 && 1e4*bestGain > D){
            tubes.push_back(bestInk);
            cur=bestCol; ++n; err-=bestGain;
        }else break;
    }
    out=cur;
    return tubes;
}
/* ============================================================ */
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N,K,H,T; double D;
    if(!(cin>>N>>K>>H>>T>>D)) return 0;
    vector<Color> own(K); rep(i,K) cin>>own[i].c>>own[i].m>>own[i].y;
    vector<Color> tgt(H); rep(i,H) cin>>tgt[i].c>>tgt[i].m>>tgt[i].y;

    /* --- palette: no partitions (0) ------------------------ */
    rep(i,N){ rep(j,N-1) cout<<0<<(j+1==N-1?'\n':' '); }
    rep(i,N-1){ rep(j,N) cout<<0<<(j+1==N?'\n':' '); }

    /* --- enumerate recipes --------------------------------- */
    int max_inj=max(1,min<int>(6,T/(2*H)));
    vector<Recipe> recs;
    for(int s=1;s<=max_inj;++s){
        for(auto ms:gen_ms(K,s)){
            vector<int> uniq=ms; uniq.erase(unique(all(uniq)),uniq.end());
            if((int)uniq.size()>3) continue;
            recs.push_back({avg_color(ms,own),move(ms)});
        }
    }
    /*  必ず 1 つは存在させる (フェイルセーフ)  */
    if(recs.empty()) recs.push_back({own[0],{0}});

    /* --- automatic cluster count --------------------------- */
    int bestCL=1; long double bestEst=1e100;
    vector<int> cid_best; vector<Color> cent_best;
    const int CL_MAX=min(20,H);
    for(int kc=1; kc<=CL_MAX; ++kc){
        auto [cid,cent]=k_means(tgt,kc,4,25);
        vector<const Recipe*> repr(kc,nullptr);
        rep(c,kc){
            double best=1e100; const Recipe* br=nullptr;
            for(const Recipe&r:recs){
                double d=dist2(r.col,cent[c]);
                if(d<best){best=d;br=&r;}
            }
            repr[c]=br;
        }
        ll v=0; long double e=0;
        rep(i,H){ v+=repr[cid[i]]->tubes.size(); e+=dist2(repr[cid[i]]->col,tgt[i]); }
        long double est=1.0L + D*(long double)(v-H) + 1e4L*e;
        if(est<bestEst){bestEst=est; bestCL=kc; cid_best=move(cid); cent_best=move(cent);}
    }
    if(cid_best.empty()){ cid_best.assign(H,0); cent_best={avg_color(vector<int>{0},own)}; }
    /* --- 2nd-level sub-clustering --------------------------- */
    vector<const Recipe*> rep_of_sub;
    vector<int> subid(H,0); int subCnt=0;
    rep(c,bestCL){
        vector<int> idx;
        rep(i,H) if(cid_best[i]==c) idx.push_back(i);
        if(idx.empty()) continue;
        int SUBK=min(3,(int)idx.size());
        if(SUBK==1){
            double best=1e100; const Recipe* br=&recs[0];
            for(const Recipe&r:recs){
                double d=dist2(r.col,cent_best[c]);
                if(d<best){best=d;br=&r;}
            }
            for(int id:idx) subid[id]=subCnt;
            rep_of_sub.push_back(br); ++subCnt;
        }else{
            vector<Color> loc; for(int id:idx) loc.push_back(tgt[id]);
            auto [lcid,lcent]=k_means(loc,SUBK,4,25);
            vector<const Recipe*> repS(SUBK,nullptr);
            rep(sc,SUBK){
                double best=1e100; const Recipe* br=&recs[0];
                for(const Recipe&r:recs){
                    double d=dist2(r.col,lcent[sc]);
                    if(d<best){best=d;br=&r;}
                }
                repS[sc]=br;
            }
            rep(j,idx.size()){
                int sid=subCnt+lcid[j];
                subid[idx[j]]=sid;
            }
            rep(sc,SUBK) rep_of_sub.push_back(repS[sc]);
            subCnt+=SUBK;
        }
    }
    if(rep_of_sub.empty()) rep_of_sub.push_back(&recs[0]);  // 検証済みポインタ

    /* --- output operations --------------------------------- */
    const int X=0,Y=0;
    ll ops=0,Vinj=0; long double Esum=0;

    auto inject=[&](int k){ cout<<1<<' '<<X<<' '<<Y<<' '<<k<<'\n'; ++ops; ++Vinj; };
    auto extract=[&](){     cout<<2<<' '<<X<<' '<<Y<<'\n'; ++ops; };
    auto discard=[&](){     cout<<3<<' '<<X<<' '<<Y<<'\n'; ++ops; };

    rep(i,H){
        int remain = H - i;
        /* ---- decide maximum tubes affordable right now ---- */
        int slackOps = T - ops;                       // 残り許可手数
        int minOpsNeeded = 2 * remain;               // 各ターゲット最低 2 手
        if(slackOps < minOpsNeeded){                 // 極端な場合でも通す
            assert(false && "T が小さすぎて解が存在しません");
        }

        /* ---- D large → single-ink shortcut --------------- */
        if(D>=1000){
            double best=1e100; int ink=0;
            rep(k,K){ double d=dist2(own[k],tgt[i]); if(d<best){best=d;ink=k;} }
            inject(ink); extract();
            Esum += dist2(own[ink],tgt[i]);
            continue;
        }

        /* ---- normal path --------------------------------- */
        const Recipe* base=rep_of_sub[subid[i]];
        vector<int> tubes=base->tubes;
        int maxTubesAllowed = (T - ops - 2*(remain-1)) / 2; // 確保分を除く
        maxTubesAllowed = max(1,maxTubesAllowed);
        if((int)tubes.size()>maxTubesAllowed) tubes.resize(maxTubesAllowed);

        Color baseCol = avg_color(tubes,own);
        /* micro adjust within tightened limit (=T - 2*(remain-1)) */
        Color finalCol;
        tubes = micro_adjust(baseCol,(int)tubes.size(),tubes,
                             tgt[i],own,D,ops,T - 2*(remain-1),finalCol);

        /* 最後の安全ガード */
        while(ops + 2*(int)tubes.size() > T - 2*(remain-1))
            tubes.pop_back(), finalCol=avg_color(tubes,own);

        for(int id:tubes) inject(id);
        extract();
        rep(j,(int)tubes.size()-1) discard();

        Esum += dist2(finalCol,tgt[i]);
    }

    assert(ops<=T);

    long long errTerm = llround(1e4L*Esum);
    long double score = 1.0L + D*(long double)(Vinj - H) + (long double)errTerm;

    cerr<<fixed<<setprecision(0)
        <<"#V="<<Vinj<<' '
        <<"E="<<setprecision(6)<<(double)Esum<<' '<<setprecision(0)
        <<"score="<<score<<'\n';
    return 0;
}
