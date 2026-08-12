#include <iostream>
#include <vector>
using namespace std;
void chmax(int &a, int b){a = max(a, b);}

int main()
{
    int n, m; cin >> n >> m;
    int ans = 1;
    for (int i = 1; (long long)i*i <= m; i++)
    {
        if (m%i) continue;
        int d1 = i, d2 = m/i;
        // cout << d1 << " " << d2 << endl;
        if (n <= m/d1) chmax(ans, d1);
        if (n <= m/d2) chmax(ans, d2);
    }
    cout << ans << endl;
    return 0;
}