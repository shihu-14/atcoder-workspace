
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int n, K; cin >> n >> K;
    vector<int> ans(n);
    for (int i = 0; i < n; i++)
    {   
        int m; cin >> m;
        for (int j = 0; j < m; j++)
        {
            int s; cin >> s;
            if (s >= K) ans[i]++;
        }
    }
    for (int i = 0; i < n; i++)
    {
        cout << ans[i] << endl;
    }
}