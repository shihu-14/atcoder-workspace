#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <utility>

using namespace std;
using ll = long long;

const ll INF = 3001002003004005006ll;

void chmin(ll &a, ll b){a = min(a, b);}
void chmax(ll &a, ll b){a = max(a, b);}

int main()
{
    int n; cin >> n;
    vector<ll> s(n);
    for (int i = 0; i < n; i++)
    {
        cin >> s[i];
    }
    for (int i = 0; i < n-1; i++)
    {
        s[i+1] += s[i];
    }

    ll ans = INF;
    for (int i = 1; i < n-2; i++)
    {
        vector<pair<ll, ll>> left, right;
        {
            ll sum = s[i];
            int mi = lower_bound(s.begin(), s.begin()+i+1, sum/2)-s.begin();
            if (mi < i)
            {
                ll sum1 = s[mi], sum2 = sum-sum1;
                assert(sum1 != 0 && sum2 != 0);
                left.emplace_back(sum1, sum2);
            }
            mi--;
            if (mi >= 0)
            {
                ll sum1 = s[mi], sum2 = sum-sum1;
                assert(sum1 != 0 && sum2 != 0);
                left.emplace_back(sum1, sum2);
            }
        }
        {
            ll sum = s[n-1]-s[i];
            int mi = lower_bound(s.begin()+i+1, s.end(), sum/2+s[i])-s.begin();
            if (mi < n-1)
            {
                ll sum1 = s[mi]-s[i], sum2 = sum-sum1;
                assert(sum1 != 0 && sum2 != 0);
                right.emplace_back(sum1, sum2);
            }
            mi--;
            if (mi >= i+1)
            {
                ll sum1 = s[mi]-s[i], sum2 = sum-sum1;
                assert(sum1 != 0 && sum2 != 0);
                right.emplace_back(sum1, sum2);
            }
        }
        for (auto [l1, l2]: left)
        {
            for (auto [r1, r2]: right)
            {
                chmin(ans, max({l1, l2, r1, r2})-min({l1, l2, r1, r2}));
            }
        }
    }
    cout << ans << '\n';
    return 0;
}