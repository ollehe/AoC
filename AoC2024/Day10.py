from collections import deque

topographic_map = []
with open('input.txt') as f : 
    lines = f.readlines()
    lines = [list(line.strip()) for line in lines ]
    topographic_map = [list([int(val) for val in line])  for line in lines]

def in_bounds(point,topographic_map) : 
    return point[0] >= 0 and point[1] >= 0 and point[0] < len(topographic_map) and point[1] <  len(topographic_map[1])

def find_trailheads(topographic_map) : 
    list_of_trailheads = []
    for row,list_of_heights in enumerate(topographic_map) : 
        for col,height in enumerate(list_of_heights) : 
            if height == 0 : 
                list_of_trailheads.append((row,col))
    return list_of_trailheads

def add(first, second) : 
    return ( first[0]+second[0],first[1]+second[1])

def score_trailhead(trailhead, topographic_map) : 
    queue = deque()
    queue.append(trailhead)
    explored = set()
    shifts = [(0,1),(1,0),(-1,0),(0,-1)]
    explored.add(trailhead)
    score = 0
    while queue : 
        current_position = queue.pop()
        current_height = topographic_map[current_position[0]][current_position[1]]
        #print(f"At {current_position} and height {current_height}")
        for shift in shifts : 
            new_position = add(current_position,shift)
            if in_bounds(new_position,topographic_map) : 
                new_height = topographic_map[new_position[0]][new_position[1]]
                if new_position not in explored and (new_height-current_height) == 1: 
                    #print(f"Can reach {new_position} at height {new_height}")
                    explored.add(new_position)
                    queue.append(new_position)
                    if new_height == 9 : 
                        score += 1

    return score

def score_trailhead_p2(trailhead, topographic_map) : 
    # Initalize
    stack = []
    stack.append(trailhead)
    explored = set()
    shifts = [(0,1),(1,0),(-1,0),(0,-1)]
    score = 0
    # DFS
    while stack : 
        current_position = stack.pop()
        current_height = topographic_map[current_position[0]][current_position[1]]
        if current_height not in explored : 
            explored.add(current_position)
            for shift in shifts : 
                new_position = add(current_position,shift)
                if in_bounds(new_position,topographic_map) : 
                    new_height = topographic_map[new_position[0]][new_position[1]]
                    if  (new_height-current_height) == 1: 
                        explored.add(new_position)
                        stack.append(new_position)
                        if new_height == 9 : 
                            score += 1
    return score



def total_score(topographic_map,part2) : 
    trailheads = find_trailheads(topographic_map)
    if part2 : 
        scores = [score_trailhead_p2(trailhead,topographic_map) for trailhead in trailheads]
    else :
        scores = [score_trailhead(trailhead,topographic_map) for trailhead in trailheads]

    return sum(scores)


print(total_score(topographic_map,False))
print(total_score(topographic_map,True))
