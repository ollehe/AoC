# -*- coding: utf-8 -*-
"""
Created on Tue Dec 13 10:22:08 2022

@author: ollee
"""


class DataPacket: # Tree structure to describe filesystem
    def __init__(self, left, right):
        self.left = left 
        self.right = right 


def clean_input(lines) : 
    # Removes line breaks from loaded file
    new_line = []
    for line in lines:
        new_line.append(line.strip()) 
    return new_line

def construct_data_packet(lines): 
    left = []
    right = []
    #remove all blanks
    for line in lines : 
        if not line :
            lines.remove(line)
    number_of_packets = round(len(lines))
    for i in range(0,number_of_packets,2) : 
        left.append(eval(lines[i]))
        right.append(eval(lines[i+1]))
    return [left, right]

def compare_packet(left, right) :
    comp =0 # 1 for true, 0 for undecided, -1 for false
    if type(left) == int and type(right) == int :
        if left < right :
            comp = 1
            return comp 
        if left > right :
            comp = -1
            return comp
    elif type(left) == list and type(right)==list :
        l_len = len(left)
        r_len = len(right) 
        if not left and right : #if left is empty and right nonempty 
            comp = 1
            return comp 
        elif left and not right : #if left is nonempty and right empty
            comp = -1 
            return comp 
        else :             
            i=0
            while i < min(l_len,r_len) :
                if compare_packet(left[i],right[i])==1 :
                    comp = 1
                    return comp 
                elif compare_packet(left[i],right[i]) == -1 :
                    comp = -1
                    return comp
                i=i+1
                if i>=min(l_len, r_len ):
                    if l_len < r_len :
                        comp = 1
                        return comp 
                    elif l_len > r_len :
                        comp = -1
                        return comp                         
    elif type(left) == list and type(right)==int :
        if compare_packet(left,[right])==1 :
            comp = 1
            return comp 
        elif compare_packet(left,[right]) == -1 :
            comp = -1
            return comp

    elif type(left) == int and type(right)==list :
        if compare_packet([left],right)==1 :
            comp = 1
            return comp 
        elif compare_packet([left],right) == -1 :
            comp = -1
            return comp
    return comp

def bubbleSort(arr):
     
    n = len(arr)
    # For loop to traverse through all
    # element in an array
    for i in range(n):
        for j in range(0, n - i - 1):
             
            # Range of the array is from 0 to n-i-1
            # Swap the elements if the element found
            # is greater than the adjacent element
            if compare_packet(arr[j], arr[j+1])==-1 :
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
    return arr
                 
        

with open("13dec.txt") as f:
    lines = f.readlines()
    lines = clean_input(lines)
    left = []
    right = []     
    packets = construct_data_packet(lines)
    left = packets[0]
    right = packets[1]
    number_of_packets = range(len(packets[0]) )
    # s = 0
    list_of_packets = []
    list_of_packets.append([[2]])
    list_of_packets.append([[6]])
    for i in number_of_packets :
        if compare_packet( left[i], right[i]) ==1 :
            list_of_packets.append(left[i])
            list_of_packets.append(right[i])
        else :
            list_of_packets.append(right[i])
            list_of_packets.append(left[i])
        if compare_packet( left[i], right[i]) ==0 :
            print('uh oh')
    
    sorted_packet = bubbleSort(list_of_packets)
    p=1
    number_of_packets = range(len(sorted_packet))
    for packet in sorted_packet:
        print(packet)
    
    for i in number_of_packets :
        if sorted_packet[i] ==[[2]] or sorted_packet[i]==[[6]] :
            p=p*(i+1)
            
    print(p)

