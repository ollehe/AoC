# -*- coding: utf-8 -*-
"""
Created on Fri Dec  9 13:57:53 2022

@author: ollee
"""
                
def clean_input(lines) : 
    # Removes line breaks from loaded file
    new_line = []
    for line in lines:
        new_line.append(line.strip()) 
    return new_line
def get_motions(lines) : 
    motions = []
    for line in lines : 
        l = line.split()
        l[1] = int(l[1])
        # print(l)
        motions.append(l)
    return motions       
def move_head(rope, order) : 
    # rope is a dictionary with 
    #    rope = { 'head' : ['tail',xpos,ypos],
    #              'tail' : ['head', xpos,ypos],
    #              'visited' : {(xpos,ypos)}
    #             }
    move_right = "R"
    move_left = "L"
    move_up = "U"
    move_down = "D"
    head = rope['head']
    xpos_head = head[1]
    ypos_head = head[2]
    tail = rope['tail']
    direction = order[0]
    number_of_steps = range(order[1])
    
    if direction == move_right :
        for i in number_of_steps :
            # print("Move head from: (" +str(xpos_head)+','+str(ypos_head) + ")" )
            xpos_head = xpos_head +1
            # print("To: (" +str(xpos_head)+','+str(ypos_head) + ")" )
            new_pos = {'head' : ['tail', xpos_head,ypos_head]}
            rope.update(new_pos)
            move_tail(rope)
    if direction == move_left :
        for i in number_of_steps :
            # print("Move head from: (" +str(xpos_head)+','+str(ypos_head) + ")" )
            xpos_head = xpos_head -1
            # print("To: (" +str(xpos_head)+','+str(ypos_head) + ")" )
            new_pos = {'head' : ['tail', xpos_head,ypos_head]}
            rope.update(new_pos)
            move_tail(rope)
    if direction == move_up :
        for i in number_of_steps :
            # print("Move head from: (" +str(xpos_head)+','+str(ypos_head) + ")" )
            ypos_head = ypos_head +1
            # print("To: (" +str(xpos_head)+','+str(ypos_head) + ")" )
            new_pos = {'head' : ['tail', xpos_head,ypos_head]}
            rope.update(new_pos)
            move_tail(rope)
    if direction == move_down :
        for i in number_of_steps :
            # print("Move head from: (" +str(xpos_head)+','+str(ypos_head) + ")" )
            ypos_head= ypos_head -1
            # print("To: (" +str(xpos_head)+','+str(ypos_head) + ")" )
            new_pos = {'head' : ['tail', xpos_head,ypos_head]}
            rope.update(new_pos)
            move_tail(rope)
def move_tail(rope) :        
    # check position of head and then update position accordingly
    # rope is a dictionary with 
    #     rope = { 'head' : ['tail',xpos,ypos],
    #              'tail' : ['head', xpos,ypos],
    #             'visited' : {(xpos,ypos)}
    #             }
    head = rope['head']
    tail = rope['tail']
    head_xpos = head[1]
    head_ypos = head[2]
    tail_xpos = tail[1]
    tail_ypos = tail[2]
    # print("Tail moves from (" + str(tail_xpos)+','+str(tail_ypos) +')')
    #check where head is in relation to rope: 
    xdiff = head_xpos-tail_xpos
    ydiff = head_ypos-tail_ypos        
    # If head and tail are in the same row but tail is more than one step away from head
    if abs(xdiff)>1 and ydiff ==0 :
        # move tail towards head
        if head_xpos>tail_xpos :
            tail_xpos =tail_xpos+1
        else:
            tail_xpos =tail_xpos-1
        new_pos = {'tail' : ['head', tail_xpos,tail_ypos]}
        rope.update(new_pos)
        new_visited = (tail_xpos,tail_ypos)
        visited_sites=rope['visited']
        visited_sites.add(new_visited)
        new_sites = {'visited':visited_sites}
        rope.update(new_sites)
        # print(" to: "+str(new_visited))

    # If head and tail are in the same column but tail is more than one step away from head
    elif abs(ydiff)>1 and xdiff ==0 :
        # move tail towards head
        if head_ypos>tail_ypos :
            tail_ypos =tail_ypos+1
        else:
            tail_ypos =tail_ypos-1
        new_pos = {'tail' : ['head', tail_xpos,tail_ypos]}
        rope.update(new_pos)
        new_visited = (tail_xpos,tail_ypos)
        visited_sites=rope['visited']
        visited_sites.add(new_visited)
        new_sites = {'visited':visited_sites}
        rope.update(new_sites)
        # print(" to: "+str(new_visited))

        
    # if the head and tail aren't touching and aren't in the same row or column, 
    # the tail always moves one step diagonally to keep up:

    elif abs(ydiff)>1 and abs(xdiff)>0 :
        # If position of head and tail is: 
        #  . . H   
        #  . . . 
        #  . T .
        # Move tail up one step and to the right one step 
        if head_ypos > tail_ypos and head_xpos>tail_xpos :
            tail_ypos = tail_ypos+1
            tail_xpos = tail_xpos+1
        # else if position of head and tail is: 
        #  H . .   
        #  . . .
        #  . T .
        # Move tail up one step and to the left one step 
        elif head_ypos > tail_ypos and head_xpos<tail_xpos :
            tail_ypos = tail_ypos+1
            tail_xpos = tail_xpos-1
        # else if position of head and tail is: 
        #  T . .   
        #  . . .
        #  . H .
        # Move tail down one step and to the right one step
        elif head_ypos<tail_ypos and head_xpos>tail_xpos :
            tail_ypos = tail_ypos-1
            tail_xpos = tail_xpos+1
        # else if position of head and tail is: 
        #  . . T   
        #  . . .
        #  . H .
        # Move tail down one step and to the left one step
        elif head_ypos<tail_ypos and head_xpos<tail_xpos :
            tail_ypos = tail_ypos-1
            tail_xpos = tail_xpos-1

        new_pos = {'tail' : ['head', tail_xpos,tail_ypos]}
        rope.update(new_pos)
        new_visited = (tail_xpos,tail_ypos)
        visited_sites=rope['visited']
        visited_sites.add(new_visited)
        new_sites = {'visited':visited_sites}
        rope.update(new_sites)
        # print(" to: "+str(new_visited))

        
    elif abs(xdiff)>1 and abs(ydiff)>0 :
        # Position of head and tail is: 
        #  . . .        
        #  . . H  
        #  T . .
        # Move tail up one step and to the right one step 
        if head_ypos > tail_ypos and head_xpos>tail_xpos :
            tail_ypos = tail_ypos+1
            tail_xpos = tail_xpos+1
        # Position of head and tail is: 
        #  . . .   
        #  H . .
        #  . . T
        # Move tail up one step and to the left one step 
        elif head_ypos > tail_ypos and head_xpos<tail_xpos :
            tail_ypos = tail_ypos+1
            tail_xpos = tail_xpos-1
        # Position of head and tail is: 
        #  . . .   
        #  T . .
        #  . . H
        # Move tail down one step and to the right one step
        elif head_ypos<tail_ypos and head_xpos>tail_xpos :
            tail_ypos = tail_ypos-1
            tail_xpos = tail_xpos+1
        # Position of head and tail is: 
        #  . . .   
        #  . . T
        #  H . .
        # Move tail down one step and to the left one step
        elif head_ypos<tail_ypos and head_xpos<tail_xpos :
            tail_ypos = tail_ypos-1
            tail_xpos = tail_xpos-1

        new_pos = {'tail' : ['head', tail_xpos,tail_ypos]}
        rope.update(new_pos)
        new_visited = (tail_xpos,tail_ypos)
        visited_sites=rope['visited']
        visited_sites.add(new_visited)
        new_sites = {'visited':visited_sites}
        rope.update(new_sites)
        # print(" to: "+str(new_visited))
        
    xdiff = head_xpos-tail_xpos
    ydiff = head_ypos-tail_ypos       
    if abs(xdiff)>1 or abs(ydiff)>1 :
        print("uhoh big problem")
        
def move_head_gen(rope, order) : 
    # 'piece of rope' : ['previous', 'next',xpos,ypos ]
    # rope is a dictionary arbitrary length of the form
    # dictionary is ordered: head is the first and tail is the second to last
    # the last item contains the list of visited sites
    visited ='visited'
    move_right = "R"
    move_left = "L"
    move_up = "U"
    move_down = "D"
    keys = list(rope.keys())
    
    head_key = keys[0]
    head =  rope[head_key]
    number_of_pieces = len(keys)
    index_set = range(1,number_of_pieces-1)
    print(list(index_set))
    previous = head[0] 
    next_piece = head[1] 

    xpos_head = head[2]
    ypos_head = head[3]
    direction = order[0]
    number_of_steps = range(order[1])
    
    if direction == move_right :
        for i in number_of_steps :
            xpos_head = xpos_head +1
            # 'piece of rope' : ['previous', 'next',xpos,ypos ]
            new_pos = { head_key : [previous, next_piece, xpos_head,ypos_head]}
            rope.update(new_pos)
            for i in index_set : 
                # move_rope(rope, head_key,tail_key, is_tail)
                move_rope(rope, str(i-1),str(i), i== 9)
    if direction == move_left :
        for i in number_of_steps :
            xpos_head = xpos_head -1
            new_pos = { head_key : [previous, next_piece, xpos_head,ypos_head]}
            rope.update(new_pos)
            for i in index_set : 
                # move_rope(rope, head_key,tail_key, is_tail)
                move_rope(rope, str(i-1),str(i), i== 9)
    if direction == move_up :
        for i in number_of_steps :
            ypos_head = ypos_head +1
            new_pos = { head_key : [previous, next_piece, xpos_head,ypos_head]}
            rope.update(new_pos)
            for i in index_set : 
                # move_rope(rope, head_key,tail_key, is_tail)
                move_rope(rope, str(i-1),str(i), i== 9)
    if direction == move_down :
        for i in number_of_steps :
            ypos_head= ypos_head -1
            new_pos = { head_key : [previous, next_piece, xpos_head,ypos_head]}
            rope.update(new_pos)
            for i in index_set : 
                # move_rope(rope, head_key,tail_key, is_tail)
                move_rope(rope, str(i-1),str(i), i== 9)
def move_rope(rope, head_key,tail_key, is_tail) :        
    # check position of head and then update position accordingly
    # rope is a dictionary with
    #      'piece_of_rope' : ['previous', 'next',xpos,ypos ], for each piece and last entry is
    #             'visited' : {(xpos,ypos)}
    # If rope contains 
    
    head = rope[head_key]
    tail = rope[tail_key]
    head_xpos = head[2]
    head_ypos = head[3]
    tail_xpos = tail[2]
    tail_ypos = tail[3]

    #check where head is in relation to rope: 
    xdiff = head_xpos-tail_xpos
    ydiff = head_ypos-tail_ypos        
    # If head and tail are in the same row but tail is more than one step away from head
    if abs(xdiff)>1 and ydiff ==0 :
        # move tail towards head
        if head_xpos>tail_xpos :
            tail_xpos =tail_xpos+1
        else:
            tail_xpos =tail_xpos-1
                #'piece_of_rope' : ['previous', 'next',xpos,ypos ]
        new_pos = { tail_key : [head, tail[1], tail_xpos,tail_ypos]}
        rope.update(new_pos)
        if is_tail :
            new_visited = (tail_xpos,tail_ypos)
            visited_sites=rope['visited']
            visited_sites.add(new_visited)
            new_sites = {'visited':visited_sites}
            rope.update(new_sites)
        # print(" to: "+str(new_visited))

    # If head and tail are in the same column but tail is more than one step away from head
    elif abs(ydiff)>1 and xdiff ==0 :
        # move tail towards head
        if head_ypos>tail_ypos :
            tail_ypos =tail_ypos+1
        else:
            tail_ypos =tail_ypos-1
        new_pos = { tail_key : [head, tail[1], tail_xpos,tail_ypos]}
        rope.update(new_pos)
        if is_tail :
            new_visited = (tail_xpos,tail_ypos)
            visited_sites=rope['visited']
            visited_sites.add(new_visited)
            new_sites = {'visited':visited_sites}
            rope.update(new_sites)
        # print(" to: "+str(new_visited))

        
    # if the head and tail aren't touching and aren't in the same row or column, 
    # the tail always moves one step diagonally to keep up:

    elif abs(ydiff)>1 and abs(xdiff)>0 :
        # If position of head and tail is: 
        #  . . H   
        #  . . . 
        #  . T .
        # Move tail up one step and to the right one step 
        if head_ypos > tail_ypos and head_xpos>tail_xpos :
            tail_ypos = tail_ypos+1
            tail_xpos = tail_xpos+1
        # else if position of head and tail is: 
        #  H . .   
        #  . . .
        #  . T .
        # Move tail up one step and to the left one step 
        elif head_ypos > tail_ypos and head_xpos<tail_xpos :
            tail_ypos = tail_ypos+1
            tail_xpos = tail_xpos-1
        # else if position of head and tail is: 
        #  T . .   
        #  . . .
        #  . H .
        # Move tail down one step and to the right one step
        elif head_ypos<tail_ypos and head_xpos>tail_xpos :
            tail_ypos = tail_ypos-1
            tail_xpos = tail_xpos+1
        # else if position of head and tail is: 
        #  . . T   
        #  . . .
        #  . H .
        # Move tail down one step and to the left one step
        elif head_ypos<tail_ypos and head_xpos<tail_xpos :
            tail_ypos = tail_ypos-1
            tail_xpos = tail_xpos-1

        new_pos = { tail_key : [head, tail[1], tail_xpos,tail_ypos]}
        rope.update(new_pos)
        if is_tail :
            new_visited = (tail_xpos,tail_ypos)
            visited_sites=rope['visited']
            visited_sites.add(new_visited)
            new_sites = {'visited':visited_sites}
            rope.update(new_sites)
        
    elif abs(xdiff)>1 and abs(ydiff)>0 :
        # Position of head and tail is: 
        #  . . .        
        #  . . H  
        #  T . .
        # Move tail up one step and to the right one step 
        if head_ypos > tail_ypos and head_xpos>tail_xpos :
            tail_ypos = tail_ypos+1
            tail_xpos = tail_xpos+1
        # Position of head and tail is: 
        #  . . .   
        #  H . .
        #  . . T
        # Move tail up one step and to the left one step 
        elif head_ypos > tail_ypos and head_xpos<tail_xpos :
            tail_ypos = tail_ypos+1
            tail_xpos = tail_xpos-1
        # Position of head and tail is: 
        #  . . .   
        #  T . .
        #  . . H
        # Move tail down one step and to the right one step
        elif head_ypos<tail_ypos and head_xpos>tail_xpos :
            tail_ypos = tail_ypos-1
            tail_xpos = tail_xpos+1
        # Position of head and tail is: 
        #  . . .   
        #  . . T
        #  H . .
        # Move tail down one step and to the left one step
        elif head_ypos<tail_ypos and head_xpos<tail_xpos :
            tail_ypos = tail_ypos-1
            tail_xpos = tail_xpos-1

        new_pos = { tail_key : [head, tail[1], tail_xpos,tail_ypos]}
        rope.update(new_pos)
        if is_tail :
            new_visited = (tail_xpos,tail_ypos)
            visited_sites=rope['visited']
            visited_sites.add(new_visited)
            new_sites = {'visited':visited_sites}
            rope.update(new_sites)
        # print(" to: "+str(new_visited))
        
    xdiff = head_xpos-tail_xpos
    ydiff = head_ypos-tail_ypos       
    if abs(xdiff)>1 or abs(ydiff)>1 :
        print("uhoh big problem")


with open("9dec.txt") as f:
    lines = f.readlines()
    clean_input(lines)
    motions = get_motions(lines)
    # Part 1
    # xpos =0
    # ypos=0
    # rope = { 'head' : ['tail',xpos,ypos],
    #          'tail' : ['head', xpos,ypos],
    #          'visited' : {(xpos,ypos)}
    #         }
    # head = rope['head']
    # # print("Start at position (" + str(head[1]) +','+str(head[2])+")" )
    # for motion in motions :
    #     # print("Move " + str(motion[1])+ " steps to " + motion[0])
    #     move_head(rope, motion)
    #     head = rope['head']
        # print("Head is now at position ("+ str(head[1]) +','+str(head[2])+")"  )
    # visited_sites = rope['visited']
    # print(visited_sites)
    # print(len(visited_sites))
    length_of_rope = 10
    xpos =0
    ypos=0    
    # Create a rope of length length of rope of format :
    #    'piece of rope' : ['previous', 'next',xpos,ypos ]
    rope_gen = dict()
    for i in range(length_of_rope): 
        this_piece = str(i)
        previous_piece = str(i-1)
        next_piece = str(i+1)
        piece = {this_piece : [previous_piece, next_piece, xpos,ypos]}
        rope_gen.update(piece)
    # print(rope_gen)    
    visited_sites = { 'visited' : {(xpos,ypos)} }
    rope_gen.update(visited_sites)
    keys = list(rope_gen.keys())
    print(keys)
    print(len(keys))
    n=len(keys)-1
    print(list(range(n)))
    for motion in motions: 
        move_head_gen(rope_gen, motion)
    visited_sites = rope_gen['visited']
    print(visited_sites)
    print(len(visited_sites))



    
    
