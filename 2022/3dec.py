# -*- coding: utf-8 -*-
"""
Created on Sat Dec  3 09:40:07 2022

@author: ollee
"""
###             Part 1                  ###
# Lowercase item types have priorities 1-26

# Create a dictionary with elements corresponding to item type and priority
items=dict()

for i in range(97,123):
    items[chr(i)]=i-96
    # print(chr(i)+ " has value "+ str(items[chr(i)]))

# Uppercase item types have priorities 27-52
for i in range(65,91):
    items[chr(i)]=i- 38
    # print(chr(i)+ " has value "+ str(items[chr(i)]))
# Given a string, it splits the string in half if it is even, 
# otherwise it returns an empty string
def split_string_in_half(s):
    if len(s)%2 == 0 :
        n= round(len(s)/2-1)
        first_half = s[:n+1]
        second_half = s[n+1:]
        return [first_half,second_half]
    else :
        return "" 

with open("3dec.txt") as f:
    lines = f.readlines()
    total_value=0
    N=len(lines)
    ###             Part 1           ###
    # for line in lines:
    #     # Split string into the two sackes
    #     first_sack = split_string_in_half(line.strip())[0]
    #     second_sack = split_string_in_half(line.strip())[1]

    #     # Get the common item 
    #     common_item = list(set(first_sack)&set(second_sack))
    #     total_value=total_value+ items[common_item[0]]
            
    # print(total_value)
    
    ###             Part 2          ###
    for i in range(0,N,3) :
        common_item =list( set( lines[i].strip() )&set( lines[i+1].strip() )&set( lines[i+2].strip() ) ) 
        total_value=total_value+ items[common_item[0]]
    print(total_value)