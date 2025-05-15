n, x = map(int, input().split())
s = input()

for i in range(n):
    if s[i] == 'U':
        x //= 2
    elif s[i] == 'L':
        x *= 2
    elif s[i] == 'R':
        x = x*2 + 1

print(x)