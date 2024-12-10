first = []
second = []
with open("input.txt") as f : 
	lines = f.readlines()
	first = [int(line.split('   ')[0]) for line in lines ]
	second = [int(line.split('   ')[1].strip()) for line in lines ]

first.sort()
second.sort()
difference = [ abs(pair[0]-pair[1]) for pair in zip(first,second) ]
print(sum(difference))
similarity =0

for val in first : 
	freq = 0
	for check in second : 
		if val == check : 
			freq +=1
		elif check > val : 
			break 
	similarity += val*freq
print(similarity)