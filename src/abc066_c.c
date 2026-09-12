#include <stdio.h>

int main(void)
{
    int n; scanf("%d", &n);
    int odd[n/2], even[n-n/2];
    for (int i = 0; i < n; i++)
    {
        if (i%2 == 0)
        {
            scanf("%d", even+i/2);
        }
        else
        {
            scanf("%d", odd+i/2);
        }
    }    
    if (n%2 == 0)
    {
        for (int i = n/2-1; i >= 0; i--)
        {
            printf("%d ", odd[i]);
        }
        for (int i = 0; i < n-n/2; i++)
        {
            printf("%d ", even[i]);
        }
        printf("\n");
    }
    else
    {
        for (int i = n-n/2-1; i >= 0; i--)
        {
            printf("%d ", even[i]);
        }
        for (int i = 0; i < n/2; i++)
        {
            printf("%d ", odd[i]);
        }
        printf("\n");
    }
    return 0;
}