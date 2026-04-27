// binary trie
template<class T=ll, int k=60>
struct BinaryTrie
{
    struct Node
    {
        array<int, 2> to;
        int cnt;
        // add other variables
        Node(): to({-1, -1}), cnt(0) {}
    };
    vector<Node> d; 
    BinaryTrie(): d(1){}
    int go(int v, int x)
    {
        if (d[v].to[x] == -1)
        {
            d[v].to[x] = d.size();
            d.emplace_back(Node());
        }
        return d[v].to[x];
    }
    void add(T x, int a=1)
    {
        int v = 0;
        vector<int> vs;
        for (int i = k-1; i >= 0; --i){
            d[v].cnt += a;
            vs.emplace_back(v);
            v = go(v, x>>i&1);
        }
        d[v].cnt += a;
    }
    // add other functions
    ll f(,,)
    {

    }
};

/* testcase

abc451_g

*/
