import math
import heapq
from collections import deque


def add(pos,dir) :
    (x,y)=pos
    (u,v)=dir
    return (x+u,y+v)

def in_bounds(pos,rows,cols) : 
    (row,col) = pos
    return row>= 0 and col >= 0 and row < rows and col < cols 

def find_best_path(blocking_objects,start) : 
    # This is the map containing the scores (equivalent to distances)
    rows = max( [row for (row,col) in blocking_objects])+1
    cols = max( [col for (row,col) in blocking_objects])+1
    distances ={}
    previous ={}
    #facing = (0,1)
    nodes = set()
    shifts = [(1,0),(-1,0),(0,1),(0,-1)]
    # Initialize
    for i in range(rows): 
        for j in range(cols) : 
            if (i,j) not in blocking_objects : 
                distances[(i,j)] = float('inf')
                previous[(i,j)] = (None,None)

    distances[start] = 0
    pq = [(0,start)]

    #Do Dijkstra's
    while pq : 
        current_score,node = heapq.heappop(pq)

        if current_score > distances[node] : 
            continue

        for shift in shifts : 
            next_node = add(node,shift)
            (next_row,next_col) =next_node

            if in_bounds(next_node,rows,cols) and next_node not in blocking_objects : 
                score = current_score + 1
                if score < distances[next_node] : 
                    distances[next_node] = score 
                    previous[next_node]=node
                    heapq.heappush(pq,(score,next_node))

    return distances,previous


coordinates = []
with open('input.txt') as f : 
    coordinates = [tuple( [ int(val) for val in v.strip().split(',') ]) for v in f.readlines()]
# Initialization
step = 1024
distances,_ = find_best_path(coordinates[:step],(0,0))
target = (70,70)

dist = distances[target]
print("Part 1",dist)


while math.isfinite(dist) :
    step +=1
    distances,_ = find_best_path(coordinates[:step],(0,0))
    dist = distances[target]

print(f"Distance turns infinite at step {step} at coordinate {coordinates[step-1]}")

