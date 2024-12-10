page_orderings = {}
updates = []
# el parse del dato
with open('input.txt') as f : 
    lines = f.readlines()
    for line in lines : 
        if '|' in line :
            [first, second] =line.split('|')  
            if int(first) in page_orderings : 
                page_orderings[int(first)].append(int(second))
            else :
                page_orderings[int(first)] = [int(second)]
        elif ',' in line : 
            update = [int(elem) for elem in line.split(',')]
            updates.append(update)

def is_valid(update, ordering) : 
    for pos in range(1,len(update)) : 
        if update[pos] in ordering :
            for prev in range(pos) : 
                    if update[prev] in ordering[update[pos]] : 
                        return False 
    return True

def el_bubblo_sorto(update,ordering) : 
    n = len(update) 
    for i in range(n) : 
            for j in range(n-i-1) : 
                if update[j+1] in ordering and  update[j] in ordering[update[j+1]] : 
                    update[j], update[j + 1] = update[j + 1], update[j]



valid_sum = 0
reordered_sum = 0
for update in updates : 
    if not is_valid(update,page_orderings) : 
        el_bubblo_sorto(update,page_orderings)
        reordered_sum += update[len(update)//2]
    else : 
        valid_sum += update[len(update)//2]
print(valid_sum)
print(reordered_sum)   
