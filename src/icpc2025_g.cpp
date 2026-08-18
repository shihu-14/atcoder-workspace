#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <string>
using namespace std;

using ll = long long;
const ll D = 1000000000;

int main(int argc, char* argv[])
{
    if (argc != 3) return 1;
    FILE* in = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");
    while(1)
    {
        int n; fscanf(in, "%d", &n);
        if (n == 0) break;
        vector<ll> d1(n);
        for (int i = 0; i < n; i++)
        {
            char d[50];
            fscanf(in, "%s", d);
            string s = d;
            s.erase(s.find('.'), 1);
            d1[i] = 180*D-stoll(s);
        }
        int m; fscanf(in, "%d", &m);
        vector<ll> d2(m);
        for (int i = 0; i < m; i++)
        {
            char d[50];
            fscanf(in, "%s", d);
            string s = d;
            s.erase(s.find('.'), 1);
            d2[i] = 180*D-stoll(s);
            // double d;
            // fscanf(in, "%lf", &d);
            // d2[i] = llround((180-d)*D);
        }
        for (int i = 0; i < n-1; i++)
        {
            d1[i+1] += d1[i];
        }
        d1[n-1] = 0;
        for (int i = 0; i < m-1; i++)
        {
            d2[i+1] += d2[i];
        }
        d2[m-1] = 0;
        set<int> ans;
        ans.emplace(n+m+2);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                vector<ll> d2_tmp = d2;
                ll diff = d1[i]-d2[j];
                for (int k = 0; k < m; k++)
                {
                    d2_tmp[k] += diff;
                    d2_tmp[k] = ((d2_tmp[k])%(360*D)+360*D)%(360*D);
                }
                int res = n+m+2;
                for (int k = 0; k < m; k++)
                {
                    for (int l = 0; l < n; l++)
                    {
                        if (d2_tmp[k] == d1[l])
                        {
                            res--;
                        }
                    }
                }
                ans.emplace(res);
            }
            
        }
        int cnt = 0;
        for (auto v: ans)
        {
            cnt++;
            if (cnt == ans.size())
            {
                fprintf(out, "%d\n", v);
            }
            else
            {
                fprintf(out, "%d ", v);
            }
        }
    }
    fclose(in);
    fclose(out);
    return 0;
}