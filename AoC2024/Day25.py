def convert_to_heightmap(schematic, from_top) : 
    height_map = [0 for x in schematic[0]]
    for i,row in enumerate(schematic) : 
        for j,char in enumerate(row) : 
            if char == '#' : 
                if from_top : 
                    height_map[j] = max(i,height_map[j])
                else : 
                    height_map[j] = max(len(schematic)-1-i,height_map[j])
    return height_map
def overlap(key,lock) : 
    for (x,y) in list(zip(key,lock)) : 
        if (x+y) > 5 : 
            return True
    return False

with open('input.txt', 'r') as file:
    input_text = file.read()

blocks = input_text.strip().split("\n\n")
keys_and_locks = [block.split("\n") for block in blocks]

keys = [ key for key in keys_and_locks if '#' in key[0] ]
locks = [lock for lock in keys_and_locks if '#' not in lock[0]]

keys = [convert_to_heightmap(key,True) for key in keys]
locks = [convert_to_heightmap(lock,False) for lock in locks]

possible_combinations = 0
for key in keys : 
    for lock in locks : 
        if not overlap(key,lock) :
            print( f"Key {key} has no overlap with {lock}")
            possible_combinations+=1
print(possible_combinations)
