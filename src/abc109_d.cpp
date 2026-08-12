#include <iostream>
#include <vector>
#include <tuple>

using namespace std;
using T4 = tuple<int,int,int,int>;

int main()
{
    int h, w; cin >> h >> w;
    vector<vector<int> >a(h, vector<int>(w));
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            cin >> a[i][j];
        }
    }

    vector<T4> ans;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (a[i][j]%2 == 0) continue;
            if (j == w-1)
            {
                if (i+1 < h)
                {
                    a[i][j]--;
                    a[i+1][j]++;
                    ans.emplace_back(i, j, i+1, j);
                }
            }
            else
            {
                a[i][j]--;
                a[i][j+1]++;
                ans.emplace_back(i, j, i, j+1);
            }
        }
    }
    cout << ans.size() << '\n';
    for (auto [x, y, nx, ny]: ans)
    {
        cout << x+1 << " " << y+1 << " " << nx+1 << " " << ny+1 << '\n';
    }
    return 0;
}