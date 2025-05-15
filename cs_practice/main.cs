using System;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Data.Common;
using System.Numerics;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices.Marshalling;
using Microsoft.VisualBasic;

class Program
{

    static void Main()
    {
        var input = Console.ReadLine().Split().Select(long.Parse).ToArray();
        int n = (int)input[0];
        long L = input[1], R = input[2];
        var a = Console.ReadLine().Split().Select(long.Parse).ToArray();
        var sr = new long[n+1];
        for (int i = n; i > 0; i--)
        {
            sr[i-1] = sr[i] + a[i-1];
        }
        var pq = new PriorityQueue<(long, int), long>();
        long ans = L*n;
        for (int i = n-1; i >= 0; i--)
        {
            pq.Enqueue((sr[i]-R*(n-i), i), -sr[i]+R*i);
            var (diff, id) = pq.Peek();
            ans = Math.Min(ans, L*i+sr[id]-diff);
        }
        Console.WriteLine(ans);
    }
}