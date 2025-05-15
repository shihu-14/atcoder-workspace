import itertools
s, k = input().split()
s = list(s)
k = int(k)
s.sort()
cnt = 0
t = sorted(set(itertools.permutations(s)))
print("".join(t[k-1]))

