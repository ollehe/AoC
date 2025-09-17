### Part 1 Naive solution
def unwrap_file(file) : 
    unwrapped_file = []
    id = 0
    free_space = False  
    for pos in range(len(file)) : 
        if pos%2 == 0 : 
            for i in range(file[pos]) :
                unwrapped_file.append(id)
            id +=1 
        else :
            for i in range(file[pos]) : 
                unwrapped_file.append('.')
    return unwrapped_file

def get_next_empty(current_position, unwrapped_file) :
    for i in range(current_position,len(unwrapped_file)) :
        if unwrapped_file[i] == '.' : 
            return i
    return len(unwrapped_file)

def compact(file) : 
    unwrapped = unwrap_file(file) 
    #start of empty block position is given by the sum of the of the odd 
    empty_block_pos = get_next_empty(0,unwrapped)
    end_pos = len(unwrapped)-1
    while empty_block_pos < end_pos : 
        if unwrapped[end_pos] != '.' :       
            unwrapped[empty_block_pos] = unwrapped[end_pos]
            unwrapped[end_pos] = '.'
        end_pos -= 1
        empty_block_pos = get_next_empty(empty_block_pos,unwrapped)
        
    return unwrapped

def check_sum(file) : 
    #compact the file
    compacted = compact(file)
    #first compact the file.
    sum = 0
    for i in range(len(compacted) ):
        try : 
            sum += i*compacted[i]
        except : 
            continue
    return sum

# Part 2. Can probably use this part to solve Part 1 as well. 
# Represent each file by a collection (position, size, id), position is the start of the block, size is ... size, 
# id is either integer or None (if block is empty)
def create_expanded_file_system(file_system) : 
    pos = 0
    expanded = []
    for i, val in enumerate(file_system):
        if i % 2 == 0:  # file size
            size = val
            file_id = i // 2
            expanded.append([pos, size, file_id])
        else:  # free space
            size = val
            expanded.append([pos, size, None])  # None represents free space
        pos += size  # move to the next position
    return expanded

def find_next_empty_block(valid_size, expanded_file_sys) : 
    #file = (pos,size,id)
    for i in range(len(expanded_file_sys)) : 
        file = expanded_file_sys[i] 
        size = file[1]
        if size >= valid_size and file[2] is None: 
            return i
    return None

def compact_file_system(expanded_file_sys) : 

    for file in reversed(expanded_file_sys) : 

        #file = (pos,size,id)
        pos = file[0]
        size = file[1]
        id = file[2]

        # If we are not at an empty block
        if id is not None : 
            empty_block_pos = find_next_empty_block(size, expanded_file_sys)
            # If we found a possible empty block
            if empty_block_pos is not None : 
                empty_block = expanded_file_sys[empty_block_pos]

                #Check if position available (i.e) the position of the empty block is smaller than the current 
                if empty_block[0]<pos :
                    # Change the ID to None of the current file we are editin 
                    file[2] = None
                    # fill the block and possibly create a new one.
                    filled_block = [empty_block[0], size, id ]
                    expanded_file_sys[empty_block_pos] = filled_block
                    # and add the possibly remaining size of the block (this generates a lot of small empty blocks)
                    if empty_block[1]-size >0 : 
                        new_empty_block = [empty_block[0]+size,empty_block[1]-size,None]
                        expanded_file_sys.append(new_empty_block) 
                        #Sort the list according to position of the block, this is what slows it down. Using something like a queue is probably faster
                        expanded_file_sys.sort(key = lambda x : x[0])           
    expanded_file_sys = [file for file in expanded_file_sys if file[2] is not None]

def check_sum_part2(compacted_file_system): 
    sum = 0
    for file in compacted_file_system : 
        #file = (pos,size,id)
        pos = file[0]
        size = file[1]
        id = file[2]
        if id is not None : 
            for i in range(pos,pos+size) : 
                sum += id * i
    return sum

file = []
with open("input.txt") as f  : 
    file = [int(char) for char in list(f.readline())]
#part 1 brute force
print("Part 1: ", check_sum(file))

#part 2 not brute force but slow
expanded = create_expanded_file_system(file)
compact_file_system(expanded)
print("Part 2: ",check_sum_part2(expanded))