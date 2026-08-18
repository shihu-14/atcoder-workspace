#include <iostream>
#include <bitset>
#include <string>

using namespace std;

const int D = 200010;
int main()
{
    int n; cin >> n;
    string a, b, c; cin >> a >> b >> c;
    bitset<D> s;
    s ^= bitset<D>(a);
    s ^= bitset<D>(b);
    s ^= bitset<D>(c);
    bool flag = false;
    for (int i = D-1; i>=0; i--)
    {
        if (s[i] == 1)
        {
            flag = true;
            s.flip(i);
        }
        else if (flag)
        {
            s.flip(i);
        }
    }
    cout << s.to_string() << '\n';
    return 0;
}