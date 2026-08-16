#include <iostream>
#include <vector>

using namespace std;
using ll = long long;
const int D = 40;

// 解法1
int main()
{
    int n; cin >> n;
    vector<int> stk;
    while(n != 0)
    {
        int r = ((n%2)+2)%2;
        stk.emplace_back(r);
        n -= r;
        n /= -2;
    }
    if (stk.empty())
    {
        cout << 0 << '\n';
    }
    else
    {
        for (int i = stk.size()-1; i >= 0; i--)
        {
            cout << stk[i];
        }
        cout << "\n";
    }
    return 0;
}

// 解法2
// int main()
// {
//     ll n; cin >> n;
//     vector<ll> a(D); a[1] = 1;
//     for (int i = 1; i < D-1; i++)
//     {
//         a[i+1] = a[i]*2;
//     }
//     vector<int> ans(D);
//     while(n != 0)
//     {
//         if (n >= 0)
//         {
//             ll sum = 0;
//             int id = 0;
//             for (int i = 1; i < D; i += 2)
//             {
//                 if (sum >= n) break;
//                 sum += a[i];
//                 id = i;
//             }
//             ans[id-1] = 1;
//             n -= a[id];
//         }
//         else
//         {

//             ll sum = 0;
//             int id = 0;
//             for (int i = 2; i < D; i += 2)
//             {
//                 if (sum >= abs(n)) break;
//                 sum += a[i];
//                 id = i;
//             }
//             ans[id-1] = 1;
//             n += a[id];
//         }
//     }
//     bool flag = false;
//     for (int i = D-1; i >= 0; i--)
//     {
//         if (ans[i] == 1)
//         {
//             flag = true;
//             cout << ans[i];
//         }
//         else if (flag)
//         {
//             cout << ans[i];
//         }
//     }
//     if (!flag) cout << "0";
//     cout << '\n';
//     return 0;
// }

