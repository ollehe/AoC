import networkx as nx
import re
from collections import defaultdict
import time
# Start with hardcoding the graphs
# +---+---+---+
# | 7 | 8 | 9 |
# +---+---+---+
# | 4 | 5 | 6 |
# +---+---+---+
# | 1 | 2 | 3 |
# +---+---+---+
#     | 0 | A |
#     +---+---+
numpad_dict = {
    '7': ['4', '8'],     '8': ['7', '5', '9'],     '9': ['8', '6'],
    '4': ['7', '5', '1'], '5': ['6', '8', '4', '2'], '6': ['9', '5', '3'],
    '1': ['4', '2'],    '2': ['3', '5', '1', '0'], '3': ['6', '2', 'A'],
    '0': ['A', '2'],         'A': ['3', '0']
}
numpad_graph = nx.Graph(numpad_dict)
#     +---+---+
#     | ^ | A |
# +---+---+---+
# | < | v | > |
# +---+---+---+
dpad_dict = {
    '^': ['v', 'A'], 'A': ['^', '>'],
    '<': ['v'],     'v': ['<', '^', '>'], '>': ['A', 'v']
}
dpad_graph = nx.Graph(dpad_dict)

numpad_to_dpad_moves = {
    ('A', '0'): '<', ('A', '3'): '^',
    ('0', '2'): '^', ('0', 'A'): '>',
    ('1', '4'): '^', ('1', '2'): '>',
    ('2', '1'): '<', ('2', '5'): '^', ('2', '3'): '>', ('2', '0'): 'v',
    ('3', '2'): '<', ('3', '6'): '^', ('3', 'A'): 'v',
    ('4', '7'): '^', ('4', '5'): '>', ('4', '1'): 'v',
    ('5', '4'): '<', ('5', '8'): '^', ('5', '6'): '>', ('5', '2'): 'v',
    ('6', '5'): '<', ('6', '9'): '^', ('6', '3'): 'v',
    ('7', '8'): '>', ('7', '4'): 'v',
    ('8', '7'): '<', ('8', '5'): 'v', ('8', '9'): '>',
    ('9', '8'): '<', ('9', '6'): 'v'
}

dpad_to_dpad_moves = {
    ('A', '^'): "<", ('A', '>'): "v",
    ('^', 'A'): ">", ('^', 'v'): "v",
    ('<', 'v'): ">",
    ('v', '<'): "<", ('v', '^'): "^", ('v', '>'): ">",
    ('>', 'v'): "<", ('>', 'A'): "^",
}


def find_all_shortest_paths(graph, moves):
    paths = defaultdict(list)
    for start in graph.nodes():
        for end in graph.nodes():
            if start != end:
                for path in nx.all_shortest_paths(graph, start, end):
                    sequence = "".join([moves[(first, second)]
                                       for first, second in zip(path, path[1:])])
                    paths[start+end].append(sequence)
    return paths


shortest_numpad_paths = find_all_shortest_paths(
    numpad_graph, numpad_to_dpad_moves)
shortest_dpad_paths = find_all_shortest_paths(dpad_graph, dpad_to_dpad_moves)


memo = {}


def find_minimum(current_level, sequence, maximal_level):

    # If we've reached the top of the recursion we return the length of the sequence
    if current_level == maximal_level + 1:
        return len(sequence)
    # DP part
    key = (current_level, sequence, maximal_level)
    if key in memo:
        return memo[key]

    # Otherwise we call recursively.
    # First we use decide which dictionary of shortest paths we use.
    if current_level == 0:
        shortest_paths = shortest_numpad_paths
    else:
        shortest_paths = shortest_dpad_paths

    minimal_presses = 0
    # For each pair in the sequence (the robot always start at A) hence the prepended A
    for first, second in zip('A'+sequence, sequence):
        # If minimal_sequences is empty it means that we stay at the same spot (and so the list of shortest paths is empty) 
        # in which we just press the button once more (in the else statement)
        # The appended A is due to the fact that we must press the button
        minimal_sequences = [find_minimum(
            current_level+1, seq+'A', maximal_level) for seq in shortest_paths[first+second]]
        if minimal_sequences:
            minimal_presses += min(minimal_sequences)
        else:
            minimal_presses += 1

    memo[key] = minimal_presses
    return minimal_presses


def recursive_solve(codes, lvl):
    pattern = r'\d+'
    total_complexity = 0
    for code in codes:
        val = int(re.findall(pattern, code)[0])
        total_complexity += val*find_minimum(0, code, lvl)
    return total_complexity


codes = ["029A",
         "980A",
         "179A",
         "456A",
         "379A"]


start = time.time()
print("Part 1:", recursive_solve(codes, 2))
end = time.time()
print("Total time: ", end-start)

start = time.time()
print("Part 2:", recursive_solve(codes, 25))
end = time.time()
print("Total time: ", end-start)
