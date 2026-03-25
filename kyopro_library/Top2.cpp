struct D{
    // Edit here (data)
    ll val; int col; 
    D(ll _val=-LINF, int _col=-1): val(_val), col(_col) {} 
};
struct Top2{
    D a, b;
    Top2(D _a=D(), D _b=D()): a(_a), b(_b) {}
    void operator<=(D x){ // Edit here (max)
        if (b.val < x.val){
            swap(b, x);
            if (a.val < b.val) swap(a, b);
        }
        if (a.col == b.col) swap(b, x); // Edit here (constrains: keep different colors)
    }
    void operator<=(Top2 x){
        *this <= x.a;
        *this <= x.b;
    }
};

/* testcase

abc345_e
abc429_e
abc447_g

*/
