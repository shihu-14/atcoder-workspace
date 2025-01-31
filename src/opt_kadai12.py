import networkx as nx

G = nx.DiGraph()
G.add_nodes_from(['s', '1', '2', '3', '4', '5', 't'])
G.add_edge('s', '1', capacity=3)
G.add_edge('s', '4', capacity=2)
G.add_edge('1', '2', capacity=2)
G.add_edge('1', '3', capacity=1)
G.add_edge('2', 't', capacity=1)
G.add_edge('2', '3', capacity=4)
G.add_edge('3', 't', capacity=2)
G.add_edge('3', '5', capacity=2)
G.add_edge('4', '3', capacity=2)
G.add_edge('4', '5', capacity=2)
G.add_edge('5', 't', capacity=2)
flow_value, flow_dict = nx.maximum_flow(G, 's', 't')

# 結果を表示
print("最大流の値:", flow_value)
print("フロー分布:")
for u, flows in flow_dict.items():
    for v, flow in flows.items():
        print(f"  {u} -> {v}: {flow}")
