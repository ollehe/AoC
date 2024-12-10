import pprint


def find_starting_point(grid):
    for row in range(len(grid)) : 
        for col in range(len(grid[0])) :
            if grid[row][col] == '^' :
                return [row,col]
    print("Whopsie! Couldn't find starting")
    return [-1,-1]

def find_blocking_objects(grid) : 
    blocking_objects = []
    for row in range(len(grid)) : 
        for col in range(len(grid[0])) :
            if grid[row][col] == '#' :
                blocking_objects.append([row,col])
    return blocking_objects

def in_bounds(row,col,grid) : 
    return row>=0 and col >= 0 and row < len(grid) and col < len(grid[0])

def move(grid, part2) : 
    path = set()
    starting_pos = find_starting_point(grid)
    path.add(tuple(starting_pos))
    moves = 1
    direction = [-1,0]
    current_row = starting_pos[0]
    current_col = starting_pos[1]


    while( in_bounds(current_row,current_col,grid)) : 
        #print(f"Position {current_row,current_col}, Direction {direction}")
        current_row += direction[0]
        current_col += direction[1]
        if in_bounds(current_row,current_col,grid) and grid[current_row][current_col] == '#' : 
            # move back
            current_row -= direction[0]
            current_col -= direction[1]
            # rotate
            tmp = direction[0]
            direction[0] = direction[1]
            direction[1] =  -tmp 
            #print(f"Object encountered at {current_row,current_col}, switching direction {direction}")

        elif in_bounds(current_row,current_col,grid) and grid[current_row][current_col] != '#': 
            moves += 1
            path.add((current_row,current_col))
        else :
            break 
    
    if part2 :
        obstructions = 0  
        path.remove(tuple(starting_pos))
        
        for pos in path : 
            # Change point on path 
            grid[pos[0]][pos[1]] = '#'
            direction = [-1,0]
            current_row = starting_pos[0]
            current_col = starting_pos[1]
            counter = {}

            while( in_bounds(current_row,current_col,grid)) : 
                #print(f"Position {current_row,current_col}, Direction {direction}")
                current_row += direction[0]
                current_col += direction[1]
                if in_bounds(current_row,current_col,grid) and grid[current_row][current_col] == '#' : 
                    # move back
                    current_row -= direction[0]
                    current_col -= direction[1]
                    # rotate
                    tmp = direction[0]
                    direction[0] = direction[1]
                    direction[1] =  -tmp 
                    #print(f"Object encountered at {current_row,current_col}, switching direction {direction}")

                elif in_bounds(current_row,current_col,grid) and grid[current_row][current_col] != '#': 
                    moves += 1
                    if ((current_row,current_col),tuple(direction)) not in counter : 
                        counter[((current_row,current_col),tuple(direction))]  = 1
                    else : 
                        obstructions +=1
                        break
                else :
                    break             
            grid[pos[0]][pos[1]] = '.'
        return obstructions


    return len(path)

grid = []
with open('input.txt') as f : 
    grid = [list(line.strip()) for line in f.readlines()]


print("Part 1", move(grid,False), "Part 2",move(grid, True) )