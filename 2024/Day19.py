import string
from collections import deque

def generate_map(patterns) : 
    pattern_map = {}
    for pattern in patterns : 
        if pattern[0] not in pattern_map : 
            pattern_map[pattern[0]] = [pattern]
        else :
            pattern_map[pattern[0]].append(pattern)
    return pattern_map

#This part is superfluous (and a bit buggy)
def find_design_match(string, pattern_map) : 
    queue = deque()
    explored = set()
    queue.append(string)
    # Do a kind of BFS 
    count  = 0
    while queue : 
        current = queue.pop()
        # Get neighbors
        if current[0] in pattern_map : 
            possible_patterns = pattern_map[current[0]]
            for pattern in possible_patterns : 
                if pattern == current : 
                    return True 
                elif pattern == current[:len(pattern)] : 
                    queue.append(current[len(pattern):])
    return False        



memo ={}
def count_possibilities(string,pattern_map) : 
    if string in memo :
        return memo[string]
    if not string :
        return 1
    value = 0
    if string[0] in pattern_map : 
        for pattern in pattern_map[string[0]] : 
            if pattern == string[:len(pattern)] :         
                value += count_possibilities(string[len(pattern):],pattern_map)
    memo[string] = value
    return value

designs =[]
patterns =[]
with open("input.txt") as f: 
    patterns = []
    input = f.readlines()
    patterns =  [ str.strip() for str in input[0].strip().split(',')]
    designs = [line.strip() for line in input[2:]]

pattern_map = generate_map(patterns)
total_number = 0

print("Part 1: ",sum ( [count_possibilities(design,pattern_map)>0 for design in designs ]  ) )
print("Part 2:", sum ( [count_possibilities(design,pattern_map) for design in designs  ]  )   )

