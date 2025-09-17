import pprint as pretty
from sympy.ntheory.modular import crt

with open("input.txt") as file : 
    lines = file.readlines()
    earliest_time_stamp = int(lines[0])
    bus_ids = [ [int(val),index] for val,index in zip(lines[1].split(','),range(len(lines[1]))) if val !='x' ]
    # pretty.pprint(bus_ids)
    first_arrival = float('inf')
    first_bus = 0
    for id,val in bus_ids : 
        if (earliest_time_stamp//id+1)*id  < first_arrival : 
            first_bus = id
            first_arrival = (earliest_time_stamp//id+1)*id 
        
        first_bus = min( first_bus,(earliest_time_stamp//id+1)*id )
    print("ID·(Time_stamp - First_bus)=",first_bus*(first_arrival-earliest_time_stamp))
    # In the example: 
    # [[7, 0], [13, 1], [59, 4], [31, 6], [19, 7]]
    # Find first t such that t%7 = 0, (t+1)%13=0, (t+4)%59=0, (t+6)%31=0, (t+7)%19=0
    # Classic Chinese remainder theorem application:
    remainders = [ (val-index)%val for val,index in bus_ids]
    moduli = [val for val,index in bus_ids]
    solution,_ = crt(moduli, remainders)
    print("First time is:" , solution)
