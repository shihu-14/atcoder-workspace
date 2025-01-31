// ラグランジュ補間による多項式の復元(与えられる点が初項0,公差1の等差数列になっている前提でO(n))
struct Interpolation{
    int n;
    vector<mint> a; // 多項式の係数(a0,a1..an-1)
    Interpolation() {}
    Interpolation(vector<mint> &p){ // 異なるn個(x=0,1,,,の等差数列)からO(n)で、n-1次多項式を求める。
        n = p.size(); a.resize(n);
        vector<mint> invf(n, 1); // 係数aiを求めるために割り算を行う必要があるので前計算しておく。
        mint f = 1;
        for (int i = 1; i < n; ++i) f *= i;
        invf[n-1] = mint(1)/f;
        for (int i = n-1; i >= 1; --i) invf[i-1] = invf[i]*i;
        for (int i = 0; i < n; ++i) {
            mint b = invf[i]*invf[n-1-i];
            a[i] = p[i] * ((n-1-i)&1 ? -b : b); // 偶奇で符号を調整する。
        }
    }
    // 多項式が求まった前提で、一般にf(x)の値を求める。
    mint operator()(mint x) const {
        vector<mint> r(n,1); // 右からの累積和rと、左からの累積和lを求めることで、O(n)でf(x)を求めることができる。
        for (int i = n-1; i >= 1; --i) r[i-1] = r[i]*(x-i);
        mint l = 1, res;
        for (int i = 0; i < n; ++i) {
            res += a[i]*l*r[i];
            l *= x-i;
        }
        return res;
    }
};
