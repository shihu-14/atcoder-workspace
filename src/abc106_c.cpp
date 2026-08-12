#include <iostream>
using namespace std;

int main()
{
    string s; cin >> s;
    long long k; cin >> k; k--;
    int n = s.size();
    int num = 1, id = n;
    for (int i = 0; i < n; i++)
    {
        if (s[i] != '1')
        {
            num = s[i]-'0';
            id = i;
            break;
        }
    }
    cout << (k < id ? 1 : num) << endl;
    return 0;
}