s = list(input())
t = list(input())
n = len(s)
ans = []
while s != t:
    tmp = []
    for i in range(n):
        if s[i] != t[i]:
            u = s[:]
            u[i] = t[i]
            tmp.append(u)
    tmp.sort()
    s = tmp[0][:]
    ans.append(s[:])
    
print(len(ans))
for a in ans:
    print("".join(a))