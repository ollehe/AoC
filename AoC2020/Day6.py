import pprint as pretty
def grab_answers(string, part2) : 
    list_of_set_of_answers = []
    with open(string) as file: 
        lines = file.readlines()
        list_of_answers =[]

        string_of_answers =""
        for line in lines : 
            stripped = line.strip("\n")
            if stripped:
                list_of_answers.append({char for char in stripped if char})
            # An empty line indicates a new set of answers. 
            if not stripped : 
                #this is part 1
                # set_of_answers = set.union(*list_of_answers)
                #this is part 2 comment whichever you solve
                set_of_answers = set.intersection(*list_of_answers)
                list_of_set_of_answers.append(set_of_answers)
                list_of_answers = []
        # takes care of the last set of answers, 
        # since the file doesn't have a last empty line 
        if list_of_answers : 
            # set_of_answers = {char for char in string_of_answers}
            #this is part 1
            # set_of_answers = set.union(*list_of_answers)
            #this is part 2 comment whichever you solve
            set_of_answers = set.intersection(*list_of_answers)

            list_of_set_of_answers.append(set_of_answers)
    return list_of_set_of_answers


answers = grab_answers("Day6.txt", True)

total_number_of_answers = 0

for answer in answers : 
    total_number_of_answers = total_number_of_answers+ len(answer)
print("Total number of answers",total_number_of_answers)