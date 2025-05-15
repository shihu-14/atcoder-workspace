a = list(map(int, input().split()))
v = a[0]
a = a[1:]
v %= sum(a)
man = ["F", "M", "T"]
for id, l in enumerate(a):
    if v < l:
        print(man[id])
        break
    v -= l