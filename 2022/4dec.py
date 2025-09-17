# -*- coding: utf-8 -*-
"""
Created on Sun Dec  4 09:37:04 2022

@author: ollee
"""

def get_intervals(s):
    # string s is of the format 'a-b,c-d'
    # returns a list of lists first element is a list with 
    # integers a and b, the second a list with c and d
    intervals = line.split(',')
    i1 = intervals[0].split('-')
    i2 = intervals[1].split('-')
    i2[1]=i2[1].strip()
    i1=[eval(i) for i in i1]
    i2=[eval(i) for i in i2]
    return [i1, i2]
    


def is_contained(i1,i2) :
    # Given two lists of integers corresponding to the intervals
    # [i1[0], i1[1]] and [i2[0], i2[1]]
    # Return True if either i1 is contained in i2 or vice-versa
    return ( i1[0] >= i2[0] and i1[1] <= i2[1] ) or  ( i2[0] >= i1[0] and i2[1] <= i1[1] )

def has_overlap(i1,i2):
    # Given two lists of integers corresponding to the intervals
    # [i1[0], i1[1]] and [i2[0], i2[1]]
    # Return True if they overlap 
    
    # Check first which of the intervals has the smallest left-most point
    if i1[0]<=i2[0]: 
        return i2[0]<=i1[1]
    else:
        return i1[0]<=i2[1]
    
    
    
with open("4dec.txt") as f:
    lines = f.readlines()
    contained_ints=0
    overlap_ints=0
    for line in lines :
        intervals = get_intervals(line.strip())
        
        if is_contained(intervals[0], intervals[1] ):
            contained_ints=contained_ints+1
            
        if has_overlap(intervals[0],intervals[1] ) :
            print("The intervals " + line + " has an overlap")
            overlap_ints=overlap_ints+1
            