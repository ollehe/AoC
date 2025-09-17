# -*- coding: utf-8 -*-
"""
Created on Tue Dec  6 20:42:21 2022

@author: ollee
"""

def clean_input(lines): #removes line breaks from loaded file
    new_line = []
    for line in lines:
        new_line.append(line.strip()) 
    return new_line

def find_marker(line):
    # Given a string
    marker_length = 14
    stream_length = len(line)
    for i in range(stream_length-marker_length):
        if len(set(line[i:i+marker_length]))==marker_length:
            return i+marker_length    
with open("6dec.txt") as f:
    lines = f.readlines()
    lines = clean_input(lines)
    line = lines[0]
    a=find_marker(line)
    print(a)