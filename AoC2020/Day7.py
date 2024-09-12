import pprint as pretty
def parse_instructions(file) : 
    with open(file) as f : 
        lines = f.readlines()
        # Idea is to make a dictionary where each type of bag points to the bags it contains
        # effectively constructing a tree with parent bags and children bags
        instructions = dict()
        for line in lines: 
            # Here we do a bunch of parsing just to make it more uniform
            cleaned_line = line.rstrip('s.\n')
            split_line = cleaned_line.split("contain") 
            # this is the bag that contains the other bags
            parent_bag = split_line[0].rstrip('s ')
            instructions[parent_bag] = []
            # print("Bag", parent_bag, "contains:")

            # pretty.pprint(parent_bag)
            children_bags =split_line[1].strip().split(',')
            # loop over 
            for bag in children_bags : 
                cleaned_bag = bag.strip().rstrip('s')
                if cleaned_bag != "no other bag" :
                    # pretty.pprint([cleaned_bag[0], cleaned_bag[1:].strip() ])
                    instructions[parent_bag].append([cleaned_bag[1:].strip(),int(cleaned_bag[0])])




    return instructions

def find_bag_types(luggage_instructions, given_bag) : 
    #first initialize 
    list_of_bags = []
    set_of_unique_bags = set()
    counter = 0
    list_of_bags.append(given_bag)
    while list_of_bags : 
        # pretty.pprint(list_of_bags)
        current_bag = list_of_bags.pop()
        for key in luggage_instructions :
            bags = [] 
            for weight in luggage_instructions[key]:
                bags.append(weight[0])
            if current_bag in bags:
                list_of_bags.append(key)
                set_of_unique_bags.add(key)

    print("Number of unique bags containing", given_bag, ": ",len(set_of_unique_bags))

def number_of_contained_bags(luggage_instructions,bag) :
    if not luggage_instructions[bag] :
        return 1
    sum = 0
    for child in luggage_instructions[bag] : 
        sum = sum + child[1]*number_of_contained_bags(luggage_instructions, child[0])
    return sum+1
instructions = parse_instructions("Day7.txt")
# pretty.pprint(instructions)
find_bag_types(instructions,"shiny gold bag")
n = number_of_contained_bags(instructions,"shiny gold bag")
#Off by one,
print("Number of bags that one shiny gold bag contains is: ",n-1)