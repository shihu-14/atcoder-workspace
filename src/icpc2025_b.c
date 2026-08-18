#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[])
{
    if (argc != 3) return 1;
    FILE* in = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");
    int n;
    char s[51];
    while(fscanf(in, "%d%s", &n, s) == 2)
    {
        int id = 0;
        for (int i = n-1; i > 0; i--)
        {
            bool flag = true;
            for (int j = 0; j < i; j++)
            {
                if (s[j] != s[n-i+j])
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                id = i;
                break;
            }
        }
        // printf("%d\n", id);
        for (int j = 0; j < n; j++)
        {
            fprintf(out, "%c", s[j]);
        }
        for (int j = id; j < n; j++)
        {
            fprintf(out, "%c", s[j]);
        }
        fprintf(out, "\n");
    }

    fclose(in);
    fclose(out);
    return 0;
}


/*
../icpc2025_testcase/B/001.in
../icpc2025_testcase/B/002.in

*/