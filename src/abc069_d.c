#include <stdio.h>

int main(void)
{
    int h, w, n; 
    scanf("%d %d %d", &h, &w, &n);
    int ans[h][w];
    int x = 0, y = 0;
    for (int i = 1; i <= n; i++)
    {
        int a; scanf("%d", &a);
        for (;x < h;)
        {
            if (x%2 == 0)
            {
                for (;;)
                {
                    ans[x][y++] = i;
                    a--;
                    if (y == w)
                    {
                        x++;
                        y = w-1;
                        break;
                    }
                    if (a == 0) break;
                }
            }
            else
            {
                for (;;)
                {
                    ans[x][y--] = i;
                    a--;
                    if (y == -1)
                    {
                        x++;
                        y = 0;
                        break;
                    }
                    if (a == 0) break;
                }
            }
            if (a == 0) break;
        }
    }
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            printf("%d ", ans[i][j]);
        }
        printf("\n");
    }
    return 0;
}