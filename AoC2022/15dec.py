# -*- coding: utf-8 -*-
"""
Created on Thu Dec 15 09:34:41 2022

@author: ollee
"""


def clean_input(lines) : 
    # Removes line breaks from loaded file
    new_line = []
    for line in lines:
        new_line.append(line.strip()) 
    return new_line

def get_coordinates(lines) :
    sensors = []
    beacons = []
    for line in lines : 
        split_line = line.split(':')
        #Split line into sensor data and beacon data
        sensor_data = split_line[0]
        
        beacon_data = split_line[1]

        # Get x and y coordinate (x=col_data) and (y=row_data)
        sensor_data_split = sensor_data.split(',')
        sensor_row = sensor_data_split[1].split('=')[1]
        sensor_col = sensor_data_split[0].split('=')[1]
        sensors.append([int(sensor_row),int(sensor_col) ] )
        
        # Get x and y coordinate (x=col_data) and (y=row_data)
        beacon_data_split = beacon_data.split(',')
        beacon_row = beacon_data_split[1].split('=')[1]
        beacon_col = beacon_data_split[0].split('=')[1]
        beacons.append([int(beacon_row), int(beacon_col) ] )
    return [sensors, beacons]
class Ball :
    def __init__(self, row, col, radius) :
        self.row = row
        self.col = col
        self.radius = radius
    def overlap(self, ball) :
        x = [self.row, self.col]
        y = [ball.row, ball.col]
        return L1_distance(x,y) <= self.radius+ball.radius
    def intersect_row(self,row) :
        return abs(self.row-row)<=self.radius
class Interval : 
    def __init__(self, left,right) :
        self.left = left
        self.right = right
    def intersect(self,interval) :
        return self.get_overlap(interval) >0
    def get_overlap(self, interval) :        
        return max(0, min(self.right, interval.right) - max(self.left, interval.right))
    def length(self) :
        return self.right-self.left+1
class Union_of_Intervals :
    def __init__(self, intervals) :
        self.intervals=intervals
    def reduce(self) : 
        intervals = self.intervals 

                
        

def construct_interval_from_intersection(ball,row):    
    return Interval(ball.col-(ball.radius -abs(ball.row -row ) ),ball.col+(ball.radius -abs(ball.row -row ) ) )    

def union_of_intervals(i1,i2):
    return Interval(min(i1.left,i2.left),max(i1.right,i2.right)  )
        
def L1_distance( a,b) :
    return abs(a[0]-b[0])+abs(a[1]-b[1])


def merge_intervals(intervals):
    """
    A simple algorithm can be used:
    1. Sort the intervals in increasing order
    2. Push the first interval on the stack
    3. Iterate through intervals and for each one compare current interval
       with the top of the stack and:
       A. If current interval does not overlap, push on to stack
       B. If current interval does overlap, merge both intervals in to one
          and push on to stack
    4. At the end return stack
    """
    sorted_by_lower_bound = sorted(intervals, key=lambda tup: tup[0])
    merged = []
    
    for higher in sorted_by_lower_bound:
        if not merged:
            merged.append(higher)
        else:
            lower = merged[-1]
            # test for intersection between lower and higher:
            # we know via sorting that lower[0] <= higher[0]
            if higher[0] <= lower[1]:
                upper_bound = max(lower[1], higher[1])
                merged[-1] = (lower[0], upper_bound)  # replace by merged interval
            else:
                merged.append(higher)
    return merged
     
 

with open("15dec.txt") as f:
    lines = f.readlines()
    lines = clean_input(lines)
    coords= get_coordinates(lines)
    sensors = coords[0]
    beacons = coords[-1]
    distinct_beacons = []
    for beacon in beacons :
        if beacon not in distinct_beacons :
            distinct_beacons.append(beacon)
    number_of_sensors = range(len(sensors))
    # Construct balls
    list_of_balls = []
    for i in number_of_sensors :
        radius = L1_distance(sensors[i] , beacons[i])
        list_of_balls.append(Ball(sensors[i][0], sensors[i][1], radius))
    # row = 2000000
    # # Part 1    :     
    # row_intersecting_balls = []
    # # Find balls that intersect the specified row
    # for ball in list_of_balls :
    #     if ball.intersect_row(row) :
    #         # print("Ball at row= " + str(ball.row)+ " and column= "+ str(ball.col) +" intersect at row= " +str(row)  )
    #         row_intersecting_balls.append(ball)
    # number_of_intersections = len(row_intersecting_balls) 
    # row_intervals = []
    # # Construct the intervals of the intersecting balls 
    # for ball in row_intersecting_balls :
    #     i = construct_interval_from_intersection(ball, row)
    #     row_intervals.append(i)
    # # Take the union of all intervals 
    # union = row_intervals[0]
    # for interval in row_intervals :
    #     union = union_of_intervals(union, interval) # There is a bug in my program here, if there is no overlap between the intervals 
    # # Count the number of distinct beacons that sit on the same row : 
    # beacons_on_row = 0
    # for beacon in distinct_beacons : 
    #     if beacon[0] == row :
    #         beacons_on_row = beacons_on_row +1
          
    # Part 2: 
    max_row =  4000000
    max_col =  4000000
    universe = list(range(max_row+1))
    unavailable_spots = []

    for row in range(max_row+1):
        row_intersecting_balls = []
        for ball in list_of_balls :
            if ball.intersect_row(row) :
                # print("Ball at row= " + str(ball.row)+ " and column= "+ str(ball.col) +" intersect at row= " +str(row)  )
                row_intersecting_balls.append(ball)
        row_intervals = []
        # Construct the intervals of the intersecting balls 
        for ball in row_intersecting_balls :
            i = construct_interval_from_intersection(ball, row)
            i.left = max(i.left,0)
            i.right =min(i.right,max_row)
            interval = [i.left, i.right]
            row_intervals.append(interval)
        # print(row_intervals)
        # print(sorted(row_intervals))
        # Take the union of all intervals 
        row_intervals = merge_intervals(row_intervals)
        # union = Union_of_Intervals((row_intervals))
        unavailable_spots.append( row_intervals )
    
 
    # print(len(unavailable_spots))
    row = 0
    for intervals in unavailable_spots : 
        row=row+1
        if len(intervals)>1 :
            # print(intervals)
            first = intervals[0]
            second =intervals[1]
            if second[0]-first[1]>1 :
                print("Found it! ")
                print("Row : " + str(row-1) )
                print("Col : " + str(first[1]+1) ) 
                print("Signal value is: " + str( (first[1]+1)*4000000+row-1  ) )

        
    
    

