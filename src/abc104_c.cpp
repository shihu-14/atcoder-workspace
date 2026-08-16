#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

const int INF = 1001001001;
const int B = 100;
int main()
{
    int D, G; cin >> D >> G;

    vector<int> ps(D), cs(D), id(D);
    for (int i = 0; i < D; i++)
    {
        cin >> ps[i] >> cs[i];
    }
    iota(id.begin(), id.end(), 0);
    int ans = INF;
    do
    {
        int cnt = 0, sum = 0;
        for (int i = 0; i < D; i++)
        {
            int j = id[i];
            if (sum+(ps[j]-1)*B*(j+1) >= G)
            {
                cnt += (G-sum+(B*(j+1))-1)/(B*(j+1));
                // cout << cnt << endl;
                break;
            }
            cnt += ps[j]-1;
            sum += (ps[j]-1)*B*(j+1);

            // cout << j << " " << sum << " " << i << endl;
            // cout << (ps[j]-1) << " " << B*(j+1) << endl;
            // cout << B*(j+1)+cs[j] << endl;
            if (sum+B*(j+1)+cs[j] >= G)
            {
                cnt++;
                break;
            }
            cnt++;
            sum += B*(j+1)+cs[j];
        }
        // cout << cnt << " " << endl;
        ans = min(ans, cnt);
    } while(next_permutation(id.begin(), id.end()));

    cout << ans << '\n';
    return 0;
}