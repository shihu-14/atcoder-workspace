// Trie木
struct Trie {
    struct Node { // Edit here (Define information for each node)
        unordered_map<char, int> to; // next node
        int cnt; // Node weight
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
        // Edit here (processing for v)
        g[v].cnt++;
        // return v; // void -> int
    }
    // Edit here (DFS on the trie)
};