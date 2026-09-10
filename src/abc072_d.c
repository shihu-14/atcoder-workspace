#include <stdio.h>
#include <stdlib.h>
#define D 100010

int main(void)
{
    int n; scanf("%d", &n);
    int *a = malloc(sizeof(int)*n);
    
    for (int i = 0; i < n; i++)
    {
        scanf("%d", a+i);
        a[i]--;
    }
    int ans = 0;
    for (int i = 0; i < n; i++)
    {
        if (i < n-1 && a[i] == i && a[i+1] == i+1)
        {
            ans++;
            i++;
        }
        else if (a[i] == i)
        {
            ans++;
        }
    }
    free(a);
    printf("%d\n", ans);
    return 0;
}