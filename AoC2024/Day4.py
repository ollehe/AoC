import itertools

def in_bounds(rows,cols,row,col) : 
    return row < rows and col < cols and row >= 0 and col >= 0 

def find_char(grid, char) : 
    chars = []
    rows = len(grid)
    cols = len(grid[0])
    for row in range(len(grid)) : 
        for col in range(len(grid[0])) : 
            if in_bounds(rows,cols,row,col) and grid[row][col] == char : 
                chars.append([row,col])
    return chars

def find_xmas(grid) :
    rows = len(grid)
    cols = len(grid[0])
    list_of_X = find_char(grid, 'X')
    number_of_xmas = 0
    shifts = [
        [-1,-1], 
        [1,1], 
        [-1,1],
        [1,-1],
        [-1,0],
        [0,-1],
        [1,0],
        [0,1]
        ]
    for pos in list_of_X :
        row = pos[0]
        col = pos[1]
        for shift in shifts : 
            row_shift = shift[0]
            col_shift = shift[1]
            MAS = ""
            for step in range(1,4) :
                next_row = row + row_shift*step
                next_col = col + col_shift*step
                if not in_bounds(rows,cols,next_row,next_col) :
                    break
                MAS += ""+grid[next_row][next_col]
                if MAS == "MAS" : 
                    number_of_xmas +=1
                    break
    return number_of_xmas


def is_valid(rows,cols,row,col) : 
    return row >= 1 and col >= 1 and row < rows -1 and col < cols -1     

def find_xMAS(grid) : 
    rows = len(grid)
    cols = len(grid[0])
    list_of_A = find_char(grid, 'A')
    shifts = [
        [-1,-1], 
        [-1,1],
        [1,1], 
        [1,-1],
        ]
    number_of_xMAS = 0
    for pos in list_of_A : 
        row = pos[0]
        col = pos[1]
        if is_valid(rows,cols,row,col) : 
            # Check for the following configurations: 
            # *.*  
            # .A.   
            # *.* 
            number_of_Ms = 0
            number_of_Ss = 0
            for shift in shifts : 
                shifted_row = row + shift[0]
                shifted_col = col + shift[1]
                if grid[shifted_row][shifted_col] == 'M' : 
                    number_of_Ms +=1
                elif grid[shifted_row][shifted_col] == 'S' : 
                    number_of_Ss +=1
                if number_of_Ms == 2 and number_of_Ss == 2 : 
                    for i in range(len(shifts)) : 
                        first_row = row + shifts[i][0]
                        first_col = col + shifts[i][1]
                        second_row = row + shifts[(i+1)%len(shifts)][0]
                        second_col = col + shifts[(i+1)%len(shifts)][1]
                        if grid[first_row][first_col] == grid[second_row][second_col] :
                            number_of_xMAS += 1
                            break
    return number_of_xMAS







grid = []
with open("input.txt") as f:
    lines = f.readlines()
    grid = [line.strip() for line in lines]

print(find_xmas(grid))
print(find_xMAS(grid))