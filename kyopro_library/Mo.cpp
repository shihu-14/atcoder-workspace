// calc diff
struct D {
  int n; vector<int>& a;
  // Edit here (add data structure if needed)
  ll d;
  D(vector<int>& a): n(a.size()), a(a), d(0) {}
  // update query: add(), del()
  void add(int i) {
    // Edit here
  }
  void del(int i){
    // Edit here
  }
  // select query: get()
  ll get() {
    // Edit here (plz add parameters if needed)
    return d;
  }
};
// Mo's Algorithm: query: [l, r)
template<class T=long long>
vector<T> Mo(vector<pair<int,int>>& query, D& d) {
  int Q = query.size(); 
  vector<T> res(Q); // res[i] = query[i]
  int W = d.n/(sqrt(Q)+1)+1; 
  vector<int> id(Q); iota(id.begin(), id.end(), 0);
  vector<int> Wid(Q); for(int i=0; i<Q; ++i) Wid[i] = query[i].first/W;
  sort(id.begin(), id.end(), [&](int i, int j) {
    if (Wid[i] != Wid[j]) return Wid[i] < Wid[j];
    if (Wid[i]&1) return query[i].second > query[j].second;
    else return query[i].second < query[j].second;
  });
  int l = 0, r = 0;
  for (int i : id) {
    auto [nl, nr] = query[i];
    while (r < nr) d.add(r++);
    while (l > nl) d.add(--l);
    while (l < nl) d.del(l++);
    while (r > nr) d.del(--r);
    res[i] = d.get();
  }
  return res;
}