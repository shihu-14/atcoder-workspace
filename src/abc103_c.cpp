#include <iostream>
#include <vector>

using std::cin;
using std::cout;

int main()
{
    int n; cin >> n;
    int ans = 0;
    for (int i = 0; i < n; i++)
    {
        int a; cin >> a;
        ans += a-1;
    }
    cout << ans << '\n';
    return 0;
}