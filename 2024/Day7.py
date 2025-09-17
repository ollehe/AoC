import re
import pprint

expressions = []
upper_bounds = []
with open('input.txt') as f : 
    lines = f.readlines()
    expressions = [line.strip().replace(': ','==') for line in lines]
    upper_bounds = [line.strip().replace(': ','>=') for line in lines]

def determine_correctness(equation) : 
    # Expression is of the form "a==a1 b1 c ..." 
    max_val = equation.split('==')[0]
    expression = equation.split('==')[1]
    number_pattern = r'\d+'
    numbers = re.findall(number_pattern,expression)
    if len(numbers) > 1 : 
        if int(max_val) < int(numbers[0]) :
            return False
        else :

            multiplied = equation.replace(f"{numbers[0]} {numbers[1]}", str(eval(f"{numbers[0]}*{numbers[1]}")),1)
            added = equation.replace(f"{numbers[0]} {numbers[1]}", str(eval(f"{numbers[0]}+{numbers[1]}")),1)
            #Part 2
            concatenated = equation.replace(f"{numbers[0]} {numbers[1]}", f"{numbers[0]}{numbers[1]}",1)
            # Comment out the last or statement for part 1
            return determine_correctness(multiplied) or determine_correctness(added) or determine_correctness(concatenated)
    else : 
        return eval(equation)

correct_evaluations = 0
value = 0
for pair in zip(expressions,upper_bounds) :
    expression = pair[0]
    upper_bound = pair[1]
    if determine_correctness(expression) : 
        correct_evaluations += 1
        value += int(expression.split('==')[0])


print("Correct evaluations",correct_evaluations, "total value: ", value)