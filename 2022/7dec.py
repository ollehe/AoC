# -*- coding: utf-8 -*-
"""
Created on Wed Dec  7 07:50:18 2022

@author: ollee
"""

class Tree: # Tree structure to describe filesystem
    def __init__(self, children, data, parent, name, size):
        self.children = children 
        self.data = data 
        self.parent = parent #Points towards the parent node
        self.name = name # String with the name of the node.
        self.size = 0
    def add_child(self, obj):
        self.children.append(obj)
    def add_parent(self, obj): 
        self.parent.append(obj)
    def add_data(self, obj): 
        self.data.append(obj)

def clean_input(lines) : 
    # Removes line breaks from loaded file
    new_line = []
    for line in lines:
        new_line.append(line.strip()) 
    return new_line
def check_if_cd(line) :
    # Check if command line is $ cd xyz
    cd_str = "cd"
    split_line = line.split()
    for l in split_line :
        if l == cd_str:
            return True
def check_if_ls(line) :
    ls_str ="ls"
    split_line = line.split()
    for l in split_line:
        if ls_str == l:
            return True
    
def check_if_file(line) :
    split_line =line.split()
    return split_line[0].isnumeric()
    # Check if command line is $ ls    
    ls_str = "ls"
    split_line = line.split()
    for l in split_line :
        if l == ls_str:
            return True

def check_if_nodes_equal(node1,node2):
    return (node1.children == node2.children) and (node1.data == node2.data) and (node1.parent == node2.parent) and (node1.name == node2.name ) and (node1.size == node2.size) 
def bfs_tree(visited, root): 
    # BFS search which returns the number of nodes with large weight
    queue = []
    visited.append(root)
    queue.append(root)
    while queue:          # Creating loop to visit each node
        m = queue.pop(0) 
        # print (m, end = " ") 
        # print("At node: " +m.name)
        # print("Contains  files:")
        # print(m.data)
        children = m.children 
        for child in children:
            if child not in visited:
                # print("Node : " +child.name + " not visited")
                visited.append(child)
                queue.append(child)
    return visited
        

        
def generate_tree(file_sys) : 
    tree =[]
    
    n = len(file_sys)
    i=0
    current_node = Tree([],[],None," ", 0) # dummy node
    
    while i<n-1 :
        # Go through list
        line = file_sys[i]
        # Check if line is equal to $ cd xyz
        if check_if_cd(line) :
            name = line.split()[2]
            # If name not == ..
            if name.isalpha() or name =="/": 
                # Create a new node and add the data
                # print("Create a new node with name " + name)
                # Tree(children, data, parent, name, size)
                new_node = Tree([],[],current_node,name,0)                
                j=i+1
                new_line = file_sys[j]
                while not(check_if_cd(new_line)) and j<n-1 :
                    # Add files 
                    if check_if_file(new_line) : 
                        l=new_line.split()
                        new_node.data.append(int(l[0]))  
                    j=j+1
                    new_line = file_sys[j]
                if j==n-1 : # fulhaxxxx för sista raden
                    if check_if_file(new_line) : 
                        l=new_line.split()
                        new_node.data.append(int(l[0]))                    
                
                # Compute size of all files within node.
                for file in new_node.data:
                    new_node.size = new_node.size +file
                i=j
                # print("Directory: " + current_node.name+ " has files" )
                # print(current_node.data)
                tree.append(new_node)
                current_node = new_node
            else :
                # print(name)
                current_node = current_node.parent
                # print(current_node.name)
                i=i+1
    # print(tree[0].name)
    for node in reversed(tree) :
        parent_node = node.parent
        parent_node.children.append(node)
    tree[0].parent = None        
    return tree[0]




with open("7dec.txt") as f:
    lines = f.readlines()
    lines = clean_input(lines)
    root = generate_tree(lines)
    nodes=bfs_tree([], root)
    # for node in nodes :
    #     # print(node.name)
    #     parent = node.parent
    #     if parent is not None :
    #         print(node.name + " has parent " +parent.name)
    #     if parent is None :
            # print(node.name + " has no parent!")

    total_dir_size =0
    max_size = 100000
    # Go from leaves and up
    # for node in nodes:
    #     if node.data :
    #         print(node.name+ " has no files!")
    for node in reversed(nodes) :
        parent = node.parent
        if parent is not None : # Only root has none as parent
            parent.size = node.size+parent.size 
    for node in nodes :
        if node.size < max_size :
            # print("Node " + node.name + " is small" + " and has size " + str(node.size))
            total_dir_size = node.size +total_dir_size
    
    s = "vsztsjfh"
    for node in nodes :
        if node.name == s :
            print("node " + s + " has files: ")

            print(node.data)

    # print(total_dir_size)
    cap = 70000000 # Total memory
    req = 30000000 # Required memory
    available_space =cap-root.size
    print("root size: ")
    print(root.size)
    print("available space : ")
    print(available_space)
    d = req-(cap-root.size)
    # print(d)
    # 143434
    #Find the smallest directory that, if deleted, would free up enough space on the filesystem to run the update. What is the total size of that directory?
    smallest =[]
    for node in nodes :
        if node.size > d :
            # print(node.name + " has smaller directory ")
            smallest.append(node.size)
    #         print(node.name)
    #         print(node.size)
    print(min(smallest))
    # sorted_smallest = sorted(smallest)
    # print(sorted_smallest)


        
    
    # problem_node_1= "jssnn"
    # problem_node_2 = "qpbqqj"
    # for node in nodes:
    #     if node.name == problem_node_1:
    #         print(node.name + " has children: ")
    #         for child in node.children :
    #             print(child.name)
    #     if node.name == problem_node_2 : 
    #         print(node.name + " has children: ")
    #         for child in node.children :
    #             print(child.name)



                
        
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    