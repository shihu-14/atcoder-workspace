from collections import defaultdict
n = int(input())
a = [int(input()) for _ in range(n)]
a.sort()
d = defaultdict(int)
for i in range(n):
    for j in range(n):
        if a[i]*a[j] > a[n-1]:
            break
        d[a[i]*a[j]] += 1

ans = 0
for i in range(n):
    ans += d[a[i]]

print(ans)