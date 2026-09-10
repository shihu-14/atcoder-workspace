#include <stdio.h>
#include <stdlib.h>

// int cmp(const void* a, const void* b)
// {
//     int x = *(const int*)a;
//     int y = *(const int*)b;
//     return (x>y)-(x<y);
// }

void merge_sort(int* arr, int l, int r, int* tmp)
{
    if (r-l <= 1)
    {
        return;
    }
    int m = (l+r)/2;
    merge_sort(arr, l, m, tmp);
    merge_sort(arr, m, r, tmp);
    int i = l, j = m, k = l;
    while(i < m && j < r)
    {
        if (arr[i] <= arr[j])
        {
            tmp[k++] = arr[i++];
        }
        else
        {
            tmp[k++] = arr[j++];
        }
    }
    while(i < m)
    {
        tmp[k++] = arr[i++];
    }
    while (j < r)
    {
        tmp[k++] = arr[j++];
    }
    for (int x = l; x < r; x++)
    {
        arr[x] = tmp[x];
    }
    return;
}
int main(void)
{
    int n; scanf("%d", &n);
    int a[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d", a+i);
    }
    // qsort(a, n, sizeof(int), cmp);
    int* tmp = malloc(sizeof(int)*n);
    merge_sort(a, 0, n, tmp);
    free(tmp);
    // for (int i = 0; i < n; i++)
    // {
    //     printf("%d ", a[i]);
    // }
    // printf("\n");
    int cnt[n], id[n];
    for (int i = 0; i < n; i++)
    {
        cnt[i] = 0;
        id[i] = -1;
    }
    {
        int j = 0;
        cnt[j]++;
        id[j] = a[j];
        for (int i = 1; i < n; i++)
        {
            if (a[i-1] == a[i])
            {
                cnt[j]++;
                continue;
            }
            j++;
            cnt[j]++;
            id[j] = a[i];
        }
    }
    int max1 = -1;
    for (int i = n-1; i >= 0; i--)
    {
        if (cnt[i] < 2) continue;
        if (max1 == -1)
        {
            max1 = id[i];
            if (cnt[i] >= 4)
            {
                printf("%lld\n", (long long)max1*max1);
                return 0;
            }
        }
        else
        {
            printf("%lld\n", (long long)max1*id[i]);
            return 0;
        }
    }
    printf("0\n");
    return 0;
}