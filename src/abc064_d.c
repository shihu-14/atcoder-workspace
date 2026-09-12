#include <stdio.h>


int main(void)
{
    int n; scanf("%d", &n);
    char s[n+1]; scanf("%s", s);
    int min_val = 1001001001, sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += (s[i] == '(' ? 1: -1);
        // printf("sum: %d\n", sum);
        if (sum < min_val)
        {
            min_val = sum;
        }
    }
    // printf("%d, %d\n", min_val, sum);
    if (min_val < 0)
    {
        int prefix = -min_val;
        for (int i = 0; i < prefix; i++)
        {
            printf("(");
        }
        printf("%s", s);
        sum += prefix;
        for (int i = 0; i < sum; i++)
        {
            printf(")");
        }
        printf("\n");
    }
    else
    {
        printf("%s", s);
        for (int i = 0; i < sum; i++)
        {
            printf(")");
        }
        printf("\n");
    }
    return 0;
}