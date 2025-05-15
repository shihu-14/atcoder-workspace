from collections import deque
s = input()
t = "atcoder"
n = 7

dist = {}
dist[s] = 0
q = deque()
q.append(s)
def bfs():
    while len(q) > 0:
        now = q.popleft()
        if now == t:
            return
        for i in range(n-1):
            to = list(now)
            to[i], to[i+1] = now[i+1], now[i]
            to = "".join(to)
            if to not in dist:
                dist[to] = dist[now]+1
                q.append(to)

bfs()
print(dist[t])
