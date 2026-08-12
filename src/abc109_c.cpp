#include <iostream>
#include <vector>
using namespace std;

int gcd(int a, int b)
{
    if (b == 0) return a;
    return gcd(b, a%b);
}
int main()
{
    int n, X; cin >> n >> X;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
    {
        int x; cin >> x;
        a[i] = abs(x-X);
    }
    if (n == 1)
    {
        cout << a[0] << '\n';
        return 0;
    }
    int g = gcd(a[0], a[1]);
    for (int i = 2; i < n; i++)
    {
        g = gcd(g, a[i]);
    }
    cout << g << '\n';
    return 0;
}