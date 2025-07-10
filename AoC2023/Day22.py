class Brick:
	def __init__(self, intervals : list[range]):
		self.intervals = intervals
		self.above : list[Brick] = []
		self.below : list[Brick] = []

	def fall(self) -> None: 
		pass

def parse_bricks(data : list[str]) -> list[Brick] :
	"""
		Parses input (as list of strings) and outputs a list of Bricks
	"""
	bricks : list[Brick] = []*len(data)
	for line in data : 
		start,stop = line.split('~')
		x_start,y_start,z_start = start.split(',')
		x_stop,y_stop,z_stop = start.split(',')
		x = range(int(x_start),int(x_stop)+1)
		y = range(int(y_start),int(y_stop)+1)
		z = range(int(z_start),int(z_stop)+1)
		bricks.append(Brick([x,y,z]))

	return bricks



if __name__=="__main__":
	bricks : list[Brick] = []

	with open("input.txt") as f :
		lines = f.readlines()
		bricks = parse_bricks(lines)
	
