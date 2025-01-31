// https://www.youtube.com/live/n7WZOQVuGp4
// Deletable Dual Segment Tree
struct DualSegmentTree2 {
  int n;
  vector<multiset<int>> d; 
  DualSegmentTree2(int mx=0) {
    n = 1;
    while (n < mx) n <<= 1;
    d.resize(n*2);
  }
  void add(int l, int r, int x) {
    l += n; r += n;
    while (l < r) {
      if (l%2) d[l].insert(x), l++;
      if (r%2) r--, d[r].insert(x);
      l >>= 1; r >>= 1;
    }
  }
  void del(int l, int r, int x) {
    l += n; r += n;
    while (l < r) {
      if (l%2) d[l].erase(d[l].find(x)), l++;
      if (r%2) r--, d[r].erase(d[r].find(x));
      l >>= 1; r >>= 1;
    }
  }
  int get(int i) {
    int res = 0;
    i += n;
    while (i) {
      if (d[i].size()) res = max(res, *d[i].rbegin()); // Edit here
      i >>= 1;
    }
    return res;
  }
};
