# -*- coding: utf-8 -*-
"""
Created on Fri Dec  2 10:30:30 2022

@author: ollee
"""


with open("1dec.txt") as f:
    tot_cal=[]
    lines = f.readlines()
    N=len(lines)
    print( lines[N-1] )
    i=0
    while i<N:
        l=lines[i].strip()
        prosp_max = int(l)
        j=i+1
        if j<N: 
            next_l = lines[j].strip()
      
            while j<N-1 and len(next_l)>0:
                prosp_max=prosp_max+int(next_l)
                j=j+1
                next_l = lines[j].strip()
        i=j+1


        print(prosp_max)    
        tot_cal.append(prosp_max)
    print(len(tot_cal))   
    print(tot_cal[0])
    print(sorted(tot_cal))
        


            
    


