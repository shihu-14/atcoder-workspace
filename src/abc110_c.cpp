#include <iostream>
#include <vector>
using namespace std;

int main()
{
    string s, t; cin >> s >> t;
    int n = s.size();
    vector<int> id(26, -1);
    for (int i = 0; i < n; i++)
    {
        if (id[s[i]-'a'] == -1)
        {
            id[s[i]-'a'] = t[i] - 'a';
        }
        else
        {
            if (id[s[i]-'a'] != t[i]-'a')
            {
                cout << "No" << '\n';
                return 0;
            }
        }
    }
    vector<int> d(26);
    for (int i = 0; i < 26; i++)
    {
        if (id[i] == -1) continue;
        d[id[i]]++;
        if (d[id[i]] >= 2)
        {
            cout << "No" << '\n';
            return 0;
        }
    }
    cout << "Yes" << '\n';
    return 0;
}