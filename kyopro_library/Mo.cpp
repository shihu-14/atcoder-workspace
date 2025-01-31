const int MX = 200010;
// add,delの演算の定義
struct D {
  int n; ll d; vector<int>& a; vector<int> cnt; // 必要に応じてデータ構造を追加。
  D(vector<int>& a): n(a.size()), a(a), cnt(MX), d(0) {}
  // 適宜、補助関数を定義。
  void add(int i) {
    // ここに処理を加える
  }
  void del(int i){
    // ここに処理を加える
  }
  ll get() {return d;}
};
// Mo's Algorithmのためのデータの加工と実行。クエリは[l, r)
template<class T=long long>
vector<T> Mo(vector<pair<int,int>>& query, D& d) {
  int Q = query.size(); 
  vector<T> res(Q); // クエリに対する答えを保持
  int W = d.n/(sqrt(Q)+1)+1; // 0除算防止の+1
  vector<int> id(Q); iota(id.begin(), id.end(), 0); // 何番目のクエリかを保持
  vector<int> Wid(Q); for(int i=0; i<Q; ++i) Wid[i] = query[i].first/W; // あるクエリがどの縦区分に属するかを保持
  sort(id.begin(), id.end(), [&](int i, int j) { // 実行クエリをソートするための基準を決定
    if (Wid[i] != Wid[j]) return Wid[i] < Wid[j]; // 区分が異なるなら昇順。
    if (Wid[i]&1) return query[i].second > query[j].second; // 区分が同じで奇数番目であれば、rが降順になるように
    else return query[i].second < query[j].second; // 偶数番目ならば昇順になるようにする。(区分を跨ぐ時に最小のコストで移動するため)
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