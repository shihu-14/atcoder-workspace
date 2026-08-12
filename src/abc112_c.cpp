#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

int main()
{
    int n; cin >> n;
    vector<tuple<int, int, int>> query;
    int non_zero_id = -1;
    for (int i = 0; i < n; i++)
    {
        int x, y, h; cin >> x >> y >> h;
        query.emplace_back(x, y, h);
        if (h > 0) non_zero_id = i;
    }
    assert(non_zero_id != -1);
    for (int cx = 0; cx <= 100; cx++)
    {
        for (int cy = 0; cy <= 100; cy++)
        {
            auto [x, y, h] = query[non_zero_id];
            int H = abs(x-cx)+abs(y-cy)+h;
            bool flag = true;
            for (int qi = 0; qi < n; qi++)
            {
                auto [x, y, h] = query[qi];
                if (max(0, H-abs(x-cx)-abs(y-cy)) != h)
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                cout << cx << " " << cy << " " << H << endl;
                return 0;
            }
        }
    }
    return 0;
}