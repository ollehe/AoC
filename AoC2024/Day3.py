import re
outer_pattern = r"do\(\)(.*?)don't\(\)"
inner_pattern = r"mul\(\d+,\d+\)"

part1 = []
part2 = []

with open("input.txt") as f : 
    lines = f.readlines()
    lines = [line.strip() for line in lines]
    data = "do()"+''.join(lines)+"don't()"
    segments = re.findall(outer_pattern,data)
    matches = re.findall(inner_pattern,data)
    for match in matches : 
        part1.append(match)
    active_evaluations = ''.join(segments)
    matches = re.findall(inner_pattern,active_evaluations)
    for match in matches : 
        part2.append(match)


        
def evaluate(expression) : 
    pattern = r"\d+"
    matches = re.findall(pattern, expression)
    val = 1
    for match in matches : 
        val *= int(match)
    return val

part1_values = [evaluate(expr) for expr in part1]
part2_values = [evaluate(expr) for expr in part2]

print(sum(part1_values))
print(sum(part2_values))