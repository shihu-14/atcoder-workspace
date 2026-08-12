#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
void chmin(int &a, int b){a = min(a, b);};
const int INF = 1002003004;
int main()
{
    int n, k; cin >> n >> k;
    vector<int> xs_p={0}, xs_n={0};
    for (int i = 0; i < n; i++)
    {
        int x; cin >> x;
        if (x >= 0) xs_p.emplace_back(x);
        else xs_n.emplace_back(-x);
    }
    reverse(xs_n.begin()+1, xs_n.end());
    int ans = INF;
    for (int a = 0; a < xs_p.size(); a++)
    {
        int b = k-a;
        if (xs_n.size() <= b) continue;   
        // cout << a << " " << b << "-> " << xs_p[a] << " " << xs_n[b] << endl;
        chmin(ans, 2*xs_p[a]+xs_n[b]);
        chmin(ans, xs_p[a]+2*xs_n[b]);
    }
    cout << ans << '\n';
    return 0;
}