import pprint
import networkx as nx
graph = {}
edges = []
with open("input.txt") as f : 
    for line in f.readlines(): 
        edge = line.strip().split('-')
        edges.append(edge)
        [u,v] = edge
        if u not in graph : 
            graph[u] = []
        if v not in graph : 
            graph[v] = [] 
        graph[u].append(v)
        graph[v].append(u)


def find_triangles(graph,edges) : 
    triangles = set()  # Set to store unique triangles
    # For each edge (u, v), check for common neighbors
    for u, v in edges:
        for w in graph[u]:
            if w != v and w in graph[v]:  # Check if (u, w) and (w, v) are edges
                if u[0] == 't' or v[0] == 't' or w[0] == 't' : 
                    triangle = tuple(sorted([u, v, w]))  # Sort to avoid duplicates 
                    triangles.add(triangle)
    return triangles 


print(len(find_triangles(graph,edges)))

nxGraph = nx.Graph(graph)
cliques =nx.find_cliques(nxGraph)
largest_clique =  max(cliques,key = len)
#print(largest_clique)
sorted = sorted(largest_clique)
print(','.join(sorted))
