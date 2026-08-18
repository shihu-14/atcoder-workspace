#include <stdio.h>

int main(void)
{
    FILE* in = fopen("../icpc2025_testcase/A/001.in", "r");
    FILE* out = fopen("../icpc2025_testcase/A/001.out", "a");

    char line[10];
    while(fgets(line, sizeof(line), in) != NULL)
    {
        int n;
        sscanf(line, "%d", &n);
        if (n == 0) break;
        int ans = 0;
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                ans += i*j;
            }
        }
        fprintf(stdout, "%d\n", ans);
    }
    fclose(in);
    fclose(out);
    return 0;
}