import heapq
from collections import deque
grid =[]

with open('input.txt') as f : 
    grid = [list(line.strip()) for line in f.readlines()]


def find_start(grid):
    for i,row in enumerate(grid) :
        for j,char in enumerate(row) :
            if(char == 'S') : 
                return (i,j)
            
def find_end(grid) :
    for i,row in enumerate(grid) :
        for j,char in enumerate(row) :
            if(char == 'E') : 
                return (i,j)

def rotate( direction,orientation) : 
    (x,y) = direction
    if orientation>0 : 
        tmp = x
        x = -y
        y = tmp  
    elif orientation<0:
        tmp = x
        x = y
        y = -tmp  
    return (x,y)

def add(pos,dir) :
    (x,y)=pos
    (u,v)=dir
    return (x+u,y+v)

def in_bounds(pos,grid) : 
    rows = len(grid)
    cols = len(grid[0])
    (row,col) = pos
    return row>= 0 and col >= 0 and row < rows and col < cols 



#dijkstra's
def find_best_path(grid,start,end,facing) : 
    # This is the map containing the scores (equivalent to distances)
    scores ={}
    previous ={}
    #facing = (0,1)
    nodes = set()

    # Initialize
    for i,row in enumerate(grid) :
        for j,char in enumerate(row) :
            if char == '.' :
                node = (i,j)
                dir = facing 
                for t in range(4) : 
                    scores[(node,dir)] = float('inf')
                    previous[(node,dir)] = []
                    nodes.add((node,dir))
                    dir = rotate(dir,1)
    
    dir = facing 
    for i in range(4) :
        scores[(start,dir)] = float('inf')*i
        previous[(start,dir)] = []
        nodes.add((start,dir))

        scores[(end,dir)] = float('inf')
        previous[(end,dir)] = []
        nodes.add((end,dir))
        dir = rotate(dir,1)

    scores[(start,facing)] = 0
    pq = [(0,(start,facing))]

    #Do Dijkstra's
    while pq : 
        current_score,(node,direction) = heapq.heappop(pq)

        if current_score > scores[(node,direction)] : 
            continue
        # Rotate
        for rot in [-1,1] : 
            new_direction = rotate(direction,rot)
            score = current_score+1000

            if score < scores[ (node,new_direction ) ] : 
                scores[ (node,new_direction ) ] = score 
                previous[(node,new_direction )].append((node,direction))
                heapq.heappush(pq,(score,(node,new_direction)))
            # # This is a part 2 modification
            #elif score == scores[ (node,new_direction ) ] : 
            #    previous[(node,new_direction )].append((node,direction))

        # Or move (if possible)
        next_node = add(node,direction)
        (next_row,next_col) =next_node
        if in_bounds(next_node,grid) and (grid[next_row][next_col] == '.' or  grid[next_row][next_col] == 'E' or grid[next_row][next_col] == 'S'): 
            score = current_score + 1
            if score < scores[(next_node,direction)] : 
                scores[(next_node,direction)] = score 
                previous[(next_node,direction)].append((node,direction))
                heapq.heappush(pq,(score,(next_node,direction)))
            # This is a part 2 modification            
            #elif score == scores[(next_node,direction)] : 
            #    previous[(next_node,direction)].append((node,direction))

    # Find the node that has the smallest value
    filterered_nodes = [(node,dir) for (node,dir) in scores if node == end]
    min_node = (end,facing)
    min_score = scores[min_node]
    for node in filterered_nodes : 
        if scores[node] < min_score : 
            min_node = node 
            min_score = scores[node]

    best =  min([(scores[key]) for key in filterered_nodes]) 
    print(f"Best node {min_node} with score {min_score}")        
    
    return min_score,min_node,scores

def get_all_nodes(grid) : 
    start = find_start(grid)
    end = find_end(grid)
    nodes = []
    east = (0,1)
    dir = east
    for i in range(4) : 
        nodes.append((start,dir))
        nodes.append((end,dir))
        dir = rotate(dir,1)


    for i in range(len(grid)): 
        for j in range(len(grid)) : 
            if grid[i][j] =='.':
                dir = east
                for i in range(4) : 
                    nodes.append(((i,j),dir))
                    dir = rotate(dir,1)
    return nodes


### Part 1 ###
# Get start and end positions
start = find_start(grid)
end = find_end(grid)
print(start,end)
# Run dijkstra on, all_scores also contains directions 
best_score_from_start,min_node, all_scores_from_start = find_best_path(grid,start,end,(0,1))
print(f"Part 1 {best_score_from_start}")
### Part 2 ###
# Run dijkstra from end to start facing backwards
reverse_direction = min_node[1]
reverse_direction = tuple(-v for v in reverse_direction)
best_score_from_end,_, all_scores_from_end  = find_best_path(grid,end,start,reverse_direction )

for key in all_scores_from_start : 
    print(key)



path = set()
for node in all_scores_from_start : 
    reverse_direction = tuple(-v for v in node[1])
    reverse_node = (node[0],reverse_direction)
    if  all_scores_from_start[node]+all_scores_from_end[reverse_node] == best_score_from_start :
        path.add(node[0])

print(len(path))


