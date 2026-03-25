// compressed suffix tree 
struct CST
{
    struct Node
    {
        int id;             
        int depth;          
        int parent;         
        vector<int> children;
        Node(int id, int depth, int parent) : id(id), depth(depth), parent(parent) {}
    };

    int n;
    string s;
    vector<Node> nodes;

    CST(const std::string& _s) : s(_s), n(_s.size())
    {
        build();
    }

private:
    void build()
    {
        auto sa = suffix_array(s);
        auto lcp = lcp_array(s, sa);

        nodes.reserve(2*n);
        nodes.emplace_back(0, 0, -1); 

        int v = 0;
        for (int i = 0; i < n; ++i)
        {
            int d = n - sa[i];
            // 新しい葉を追加
            if (nodes[v].depth < d)
            {
                int u = nodes.size();
                nodes.emplace_back(u, d, v);
                nodes[v].children.emplace_back(u);
                v = u;
            }
            if (i == n - 1) break;
            // lcpを参照して次のsuffixの追加位置を決定
            d = lcp[i];
            while (d < nodes[v].depth) v = nodes[v].parent;
            // v-->u-->old_child のパスを分割して新しい内部ノードuを挿入
            if (nodes[v].depth < d)
            {
                int u = nodes.size();
                nodes.emplace_back(u, d, v);
                int old_child = nodes[v].children.back();
                nodes[u].children.emplace_back(old_child);
                nodes[old_child].parent = u;
                nodes[v].children.back() = u;
                nodes[u].parent = v;
                v = u;
            }
        }
    }
};
// 参考 https://atcoder.jp/contests/abc433/submissions/71178201
/* testcase
  abc433_g
*/