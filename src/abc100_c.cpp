#include <iostream>

using namespace std;

int main()
{
    int n; cin >> n;
    int ans = 0;
    for (int i = 0; i < n; i++)
    {
        int a; cin >> a;
        while(a%2 == 0)
        {
            ans++;
            a /= 2;
        }
    }
    cout << ans << '\n';
    return 0;
}