# -*- coding: utf-8 -*-
"""
Created on Fri Dec  2 12:48:37 2022

@author: ollee
"""



# Part 1:
# Opponent: A for Rock, B for Paper, and C for Scissors. 
# Me:  X for Rock, Y for Paper, and Z for Scissors
# Point per shape: X=1, Y=2, Z=3


# Rows: A=0, B=1, C=2
# Columns: X=0, Y=1, Z=2


# opponent ={'A' : 0,
#            'B' : 1,
#            'C' : 2
#            }
# me_p1 ={'X' : 0,
#      'Y' : 1,
#      'Z' : 2
#      }
# outcome = [ [3,6,0], [0,3,6], [6,0,3]]



# Part 2:
# Opponent: A for Rock, B for Paper, and C for Scissors. 
# Me:  X for Loss, Y for Draw, and Z for Win
# Point per shape: A=1, B=2, C=3
# Loss = 0, Draw = 3, Win = 6

outcome = { 
    'A X' : 3, 'A Y': 4, 'A Z' : 8,
    'B X' : 1, 'B Y': 5, 'B Z' : 9,
    'C X' : 2, 'C Y': 6, 'C Z' : 7   
    }



with open("2dec.txt") as f:
    lines = f.readlines()
    N=len(lines)
    score=0
    for r in range(N):
        l=lines[r].strip()
        # Part 1:
        # round_i = l.split(' ')
        # i = opponent[round_i[0]]
        # j= me_p1[round_i[1]]
        # score = score + outcome[i][j]+j+1
        # Part 2:
        score = score+outcome[l]
            

    print(score)
        