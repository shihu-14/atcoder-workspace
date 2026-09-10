#include <stdio.h>

int main()
{
    int n; scanf("%d", &n);
    int zero = 0, two = 0, one = 0;
    for (int i = 0; i < n; i++)
    {
        int a; scanf("%d", &a);
        if (a%4 == 0)
        {   
            two++;
        }
        else if (a%2 != 0)
        {
            zero++;
        }
        else
        {
            one++;
        }
    }
    printf("%s\n", (one != 0 && zero <= two ) || (one == 0 && zero-1 <= two) ? "Yes": "No");
}