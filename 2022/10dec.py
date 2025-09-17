# -*- coding: utf-8 -*-
"""
Created on Sat Dec 10 18:26:18 2022

@author: ollee
"""

          
def clean_input(lines) : 
    # Removes line breaks from loaded file
    new_line = []
    for line in lines:
        new_line.append(line.strip()) 
    return new_line


def compute_value( commands, value) : 
    addition = "addx"
    no_operation = "noop"
    val = value 
    cycle =0
    cycle_value = { cycle : value }

    for command in commands : 
        split_command = command.split()
        if split_command[0] == no_operation :
            cycle = cycle+1

            cycle_value.update({ cycle: val})

        elif split_command[0] == addition :
            cycle = cycle+1
            cycle_value.update({ cycle: val})             
            cycle = cycle+1
            val = val+int(split_command[1])
            cycle_value.update({ cycle: val}) 
            # j=cycle+1
            # while j< cycle+3 :
            #     cycle_value.update({ j: val})             
            #     j=j+1
            # cycle = j+1


    # # Fill out the missing cycle values:         
    # keys = list(cycle_value.keys())
    # number_of_cycles = keys[-1]
    # number_of_cycles = range(number_of_cycles+1)
    # print(len(keys))
    # print(list(number_of_cycles))
    # for cycle in number_of_cycles : 
    #     if cycle not in keys :
    #         cycle_value.update({ cycle : cycle_value[cycle-1] })
    return cycle_value
        
        
def compute_signal( cycles, specified_cycles ) : 
    # find the largest cycle in cycles such that
    # cycle <= spec_cycle for each spec_cycle in specified_cycles
    index_set = range(len(specified_cycles))
    max_cycles = [0]*len(specified_cycles)
    strength =0
    for i in index_set :
        for cycle in cycles : 
            if cycle < specified_cycles[i] :
               max_cycles[i] = cycle
    # print(max_cycles)
    for i in index_set : 
        print("At specified cycle: " + str(specified_cycles[i]) + " the signal has strength : " + str(cycles[max_cycles[i]]))
        strength = specified_cycles[i]*cycles[max_cycles[i]]+strength
    print(strength)
        
        
def draw_message(cycles) : 
    # So, by carefully timing the CPU instructions and the CRT drawing operations, 
    # you should be able to determine whether the sprite is visible the instant each pixel is drawn. 
    # If the sprite is positioned such that one of its three pixels 
    # is the pixel currently being drawn, the screen produces a lit pixel (#); 
    # otherwise, the screen leaves the pixel dark (.).
    sprite_pos = 0
    s=""
    for cycle in cycles : 
        # print("At cycle :" + str(cycle) + " the value of X is :" + str(cycles[cycle]) )
        if abs(cycle%40-cycles[cycle])<2:
            s=s+"#" 
        else : 
            s=s+"."
        if len(s)>=40 :
            print(s)
            s=""
            


    
    
    
with open("10dec.txt") as f:
    lines = f.readlines()
    lines = clean_input(lines)
    x=1
    cycles= compute_value(lines,x)
    # Test data: 
    # At specified cycle: 20 the signal has strength : 21
    # At specified cycle: 60 the signal has strength : 19
    # At specified cycle: 100 the signal has strength : 18
    # At specified cycle: 140 the signal has strength : 21
    # At specified cycle: 180 the signal has strength : 16
    # At specified cycle: 220 the signal has strength : 18
    
        
    # for cycle in cycles :
    #     print("Cycle : "+str(cycle)+" " + "has value : "+ str(cycles[cycle]))

    # Part 1
    # specified_cycles = [20, 60, 100, 140, 180, 220]
    # compute_signal(cycles,specified_cycles)
    # Correct answer is 15140 with : 
    # At specified cycle: 20 the signal has strength : 12
    # At specified cycle: 60 the signal has strength : 4
    # At specified cycle: 100 the signal has strength : 39
    # At specified cycle: 140 the signal has strength : 22
    # At specified cycle: 180 the signal has strength : 17
    # At specified cycle: 220 the signal has strength : 21
    # for cycle in specified_cycles :
    #     print("Cycle : "+str(cycle)+" " + "has value : "+ str(cycles[cycle]))
    
    draw_message(cycles)    
            
        

    