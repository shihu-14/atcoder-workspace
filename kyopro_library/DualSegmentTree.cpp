// https://ei1333.github.io/library/structure/segment-tree/dual-segment-tree.hpp.html
template <typename Act>
struct DualSegmentTree {
  using F = typename Act::F;

 private:
  int sz, height;
  vector<F> lazy;
  Act m;

  inline void propagate(int k) {
    if (lazy[k] != m.id()) {
      lazy[2 * k + 0] = m.composition(lazy[2 * k + 0], lazy[k]);
      lazy[2 * k + 1] = m.composition(lazy[2 * k + 1], lazy[k]);
      lazy[k] = m.id();
    }
  }

  inline void thrust(int k) {
    for (int i = height; i > 0; i--) propagate(k >> i);
  }

 public:
  DualSegmentTree(Act m, int n) : m(m) {
    sz = 1;
    height = 0;
    while (sz < n) sz <<= 1, height++;
    lazy.assign(2 * sz, m.id());
  }

  void set(int k, const F &f) {
    thrust(k += sz);  
    lazy[k] = f;    
  }

  F get(int k) {
    thrust(k += sz);
    return lazy[k];
  }

  F operator[](int k) { return get(k); }

  void apply(int a, int b, const F &f) {
    thrust(a += sz);
    thrust(b += sz - 1);
    for (int l = a, r = b + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1) lazy[l] = m.composition(lazy[l], f), ++l;
      if (r & 1) --r, lazy[r] = m.composition(lazy[r], f);
    }
  }
};

struct Act{
  // Edit here
  using F;
  static constexpr F composition(const F &f, const F &g) {}
  static constexpr F id() {}
};
