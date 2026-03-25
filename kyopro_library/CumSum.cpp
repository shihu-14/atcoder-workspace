struct CumSum
{
    int h, w; 
    vector<vector<ll>> s;
    CumSum(vector<vector<ll>> &a): h(a.size()), w(a[0].size())
    {
        s.resize(h+1, vector<ll>(w+1));
        rep(i, h)rep(j, w) s[i+1][j+1] = s[i+1][j] + a[i][j];
        rep(j, w+1)rep(i, h) s[i+1][j] += s[i][j];
    }
    // [lx, rx), [ly, ry) の総和 (** 0-indexed)
    ll query(int lx, int ly, int rx, int ry)
    {
        if (lx < 0 || ly < 0 || rx > h || ry > w) return 0;
        return s[rx][ry] - s[lx][ry] - s[rx][ly] + s[lx][ly];
    }
};
