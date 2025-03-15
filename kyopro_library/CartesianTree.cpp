// https://nyaannyaan.github.io/library/tree/cartesian-tree.hpp.html
template <typename T> // (graph, root)
pair<vector<vector<int>>, int> CartesianTree(vector<T> &a){
    int n = a.size();
    vector<vector<int>> g(n);
    vector<int> stk, par(n, -1);
    stk.reserve(n);
    rep(i, n){
        int last = -1;
        while (!stk.empty() && a[i] < a[stk.back()]) {
            last = stk.back();
            stk.pop_back();
        }
        if (last != -1) par[last] = i;
        if (!stk.empty()) par[i] = stk.back();
        stk.emplace_back(i);
    }
    int root = -1;
    rep(i, n){
        if (par[i] != -1) g[par[i]].emplace_back(i);
        else root = i;
    }
    return make_pair(g, root);
}


/* testcase

* abc311_g

*/
