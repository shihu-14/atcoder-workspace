n = int(input())
n *= 2
a = list(map(int, input().split()))

ans = 0
for i in range(n-2):
    if a[i] == a[i+2]:  
        ans += 1
print(ans)