# -*- coding: utf-8 -*-
"""
Created on Fri Dec  9 07:38:21 2022

@author: ollee
"""

import numpy


def clean_input(lines) : 
    # Removes line breaks from loaded file
    new_line = []
    for line in lines:
        new_line.append(line.strip()) 
    return new_line



def construct_grid(lines) :
    #create an n by m matrix (n rows, m columns)    
    rows = len(lines)
    cols = len(lines[0])
    grid = numpy.zeros((rows,cols))

    i=0
    for line in lines :  
        j=0
        for char in line :
            grid[i,j] = int(char)
            j=j+1
        # print(row)
        i=i+1
    return grid


def row_visible(grid, row, col) :
    # rows = grid.shape[0]
    # cols = grid.shape[1]
    left_max = max(grid[row,:col])
    right_max = max(grid[row,col+1:])
    return (grid[row,col]>left_max) or (grid[row,col]>right_max)    
    
def col_visible(grid, row, col) :
    # rows = grid.shape[0]
    # cols = grid.shape[1]
    top_max = max(grid[0:row,col])
    bottom_max = max(grid[row+1:,col])
    return (grid[row,col]>top_max) or (grid[row,col]>bottom_max)

def is_visible(grid,row, col) :
    return  row_visible(grid,row,col) or col_visible(grid, row, col)
    
def row_score(grid, row , col) :
    cols = grid.shape[1]
    left_score = 1
    right_score = 1
    boolean_row =grid[row,:]<grid[row,col]

    # loop to the left
    left_iter = col-1
    while left_iter>0 and boolean_row[left_iter] :
        left_iter=left_iter-1
        left_score = left_score+1
    # loop to the right
    right_iter = col+1

    while right_iter <cols-1 and boolean_row[right_iter]:
        right_iter=right_iter+1
        right_score = right_score+1
    # cha cha reaaal smoooth
    # print("Left score of " +  "("+str(row) +","+str(col)+") is "+ str( left_score))
    # print("Right score of " +  "("+str(row) +","+str(col)+") is "+ str( right_score))
    return (left_score)*(right_score)
    
def col_score(grid, row , col) :
    top_score = 1
    bot_score = 1
    boolean_col = grid[:,col]<grid[row,col]

    # loop to the top
    top_iter = row-1
    while top_iter >0 and boolean_col[top_iter] :
        top_iter = top_iter-1
        top_score =top_score+1
    
    bot_iter = row+1
    # loop to the bottom
    while bot_iter <cols-1 and boolean_col[bot_iter] :
        bot_iter = bot_iter+1
        bot_score = bot_score+1
    # print("Top score of " +  "("+str(row) +","+str(col)+") is "+ str( top_score))
    # print("Bottom score of " +  "("+str(row) +","+str(col)+") is "+ str( bot_score))
    # cha cha reaaal smoooth
    return top_score*bot_score
    
def scenic_score(grid,row,col) : 
    return row_score(grid, row, col)*col_score(grid, row, col)
       
with open("8dec.txt") as f:
    lines = f.readlines()
    lines = clean_input(lines)
    grid = construct_grid(lines)
    rows = grid.shape[0]
    cols = grid.shape[1]
    nbr_visible_trees = 0
    visible_indices = []
    for row in range(1,rows-1):        
        for col in range(1,cols -1):
            if is_visible(grid, row, col) :
                nbr_visible_trees = nbr_visible_trees+1
                visible_indices.append([row,col])
    nbr_visible_trees = nbr_visible_trees +2*(rows-2)+2*cols     
    maximal_score=0
    for vis in visible_indices:
        ind = vis
        row = ind[0]
        col = ind[1]
        print("Tree at "+ str(vis) + " is visible and has scenic score :"  ) 
        print(scenic_score(grid, row, col)) 
        maximal_score = max( maximal_score , scenic_score(grid, row, col))
    print("Maximal scenic score  is :" + str(maximal_score))
        
        