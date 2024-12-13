import re
import pprint
linear_equations = []
part2 = True

with open("input.txt") as f :
    input = f.readlines()
    number_pattern = r"\d+"
    row = 0 
    while row <  len(input)-2 :
        group = [line.strip() for line in input[row:row+3] if line.strip() ]
        if len(group) != 3 :
            row += 1 
            continue
        first_transformation = re.findall(number_pattern,group[0])
        second_transformation = re.findall(number_pattern,group[1])
        target = re.findall(number_pattern,group[2])

        first_row = [int (first_transformation[0]),int (second_transformation[0])]
        second_row = [int (first_transformation[1]),int (second_transformation[1])]

        matrix = [ first_row,second_row]
        vector = [int(target[0]),int(target[1])]     
        if part2 : 
            vector = [val+10000000000000 for val in vector]

        linear_equations.append([matrix,vector])
        row +=3

def multiply(matrix,vector) : 
    [[a,b],[c,d]] = matrix
    [x,y] = vector
    return [a*x+b*y,c*x+d*y]

def solve(linear_equation) : 
    [[a,b],[c,d]] = linear_equation[0]
    vector = linear_equation[1]
    determinant = a*d-b*c
    inverse_matrix = [[d,-b],[-c,a]] #not really the inverse, should divide by the determinant but we'll save that for later
    solution = multiply(inverse_matrix,vector)
    solution = [val/determinant for val in solution]
    return solution

def cost(solution):
    return 3*solution[0]+solution[1]
    


total_cost = 0
for le in linear_equations : 
    sol = solve(le)
    if all(val.is_integer() for val in sol): 
        print(sol)
        total_cost +=cost(sol)
print(total_cost)




