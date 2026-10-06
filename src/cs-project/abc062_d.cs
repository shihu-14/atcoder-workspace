class ABC062_D
{
    static void chmax(ref long a, long b)
    {
        a = Math.Max(a, b);
    }
    static void Main1()
    {
        int n = int.Parse(Console.ReadLine());
        int[] a = Console.ReadLine().Split().Select(int.Parse).ToArray();

        var s1 = new SortedDictionary<int, int>();
        var s2 = new SortedDictionary<int, int>();

        long sum1 = 0, sum2 = 0, ans = long.MinValue;
        for (int i = 0; i < 3 * n; i++)
        {
            if (i < n)
            {
                if (!s1.TryAdd(a[i], 1))
                {
                    s1[a[i]]++;
                    sum1 += a[i];
                }
            }
            else if (i < 2 * n)
            {
                if (!s2.TryAdd(a[i], 1))
                {
                    s2[a[i]]++;
                    sum2 += a[i];
                }
            }
            else
            {
                if (!s2.TryAdd(a[i], 1))
                {
                    s2[a[i]]++;
                    sum2 += a[i];
                }
                var (k, v) = s2.Last();
                sum2 -= k;
                if (v - 1 == 0)
                {
                    s2.Remove(k);
                }
                else
                {
                    s2[k] = v - 1;
                }
            }
        }
        chmax(ref ans, sum1 - sum2);
        for (int i = n; i < 2 * n; i++)
        {
            // s1
            var (k1, v1) = s1.Last();
            sum1 -= k1;
            if (v1 - 1 == 0)
            {
                s1.Remove(k1);
            }
            else
            {
                s1[k1] = v1 - 1;
            }
            if (!s1.TryAdd(a[i], 1))
            {
                s1[a[i]]++;
            }
            (k1, v1) = s1.Last();
            sum1 += k1;

            // s2
            var (k2, v2) = s2.First();
            sum2 -= k2;
            if (v2 - 1 == 0)
            {
                s2.Remove(k2);
            }
            else
            {
                s2[k2] = v2 - 1;
            }
            if (!s2.TryAdd(a[i], 1))
            {
                s1[a[i]]++;
            }
            (k1, v1) = s1.Last();
            sum1 += k1;
        }
        Console.WriteLine(ans);
        return;
    }
}