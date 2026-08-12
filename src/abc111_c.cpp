#include <iostream>
#include <vector>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::max;
using std::min;
using std::swap;
struct D
{
    int val, col;
    D (int _val=-1, int _col=-1): val(_val), col(_col) {}
};
struct Top2
{
    D a, b;
    Top2 (D _a=D(), D _b=D(-2, -2)): a(_a), b(_b) {}    
    bool operator<(D d)
    {
        if (b.val < d.val)
        {
            swap(b, d);
            if (a.val < b.val)            
            {
                swap(a, b);
            }
            return true;
        }
        return false;
    }
};
int main()
{
    int n; cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];        
    }
    const int B = 100010;
    vector<int> odd_cnt(B), even_cnt(B);
    for (int i = 0; i < n; i+=2) odd_cnt[a[i]]++;
    for (int i = 1; i < n; i+=2) even_cnt[a[i]]++;
    Top2 max_odd, max_even;
    for (int i = 0; i < B; i++)
    {
        max_odd < D(odd_cnt[i], i);
        max_even < D(even_cnt[i], i);
    }
    // cout << max_even.a.val << " " << max_odd.a.val << endl;
    // cout << max_even.a.col << " " << max_odd.a.col << endl;
    if (max_odd.a.col == max_even.a.col)
    {
        cout << min(n-max_odd.a.val-max_even.b.val, n-max_odd.b.val-max_even.a.val) << endl;
    }
    else
    {
        // cout << max_even.a.val << " " << max_odd.a.val << endl;
        cout << n-max_odd.a.val-max_even.a.val << endl;
    }
    return 0;
}