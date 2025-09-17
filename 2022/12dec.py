# -*- coding: utf-8 -*-
"""
Created on Mon Dec 12 13:27:57 2022

@author: ollee
"""
import numpy as np

import sys
 
from queue import PriorityQueue
import math



    
def convert_to_height_map(lines) : 
    rows = len(lines)
    cols = len(lines[0])
    grid = np.zeros((rows,cols))
    start = "S"
    start_pos =[]
    end = "E"
    end_pos = []
    for row in range(rows) :
        line = lines[row]
        for col in range(cols) :
            if line[col] == start or line[col] == 'a': 
                grid[row,col] = 0
                start_pos.append([row,col])
            elif line[col] == end :
                grid[row,col] = ord('z')-ord('a')
                end_pos =[row,col]
            else :
                grid[row,col] = ord(line[col])-ord('a')
    return [start_pos, end_pos , grid]
        
def clean_input(lines) : 
    # Removes line breaks from loaded file
    new_line = []
    for line in lines:
        new_line.append(line.strip()) 
    return new_line

with open("12dec.txt") as f:
    lines = f.readlines()
    lines = clean_input(lines)
    # for line in lines :
    #     print(line)
    l = convert_to_height_map(lines)

    start = l[0]
    end = l[1]
    grid = l[2]

    rows = grid.shape[0]
    # print(rows)
    cols = grid.shape[1]
    # print(cols)
    graph = {}
    x_neighbour = [-1,1]
    y_neighbour = [-1,1]
    
    source = str( (end[0],end[1]) )
    targets = []
    for point in start :
        # print(point)
        targets.append(str( (point[0],point[1]) ) )
    
    for row in range(rows) :
        for col in range(cols) :
            # print( "At node: "+  str((row,col)) )

            neighbour_dict = dict()
            for x in x_neighbour : 
                if row+x >= 0 and row+x <= rows-1 : 
                    if  -(grid[row+x,col]-grid[row,col]) <= 1 : #remove -1 sign for part 1
                        neighbour_dict.update({ str((row+x,col)) : 1 } )
            for y in y_neighbour :
                if col+y >= 0 and col+y <= cols-1:
                    if  -(grid[row,col+y]-grid[row,col]) <= 1 :
                        neighbour_dict.update({ str((row,col+y)) : 1 } )
            graph.update({ str((row,col)) : neighbour_dict  }) 
    
    unvisited = graph
    shortest_distances = {}
    route = [] 
    path_nodes = {}
    
    for nodes in unvisited:
        shortest_distances[nodes] = math.inf
    shortest_distances[source] = 0
    
    while(unvisited):
            min_node = None
            for current_node in unvisited: 
                if min_node is None:
                    min_node = current_node        
                elif shortest_distances[min_node] > shortest_distances[current_node]:
                    min_node = current_node
            for node,value in unvisited[min_node].items():
                if value + shortest_distances[min_node] < shortest_distances[node]:  
                    shortest_distances[node] = value + shortest_distances[min_node]
                    path_nodes[node] = min_node
            unvisited.pop(min_node)
    
    smallest = math.inf
    for target in targets :
        destination = target
        node = destination
        while node != source:
            try:
                route.insert(0, node)
                node = path_nodes[node]
            except Exception:
                # print('Path not reachable')
                break
        route.insert(0, source)
        # print(destination)
        # print(shortest_distances[destination])
        smallest = min(shortest_distances[destination],smallest )
    print(smallest)