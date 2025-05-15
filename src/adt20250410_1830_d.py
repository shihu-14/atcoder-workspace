n = int(input())
a = list(map(int, input().split()))
b = [(-v, id+1) for id, v in enumerate(a)]
b.sort()
print(b[1][1])

