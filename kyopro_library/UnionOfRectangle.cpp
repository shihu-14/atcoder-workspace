struct Rect{
    int l1, r1, l2, r2;
    Rect(int l1=0, int r1=0, int l2=0, int r2=0): l1(l1), r1(r1), l2(l2), r2(r2) {}
};
namespace UnionOfRectangle{
    pii op(pii a, pii b){
        if (a.fi == b.fi) return {a.fi, a.se+b.se};
        else return min(a, b);
    }
    pii e(){ return {0, 0};}
    pii mapping(int f, pii x){
        x.fi += f;
        return x;
    }
    int composition(int f, int g){return f+g;}
    int id(){return 0;}
    struct Query{
        int r1, r2, c;
        Query(int r1=0, int r2=0, int c=0): r1(r1), r2(r2), c(c) {}
    };
    ll solve(int n, vector<Rect> rs){
        vector<vector<Query>> query(n+1);
        for (auto [l1, l2, r1, r2]: rs){
            query[l1].emplace_back(r1, r2, 1);
            query[l2].emplace_back(r1, r2, -1);
        }
        ll res = 0;
        lazy_segtree<pii, op, e, int, mapping, composition, id> seg(n);
        rep(i, n) seg.set(i, {0, 1});
        rep(qi, n+1){
            for (auto [r1, r2, c]: query[qi]) seg.apply(r1, r2, c);
            auto [mn, s] = seg.all_prod();
            if (mn == 0) res += n-s;
            else res += n;
        }
        return res;
    }
};

/* testcase

abc346_g

*/