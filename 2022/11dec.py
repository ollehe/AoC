# -*- coding: utf-8 -*-
"""
Created on Mon Dec 12 08:36:31 2022

@author: ollee
"""

# op = {'+': lambda x, y: x + y,
#       '-': lambda x, y: x - y
#       }

import primefac

def clean_input(lines) : 
    # Removes line breaks from loaded file
    new_line = []
    for line in lines:
        new_line.append(line.strip()) 
    return new_line
    

class Monkey:
    def __init__(self, name, items, operation, test, decision, number_of_inspections ):
        # Simulates a monkey with name = name, worry level = starting items
        # each item is a dictionary corresponding to their modulo operations
        # and specified operations, test and decision
        self.name = name
        self.items = items
        self.operation = operation
        self.test = test
        self.decision = decision
        self.number_of_inspections = 0
    def inspect(self) : 
        self.number_of_inspections =self.number_of_inspections+1    
        first = self.items[0]
        for key in first : 
            old = first[key]
            first.update({key :eval(self.operation)%key } )
        self.items[0]=first
        # old = prime_prod(self.items[0])
        # new_value = eval(self.operation)
        # new_value = new_value//3 
        # self.items[0] = list( primefac.primefac(new_value) )

    # def inspect_no_worry(self):
    #     self.number_of_inspections =self.number_of_inspections+1    
    #     if '+' in self.operation :
    #         old = prime_prod(self.items[0])
    #         new_value = eval(self.operation)
    #         self.items[0] = list( primefac.primefac(new_value) )
    #     elif '*' in self.operation : 
    #         split_string = self.operation.split()
    #         if split_string[2] == 'old' : 
    #             self.items[0].append(self.items[0])
    #         else :
    #             print(split_string)
    #             new_factor = int(split_string[2])
                
    #             self.items[0].append( new_factor )
        
        

    # def throw(self) : 
    #     item = self.item.pop(0) #remove first item
    #     return item
    
    # def grab(self, new_item):
    #     self.item.append(new_item) 
    
def create_monkeys(lines) :
    # returns a list of monkeys 
    # (name, starting_items, operation, test, decision)
    name ="Monkey"
    starting_items = "Starting items:"
    operation = "Operation:"
    test = "Test:"
    test_is_true = "If true"
    test_is_false = "If false"
    list_of_monkeys = []
    base_test_dictionary = { }
    for line in lines : 
        if test in line :
            split_line=line.split()
            base_test_dictionary.update({ int(split_line[-1]) : 0})
    for key in base_test_dictionary :
        print(key)

    
    
    i=0
    while i <len(lines)-1:
        # if name is in i'th line create new monkey    
        if name in lines[i]: 
            split_line =lines[i].split()
            monkey_name = split_line[1]
            new_monkey = Monkey(monkey_name[0], [], "", "", [],0)
            j=i+1 
            while j<=len(lines)-1 and len(lines[j])>0 :
                if starting_items in lines[j]:
                    split_line = lines[j].split(":")
                    items = split_line[1].split(",") 
                    dic_list =[] 
                    # for part 1 we convert these into strings
                    print(items)
                    for item in items :           
                        item_dictionary = {}
                        # print("Value " +item )
                        for key in base_test_dictionary :
                            # print("Modulo with " +str(key) + " yields " + str(int(item)%key ) )
                            item_dictionary.update( { key : int(item)%key } )
                        new_monkey.items.append(item_dictionary)
                        
                if operation in lines[j]:
                    split_line = lines[j].split("=")
                    new_monkey.operation = split_line[1] 
                if test in lines[j]:
                    split_line=lines[j].split() 
                    new_monkey.test = int(split_line[-1])
                if test_is_true in lines[j] :
                    split_line=lines[j].split() 
                    new_monkey.decision.append(  split_line[-1])
                if test_is_false in lines[j] :
                    split_line=lines[j].split() 
                    new_monkey.decision.append(  split_line[-1])
                j=j+1
            i=j+1
            list_of_monkeys.append(new_monkey)   
    return list_of_monkeys

def prime_prod(factors):
    p =1 
    for factor in factors :
        p=factor*p
    return p


with open("11dec.txt") as f:
    lines = f.readlines()
    lines = clean_input(lines)
    # Create a list of monkeys
    
    # print( len(lines) )
    # for line in lines :
    #     print(line)
    monkeys = create_monkeys(lines)
    
    # for monkey in monkeys: 
    #     print("Monkey : " + monkey.name)
    #     for item in monkey.items : 
    #         print(item)
    
    number_of_rounds = range(10000)
    for j in number_of_rounds :     
        for monkey in monkeys : 
            # print(monkey.items)
            items = monkey.items
            targets = monkey.decision
            # print(targets)
            while len(items)>0 : 
                monkey.inspect()
                item = monkey.items.pop(0)
                if item[monkey.test]==0 : # if worry level is divisible 
                    for target_monkey in monkeys :
                        if target_monkey.name == targets[0] :
                            target_monkey.items.append(item)
                else :
                    for target_monkey in monkeys :
                        if target_monkey.name == targets[1] :
                            target_monkey.items.append(item)
                items = monkey.items
    for monkey in monkeys : 
        print("Monkey number " +monkey.name + " has inspected " +str(monkey.number_of_inspections)+ " number of items ")
        # print(monkey.number_of_inspections)
    
    
            
        
