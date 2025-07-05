#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <atcoder/all>
using namespace std;
using namespace atcoder;

// ------------------------------------------------------------
// 型エイリアス & マクロ
// ------------------------------------------------------------
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (ll)(n); ++i)
#define rep2(i, a, b) for (ll i = (a); i < (ll)(b); ++i)
#define all(v) (v).begin(), (v).end()

// ------------------------------------------------------------
// 色表現
// ------------------------------------------------------------
struct Color {
    double c, m, y;
};
inline double dist2(const Color &a, const Color &b) {
    double dc = a.c - b.c, dm = a.m - b.m, dy = a.y - b.y;
    return dc * dc + dm * dm + dy * dy;
}

// ------------------------------------------------------------
// k‑means (Lloyd 法, k‑means++ 初期化)
// ------------------------------------------------------------
pair<vector<int>, vector<Color>> k_means(const vector<Color> &pts, int K, int max_iter = 50, int seed = 12345) {
    const int n = (int)pts.size();
    assert(K >= 1 && K <= n);

    mt19937 rng(seed);
    uniform_int_distribution<int> uni(0, n - 1);

    // k‑means++ 初期化
    vector<Color> cent; cent.reserve(K);
    cent.push_back(pts[uni(rng)]);
    vector<double> dists(n, 1e100);
    rep(i, 1){} // dummy to silence -Wunused-loop-variable
    while ((int)cent.size() < K) {
        double sum = 0;
        rep(i, n) {
            dists[i] = min(dists[i], dist2(pts[i], cent.back()));
            sum += dists[i];
        }
        uniform_real_distribution<double> ur(0, sum);
        double r = ur(rng);
        double acc = 0;
        int idx = 0;
        while (idx < n && (acc += dists[idx]) < r) ++idx;
        cent.push_back(pts[idx]);
    }

    vector<int> assign(n, -1);
    rep(iter, max_iter) {
        bool changed = false;
        // E step
        rep(i, n) {
            double best = 1e100; int gid = 0;
            rep(g, K) {
                double d = dist2(pts[i], cent[g]);
                if (d < best) best = d, gid = g;
            }
            if (assign[i] != gid) { assign[i] = gid; changed = true; }
        }
        if (!changed) break;
        // M step
        vector<double> sc(K), sm(K), sy(K); vector<int> cnt(K);
        rep(i, n) {
            int g = assign[i];
            sc[g] += pts[i].c; sm[g] += pts[i].m; sy[g] += pts[i].y; ++cnt[g];
        }
        rep(g, K) {
            if (cnt[g] == 0) cent[g] = pts[uni(rng)];
            else {
                cent[g].c = sc[g] / cnt[g];
                cent[g].m = sm[g] / cnt[g];
                cent[g].y = sy[g] / cnt[g];
            }
        }
    }
    return {assign, cent};
}

// ------------------------------------------------------------
// すべての多重集合 (重複あり) を列挙: サイズ up to S
// 生成される組はインデックス昇順 (<=) で重複注入も表現可
// 例: tubes=3, S=2 → {0},{1},{2},{0,0},{0,1},{0,2},{1,1},{1,2},{2,2}
// ------------------------------------------------------------
void dfs_comb(int K, int depth, int S, int last, vector<int>& cur, vector<vector<int>>& out){
    if(depth==S){return;} // will push inside
    rep2(i,last,K){
        cur.push_back(i);
        out.push_back(cur);
        dfs_comb(K,depth+1,S,i,cur,out);
        cur.pop_back();
    }
}

vector<vector<int>> generate_multisets(int K,int S){
    vector<vector<int>> res; vector<int> cur;
    dfs_comb(K,0,S,0,cur,res);
    return res;
}

// ------------------------------------------------------------
struct Recipe{
    Color col;              // 得られる色
    vector<int> tubes;      // 注入チューブ一覧 (重複あり)
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N,K,H,T; double D; if(!(cin>>N>>K>>H>>T>>D)) return 0;

    vector<Color> own(K); rep(i,K) cin>>own[i].c>>own[i].m>>own[i].y;
    vector<Color> tgt(H); rep(i,H) cin>>tgt[i].c>>tgt[i].m>>tgt[i].y;

    // 最大 1 ウェル (全部 0)
    rep(i,N){ rep(j,N-1) cout<<0<<(j+1==N-1?'\n':' ');} rep(i,N-1){ rep(j,N) cout<<0<<(j+1==N?'\n':' ');}    

    // ---------- 前計算: レシピ候補 ----------
    int max_inj = (int)min<ll>(4, T/(2*H)); // operations per target =2*inj <=T/H
    if(max_inj<1) max_inj=1;
    vector<vector<int>> multisets;
    for(int s=1;s<=max_inj;++s){
        auto v=generate_multisets(K,s);
        multisets.insert(multisets.end(),all(v));
    }
    vector<Recipe> recipes; recipes.reserve(multisets.size());
    for(auto &ms:multisets){
        Color c{0,0,0};
        for(int id:ms){ c.c+=own[id].c; c.m+=own[id].m; c.y+=own[id].y; }
        double s=ms.size(); c.c/=s; c.m/=s; c.y/=s;
        recipes.push_back({c,ms});
    }

    // ---------- k‑means でクラスタリング ----------
    int CL= (D<1000? min(10,H):0);
    vector<int> cid(H,0); vector<Color> cen;
    if(CL){ auto pr=k_means(tgt,CL); cid=move(pr.first); cen=move(pr.second);}    

    // ---------- 各クラスタに最良レシピ ----------
    vector<Recipe> best(CL);
    if(CL){
        rep(c,CL){ double besterr=1e100; Recipe bestR=recipes[0];
            for(const auto &r:recipes){ double d=dist2(r.col,cen[c]); if(d<besterr){besterr=d; bestR=r;} }
            best[c]=bestR; }
    }

    // ---------- 操作出力 ----------
    const int X=0,Y=0; ll ops=0;
    auto inject=[&](int k){ cout<<1<<' '<<X<<' '<<Y<<' '<<k<<'\n'; ++ops; };
    auto extract=[&](){ cout<<2<<' '<<X<<' '<<Y<<'\n'; ++ops; };
    auto discard=[&](){ cout<<3<<' '<<X<<' '<<Y<<'\n'; ++ops; };

    rep(i,H){
        if(D>=1000){ // 単色最良
            double best=1e100; int id=0; rep(kx,K){ double d=dist2(own[kx],tgt[i]); if(d<best){best=d; id=kx;} }
            inject(id); extract();
        }else{
            const Recipe &r=best[cid[i]];
            for(int id: r.tubes) inject(id);
            extract();
            rep(j,(int)r.tubes.size()-1) discard();
        }
    }

    assert(ops<=T);
    return 0;
}
