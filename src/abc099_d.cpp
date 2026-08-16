#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

void chmin(int &a, int b){a = min(a, b);}
const int INF = 1001001001;
int main()
{
    int n, c; cin >> n >> c;
    vector<vector<int> > d(c, vector<int>(c)), a(n, vector<int>(n));
    for (int i = 0; i < c; i++)
    {
        for (int j = 0; j < c; j++)
        {
            cin >> d[i][j];
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> a[i][j], a[i][j]--;
        }
    }
    vector<vector<pair<int,int> >> cs(3); 
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cs[(i+j)%3].emplace_back(i, j);
        }
    }
    vector<vector<int>> cost(3, vector<int>(c));

    for (int i = 0; i < 3; i++)
    {
        int res = 0;
        for (int j = 0; j < c; j++)
        {
            int sum = 0;
            for (auto [x, y]: cs[i])
            {
                if (a[x][y] != j)
                {
                    sum += d[a[x][y]][j];
                }
            }
            cost[i][j] = sum;
        }
    }

    int ans = INF;
    for (int i = 0; i < c; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (i == j) continue;
            for (int k = 0; k < c; k++)
            {
                if (j == k || k == i) continue;
                chmin(ans, cost[0][i]+cost[1][j]+cost[2][k]);
            }
        }
    }
    cout << ans << '\n';
    return 0;
}