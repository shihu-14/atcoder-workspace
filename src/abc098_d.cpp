#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int n; cin >> n;
    vector<int> a(n), s1(n+1), s2(n+1);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
        s1[i+1] = s1[i]^a[i];
        s2[i+1] = s2[i]+a[i];
    }
    return 0;
}