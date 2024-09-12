# -*- coding: utf-8 -*-
"""
Created on Wed Dec 14 08:35:37 2022

@author: ollee
"""
import numpy as np
import math

def clean_input(lines) : 
    # Removes line breaks from loaded file
    new_line = []
    for line in lines:
        new_line.append(line.strip()) 
    return new_line
class Cave :
    def __init__(self, grid, source) :
        self.grid = grid # matrix consisting of '.', 'o','#' elements
        self.source =source
    def visualize(self) :
        grid = self.grid
        dim = grid.shape
        rows = dim[0]
        cols = dim[1]
        for row in range(rows) :
            s=''
            for col in range(cols) :    
                # print(grid[row,col])
                s= s+' '+str(grid[row,col])
            print(s)
    def expand(self ) :
        #Expand old grid by one column to the left and to the right.
        old_grid = self.grid
        dim = old_grid.shape
        rows = dim[0]
        cols = dim[1]
        new_grid = np.chararray((rows,cols+2),unicode=True )
        #Create empty cave with only air
        for row in range(rows) :
            for col in range(cols+2) : 
                new_grid[row,col] = '.'        
        new_grid[-1,:] = '#' # add floor to bottom
        
        # Add old grid by shifting it by 1
        for row in range(rows) :
            for col in range(cols) : 
                new_grid[row,col+1] = old_grid[row,col]
                
                
        col_source = self.source[1] 
        self.source = [0, col_source+1]
        self.grid = new_grid
        
        
    def add_floor(self) :
        grid = self.grid
        dim = grid.shape
        rows = dim[0]
        cols = dim[1]
        new_row_air = ['.']*cols
        new_row_ground = ['#']*cols
        grid = np.vstack([grid, new_row_air])
        grid = np.vstack([grid, new_row_ground])
        self.grid = grid
        

        
class Sand: # Tree structure to describe filesystem
    def __init__(self, row,col, frozen, out_of_bounds ):
        self.row = row 
        self.col = col
        self.frozen = frozen 
        self.out_of_bounds = out_of_bounds
    def move(self, cave ):
        grid = cave.grid        
        row = self.row
        col = self.col
        
        if not self.out_of_bounds :         
            if grid[row+1,col]=='.' :
                self.row = self.row+1
            elif grid[row+1,col-1] =='.' :
                self.col = self.col-1
                self.row = self.row+1
            elif grid[row+1,col+1] == '.' :
                self.col = self.col+1
                self.row = self.row+1
            else :
                self.frozen = True
        else : 
            self.frozen = True
            
    def is_out_of_bounds(self,cave) : 
        dim = cave.grid.shape
        rows = dim[0]
        cols = dim[1]
        self.out_of_bounds = self.col-1<0 or self.col+1>=cols or self.row+1>= rows
        
        

def create_cave(lines) :
    cave = Cave([],None)
    obstructions = []
    for line in lines :
        split_line = line.split('->') 
        path =  []

        for piece in split_line :
            s = '['+piece +']'
            path.append(eval(s))
        obstructions.append(path)
    col_min = math.inf
    col_max = -math.inf
    row_max = -math.inf
    for path in obstructions:
        for piece in path :
            col_min = min (col_min, piece[0] )
            col_max = max (col_max, piece[0] )
            row_max = max(row_max, piece[1])
    print(col_min)
    print(row_max)
    for path in obstructions : 
        for piece in path :
            piece[0]= piece[0]-col_min
    col_delta = col_max-col_min        
    col_source = 500-col_min
    cave.source = [0,col_source]         
    grid = np.chararray((row_max+1,col_delta+1),unicode=True )
    rows = grid.shape[0]
    cols = grid.shape[1]
    # print(grid.shape)
    for row in range(rows) :
        for col in range(cols) :
            grid[row,col] = '.'
    grid[0,col_source] = '+'
    for path in obstructions : 
        n = len(path)
        for i in range(1,n) :
            # print(path[i-1])
            # print(path[i])
            
            col_diff = path[i-1][0]-path[i][0]
            
            row_diff = path[i-1][1]-path[i][1]
            if col_diff == 0 :
                col_start = path[i-1][0]
                col_stop = path[i][0]
                row_start = path[i-1][1]
                row_stop = path[i][1]
                for row in range(min(row_start,row_stop), max(row_start,row_stop)+1)  : 
                    grid[row,col_start] = '#'                    
            else: 
                col_start = path[i-1][0]
                col_stop = path[i][0]
                row_start = path[i-1][1]
                row_stop = path[i][1]
                for col in range(min(col_start,col_stop), max(col_start,col_stop)+1) : 
                    grid[row_start,col] = '#'
    cave.grid = grid
    return cave
                
def simulate_sand(cave) :
    number_of_sands = 0    
    out_of_bounds = False
    filled = False
    source = cave.source
    grid = cave.grid 
    dim = grid.shape
    rows = dim[0]
    cols = dim[1]
    sand = Sand(source[0],source[1],False,False)
    while not filled : #out_of_bounds is the termination condition for part 1
        source = cave.source
        sand = Sand(source[0],source[1],False,False)
        number_of_sands = number_of_sands+1
        while not sand.frozen:
            # print("I'm stuck papi")
            sand.is_out_of_bounds(cave)
            sand.move(cave)
            if sand.out_of_bounds :
                cave.expand()
                sand.col =sand.col+1 # must shift the column by one since we shift the cave col by 1
                sand.frozen = False
            sand.is_out_of_bounds(cave)
        source = cave.source
        row = sand.row
        col = sand.col
        cave.grid[row,col] = 'o'
        #part 1: 
        # out_of_bounds = sand.out_of_bounds
        # this is a part 2 thing 
        # if number_of_sands<50:
        # cave.visualize()
        if sand.frozen and sand.row == 0 and sand.col == source[1] :
            filled = True
            
        
        
        
    cave.grid[row,col] = '*'
    cave.visualize()
    print(number_of_sands)
        
    


with open("14dectest.txt") as f:
    lines = f.readlines()
    lines = clean_input(lines)
    cave = create_cave(lines)
    cave.visualize()
    # simulate_sand(cave)
    cave.add_floor()
    cave.visualize()
    simulate_sand(cave)


    # # simulate_sand(cave)
    # cave.visualize()
 

    
    
