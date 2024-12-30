from collections import deque
from itertools import combinations,permutations
import time
import pprint
### Parsing ###
states = []
gates = []
with open('input.txt') as f : 
    input = f.readlines()
    input = [line.strip() for line in input]
    empty_index = input.index("")

    states = input[:empty_index]
    states = [val.split(':') for val in states]
    states = { val[0].strip() :  int(val[1].strip()) for val in states  }

    gates = input[empty_index+1:] 
    gates = [gate.strip().split('->') for gate in gates]
    gates = [[gate[0].strip().split(), gate[1].strip()] for gate in gates]



### Part 1 methods: 
def try_evaluate(gate,states) : 
    [operation,target]  = gate
    first = operation[0]
    operand = operation[1]
    second = operation[2]
    if first not in states or second not in states :
        return False
    if operand == 'AND' : 
        states[target] = states[first] and states[second]
        return True
    elif operand == 'OR' :
        states[target] = states[first] or states[second]
        return True
    elif operand == 'XOR' :
        states[target] = states[first] ^ states[second]
        return True
    return False

def evaluate_all_states(gates,states) : 
    queue = deque()
    for gate in gates : 
        queue.append(gate)
    while queue : 
        current = queue.popleft()
        if not try_evaluate(current,states) : 
            queue.append(current)

def compute_binary(states, starting_character) :
    zs = {key : states[key] for key in states if key.startswith(starting_character)}
    
    sorted_zs = {k: zs[k] for k in sorted(zs)}
    sum = 0
    for exponent,z in enumerate(sorted_zs) : 
        sum += sorted_zs[z]*2**exponent

    return sum
# Part 2 methods. 
# Uses the fact that this problem is just a faulty a Ripple Carry Adder  
# The following works I believe : 
# If the output of a gate is z, then the operation has to be XOR unless it is the last bit.
# If the output of a gate is not z and the inputs are not x, y then it has to be AND / OR, but not XOR.
# If you have a XOR gate with inputs x, y, there must be another XOR gate with this gate as an input. Search through all gates for an XOR-gate with this gate as an input; if it does not exist, your (original) XOR gate is faulty.
# Similarly, if you have an AND-gate, there must be an OR-gate with this gate as an input. If that gate doesn't exist, the original AND gate is faulty.


def find_faulty(gates) : 
    faulty = []
    for [input,output] in gates:
        # If the output of a gate is z, then the operation has to be XOR unless it is the last bit.

        if output.startswith('z') and input[1] != 'XOR' and output != 'z45': 
            faulty.append([input,output])
        # If the output of a gate is not z and the inputs are not x, y then it has to be AND / OR, but not XOR.
        if (not output.startswith('z') and not 
            ( input[0].startswith(('x','y')) or input[2].startswith(('x','y')) ) 
            and input[1] == 'XOR'):
            faulty.append([input,output])
        # If you have a XOR gate with inputs x, y, there must be another XOR gate with this gate as an input. (does not hold for x00,y00)
        # Search through all gates for an XOR-gate with this gate as an input; if it does not exist, your (original) XOR gate is faulty.
        if  (input[1] == 'XOR' and input[0].startswith(('x','y')) 
             and input[2].startswith(('x','y')) 
             and not (input[0].endswith('00') or input[0].endswith('00'))) : 
            exists_as_input = False
            for gate in gates : 
                if output in gate[0] and gate[0][1] == 'XOR':
                    exists_as_input = True
            if not exists_as_input : 
                faulty.append([input,output])
        # Similarly, if you have an AND-gate, there must be an OR-gate with this gate as an input. If that gate doesn't exist, the original AND gate is faulty.
        if  (input[1] == 'AND' and input[0].startswith(('x','y')) 
             and input[2].startswith(('x','y')) 
             and not (input[0].endswith('00') or input[0].endswith('00'))) : 
            exists_as_input = False
            for gate in gates : 
                if output in gate[0] and gate[0][1] == 'OR':
                    exists_as_input = True
            if not exists_as_input : 
                faulty.append([input,output])

    return faulty





initial_states = states.copy()

x = compute_binary(states,'x')
y = compute_binary(states,'y')
evaluate_all_states(gates,states)
z = compute_binary(states,'z')
print("Part 1:",z)

faulty = find_faulty(gates)
faulty_outputs = ','.join(sorted(set([gate[1] for gate in faulty]))) 
print("Part 2:",faulty_outputs)

