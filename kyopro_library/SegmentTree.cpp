template <typename T, T (*op)(T, T), T (*e)()>
struct SegmentTree{
  private:
    int n;
    vector<T> data;
    void thrust(int i, T x){
        i += n-1;
        data[i] = x;
        while (i>0){
            i = (i-1)/2;
            data[i] = op(data[i*2+1], data[i*2+2]);
        }
    }
    T query(int l, int r, int k, int L, int R){
        if (r <= L || R <= l) return e();
        if (l <= L && R <= r) return data[k];
        T vl = query(l, r, k*2+1, L, (L+R)/2);
        T vr = query(l, r, k*2+2, (L+R)/2, R);
        return op(vl, vr);
    }
  public:
    SegmentTree(int _n){
        int x = 1;
        while (x < _n) x *= 2;
        n = x;
        data.assign(2*n-1, e());
    }
    void set(int i, T x){ // a[i] = x
        assert(0 <= i && i < n);
        thrust(i, x);
    }
    void update(int i, T x){ // a[i] <= op(a[i], x)
        assert(0 <= i && i < n);
        thrust(i, op(data[i+n-1], x));
    }
    T get(int i){ // a[i]
        assert(0 <= i && i < n);
        return data[i+n-1];
    }
    T prod(int l, int r){ // op(a[l], a[l+1], ..., a[r-1])
        assert(0 <= l && l <= r && r <= n);
        return query(l, r, 0, 0, n);
    }
    T all_prod(){ // op(a[0], a[1], ..., a[n-1])
        return query(0, n, 0, 0, n);
    }
};
