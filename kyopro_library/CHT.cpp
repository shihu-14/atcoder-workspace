// Convex Hull Trick (for max/min query)
struct CHT {
    struct line { // 一次関数を扱う構造体。
        ll a, b;
        line(ll a=0, ll b=0): a(a), b(b) {}
        ll operator()(ll x) const { return a*x+b;}
    };
    bool XisMonotone; // クエリに単調性があるかどうか。
    function<bool(ll a, ll b)> cmp; // 最大値クエリはgreaterを、最小値クエリはlessを指定すること。
    CHT(bool flag, function<bool(ll a, ll b)> cmp) : XisMonotone(flag), cmp(cmp) {}

    vector<line> ls;
    void add(ll a, ll b) { // 傾きa、切片bの直線を追加する。
        line l(a,b);
        assert(ls.size() == 0 || ls.back().a <= l.a); // 追加クエリの直線の傾きが単調であることが条件。
        while (ls.size() >= 2) {
            const line& l1 = ls[ls.size()-2];
            const line& l2 = ls[ls.size()-1];
            // この条件を満たすとき、削除不要。そうでなければ、新しい直線を追加することで末尾の直線が不要になる。
            if ((l.a-l2.a)*(l1.b-l2.b) < (l2.a-l1.a)*(l2.b-l.b)) break;
            ls.pop_back();
        }
        ls.emplace_back(l);
    }
    ll operator()(ll x) { // 与えられたクエリを処理する。
        if (XisMonotone){ // クエリに単調性がある場合は、クエリ全体をO(N)で処理できる。
            static int head = 0;
            while (head <= ls.size()-2 && !cmp(ls[head](x), ls[head+1](x))) { // クエリの先頭から二番目までを見てクエリを処理する。
                head++;
            }
            return ls[head](x);
        }
        else{ // クエリに単調性がない場合は、各クエリにO(logN)かけて処理をする。
            int wa = -1, ac = ls.size()-1;
            while (ac-wa > 1) { // 二分探索
                int wj = (wa+ac)/2;
                if (!cmp(ls[wj](x), ls[wj+1](x))) wa = wj;
                else ac = wj;
            }
            return ls[ac](x);
        }
    }
};
