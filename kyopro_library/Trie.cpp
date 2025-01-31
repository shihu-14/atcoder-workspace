// Trie木
struct Trie {
    struct Node { // Edit here
        map<char, int> to; // 行き先
        int cnt; // 頂点に対する重み
    };
    vector<Node> g;
    Trie(): g(1) {}
    void add(const string& s) {
        int v = 0;
        for (char c: s){
            if (!g[v].to.count(c)){
                int sz = g.size();
                g.emplace_back(Node());
                g[v].to[c] = sz;
            }
            v = g[v].to[c];
        }
        g[v].cnt++; // Edit here
    }
};