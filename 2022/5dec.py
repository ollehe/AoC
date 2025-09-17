# -*- coding: utf-8 -*-
"""
Created on Mon Dec  5 08:06:12 2022

@author: ollee
"""

import re

def clean_input(lines): #removes line breaks from loaded file
    new_line = []
    for line in lines:
        new_line.append(line.strip()) 
    return new_line


def stacks_and_orders(l):

    # First find index at which to split the list
    n=len(l)
    split_ind=0
    for i in range(n):
        # Find where to split the data into the stacks and the orders        
        if len(l[i].strip())==0:
            split_ind=i
            break
        
    # Construct stacks    
    regex=re.compile('[*\w]') # Match all expressions
    stack_list = l[split_ind-1]

    nbr_of_stacks = len( regex.findall( stack_list ) )
    
    stacks=[ [] for i in range(nbr_of_stacks) ] # Empty stacks for now
    
    for line in reversed(l[:split_ind-1]): # Build from bottom and up
        stack_number=regex.finditer( l[split_ind-1] ) # Restart iterator
        # Get all matches
        
        for stack in stack_number:
            matches = regex.finditer(line) #Restart iterator
            stack_id = int(stack.span()[0])
            stack_nbr=int(stack_list[stack_id])
            for match in matches :
                match_id=int(match.span()[0])
                if stack_id==match_id : 
                    stacks[stack_nbr-1].append(line[match_id])
            
                
    # for stack in stacks:
    #     print(stack)
        
    # Construct orders
    # A numeric list where each line is of the form:
    # [x,y,z ] which means: move x from y to z
    orders=[]
    regex=re.compile('\d\d?') # Match all expressions
    for line in l[split_ind+1:]: 
        order = regex.findall(line)
        order = list(map(int, order))
        orders.append(order)
        
    return [stacks, orders]
        

def move_stack(stacks, order) : 
    # Order is a numeric list 
    # where each line is of the form:
    # [x,y,z ] which means: move x from y to z
    number_of_moves= order[0] 
    transfer = stacks[order[1]-1]
    target = stacks[order[2]-1]
    
    for i in range(number_of_moves) :
        crate = transfer.pop()
        target.append(crate)    
    stacks[order[1]-1]=transfer
    stacks[order[2]-1]=target
    return stacks


def move_stack_9001(stacks, order) : 
    # Order is a numeric list 
    # where each line is of the form:
    # [x,y,z ] which means: move x from y to z
    number_of_moves= order[0] 
    transfer = stacks[order[1]-1]
    target = stacks[order[2]-1]
    crates = transfer[ len(transfer)-number_of_moves:]
    for crate in crates : 
        target.append(crate)
        
    stacks[order[1]-1]=transfer[:len(transfer)-number_of_moves]
    stacks[order[2]-1]=target
    
    return stacks




with open("5dec.txt") as f:
    lines = f.readlines()
    a = stacks_and_orders(lines)
    stacks = a[0]
    orders = a[1]
    for order in orders :
        stacks = move_stack_9001(stacks, order)
    s=""
    for stack in stacks :
        s=s+stack[len(stack)-1]
    print(s)
































