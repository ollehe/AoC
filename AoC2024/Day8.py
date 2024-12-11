import itertools


def reflect_along_x(point_of_reflection, point_to_reflect) : 
    return [2*point_of_reflection[0]-point_to_reflect[0],point_to_reflect[1]]

def reflect_along_y(point_of_reflection, point_to_reflect) : 
    return [point_to_reflect[0], 2*point_of_reflection[1]-point_to_reflect[1] ]

def reflection(point_of_reflection, point_to_reflect) : 
    return reflect_along_x(point_of_reflection,reflect_along_y(point_of_reflection,point_to_reflect))

def in_bounds(point,grid) : 
    return point[0] >= 0 and point[1] >= 0 and point[0] < len(grid) and point[1] < len(grid[0])

def find_antennas(grid) : 
    rows = len(grid)
    cols = len(grid[0])
    antennas = {}
    for row in range(rows) :
        for col in range(cols) :
            if grid[row][col] != '.' : 
                if not grid[row][col] in antennas : 
                    antennas[grid[row][col]] = [[row,col]]
                else : 
                    antennas[grid[row][col]].append([row,col])
    return antennas 

def find_antinodes(grid, part2) : 
    antennas = find_antennas(grid)
    antinodes = set()
    if part2 : 
        for type in antennas : 
            for pos in antennas[type] : 
                antinodes.add(tuple(pos))


    for antenna in antennas : 
        for pair in itertools.combinations(antennas[antenna],2) :
            first = pair[0]
            second = pair[1]
            if not part2 : 
                first_reflected = reflection(first,second)
                if in_bounds(first_reflected,grid) : 
                    antinodes.add(tuple(first_reflected))
                second_reflected = reflection(second,first)
                if in_bounds(second_reflected,grid) : 
                    antinodes.add(tuple(second_reflected))
            else : 
                # for part2 just iterate the reflections
                # first reflect the first point along the second
                point_to_reflect = first 
                point_of_reflection = second 
                reflected_point = reflection(point_of_reflection,point_to_reflect)
                while in_bounds(reflected_point,grid) : 
                    antinodes.add(tuple(reflected_point))
                    point_to_reflect = point_of_reflection
                    point_of_reflection = reflected_point 
                    reflected_point = reflection(point_of_reflection,point_to_reflect)

                # then reflect the second point along the first
                point_to_reflect = second 
                point_of_reflection = first 
                reflected_point = reflection(point_of_reflection,point_to_reflect)
                while in_bounds(reflected_point,grid) : 
                    antinodes.add(tuple(reflected_point))
                    point_to_reflect = point_of_reflection
                    point_of_reflection = reflected_point 
                    reflected_point = reflection(point_of_reflection,point_to_reflect)





    print(len(antinodes))

grid = []
with open('input.txt') as f : 
    grid = [list(line.strip()) for line in f.readlines()]

find_antinodes(grid,False)
find_antinodes(grid,True)