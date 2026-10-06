class Program
{
    static public void Main()
    {
        int[] input = Console.ReadLine().Split().Select(int.Parse).ToArray();
        int n = input[0];
        long a = input[1];
        long b = input[2];
        var pq = new PriorityQueue<long, long>(Comparer<long>.Create((a, b) => b.CompareTo(a)));

        for (int i = 0; i < n; i++)
        {
            int h = int.Parse(Console.ReadLine());
            pq.Enqueue(h, h);
        }
        long sum = 0;
        int ans = 0;
        while (pq.Count != 0)
        {
            long x1 = pq.Dequeue();
            long x2 = 0;
            if (pq.Count > 0)
            {
                x2 = pq.Dequeue();
            }
            long q = (x1 - x2+a-b-1) / (a-b);
            
            if (x - sum <= 0)
            {
                break;
            }
            x -= a - b;
            pq.Enqueue(x, x);
            sum += b;
            ans++;
        }
        Console.WriteLine(ans);
    }
}