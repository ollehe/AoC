from __future__ import annotations

X = 0
Y = 1
Z = 2

class Brick:
    def __init__(self, intervals : list[range]):
        self.intervals = intervals
        self.above : dict[Brick,Brick] = {}
        self.below : dict[Brick,Brick] = {}
    
   
    @staticmethod
    def parse_bricks(data : list[str]) -> list[Brick] :
        """
	    Parses input (as list of strings) and outputs a list of Bricks
        """
        bricks: list[Brick] = []*len(data)
        for line in data : 
            start,stop = line.split('~')
            x_start,y_start,z_start = start.split(',')
            x_stop,y_stop,z_stop = stop.split(',')
            x: range = range(int(x_start),int(x_stop)+1)
            y: range = range(int(y_start),int(y_stop)+1)
            z: range = range(int(z_start),int(z_stop)+1)
            bricks.append(Brick([x,y,z]))
        return bricks

    @staticmethod
    def fall() -> None: 
        pass
 

def __str__(self) -> str:
        """
            Used for debugging
        """
        return (
            f"{self.intervals[X].start},"
            f"{self.intervals[Y].start},"
            f"{self.intervals[Z].start}~"
            f"{self.intervals[X].stop},"
            f"{self.intervals[Y].stop},"
            f"{self.intervals[Z].stop}"
    )

    def __hash__(self) -> int:
        return hash( tuple( (i.start,i.stop) for i in self.intervals) )

    def __eq__(self,other) -> bool:
        if not isinstance(other,Brick) :
            return False
        return all( i == j for i,j in zip(self.intervals,other.intervals) )

if __name__=="__main__":
    # Parse 
    bricks : list[Brick] = []
    with open("input.txt") as f:
       lines = f.readlines()
       bricks = Brick.parse_bricks(lines)
    # Sort 
    bricks.sort(key= lambda brick : brick.intervals[Z].start)
    

