from collections import deque
import string
def find_start_and_end(grid) : 
    start = ()
    end = ()
    for i,row in enumerate(grid) : 
        for j,char in enumerate(row) :
            if char == 'S' : 
                start = (i,j)
            if char  == 'E' : 
                end = (i,j)
    return start,end

def add(n1,n2) : 
    (x1,y1) = n1
    (x2,y2) = n2 
    return (x1+x2,y1+y2)


def distance(n1,n2) : 
    (x1,y1) = n1
    (x2,y2) = n2 
    return abs(x1-x2)+abs(y1-y2)

def in_bounds(node,grid):
    rows = len(grid)
    columns = len(grid[0])
    (row,col) = node
    return row >=0 and col >= 0 and row < rows and col < columns

def get_shortest_path(grid): 
    start,end = find_start_and_end(grid)
    path = [start]
    # Do a simple bfs to get the path, doesn't really matter what you do
    q = deque()
    q.append(start) 
    visited = set()
    visited.add(start)
    shifts = [(1,0),(0,1),(-1,0),(0,-1)]
    while q : 
        current = q.pop()
        if current == end : 
            return path 
        for shift in shifts : 
            neighbor = add(current,shift)
            (row,col) = neighbor
            if in_bounds(neighbor,grid) and grid[row][col] != '#' : 
                if neighbor not in visited : 
                    visited.add(neighbor)
                    path.append(neighbor) 
                    q.append(neighbor)


def find_number_of_cheats(shortest_path,grid, maximal_cheat_time): 
    counts = {}
    # The -1 appears since the time is len(path)-1
    total_time = len(shortest_path)-1
    for i in range(len(shortest_path)) : 
        for j in range(i+1,len(shortest_path)) : 
            if distance(shortest_path[i],shortest_path[j]) <=maximal_cheat_time :
                time_save = total_time-(len(shortest_path[:i])+len(shortest_path[j+1:] ) + distance(shortest_path[i],shortest_path[j]))
                if time_save not in counts : 
                    counts[time_save] = 1
                else : 
                    counts[time_save] += 1
    sorted_counts = {k: counts[k] for k in sorted(counts)}

    return sum( [sorted_counts[key]  for key in sorted_counts if key >=100]) 



grid = []
with open("input.txt") as f: 
    grid = [list(line.strip()) for line in f.readlines()]

shortest_path = get_shortest_path(grid)

print("Part 1:", find_number_of_cheats(shortest_path,grid,2))
print("Part 2:", find_number_of_cheats(shortest_path,grid,20))
