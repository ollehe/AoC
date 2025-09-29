from __future__ import annotations

X = 0
Y = 1
Z = 2
# LETTERS: list[str] = ['A', 'B','C','D','E','F','G']

class Brick:

    def __init__(self, intervals : list[range]):
        self.letter: str = ""
        self.intervals = intervals
        self.above : list[Brick] = []
        self.below : list[Brick] = []

    @staticmethod
    def parse_bricks(data : list[str]) -> list[Brick] :
        """
	    Parses input (as list of strings) and outputs a list of Bricks
        """
        bricks: list[Brick] = []*len(data)
        for index,line in enumerate(data) :
            start,stop = line.split('~')
            x_start,y_start,z_start = start.split(',')
            x_stop,y_stop,z_stop = stop.split(',')
            x: range = range(int(x_start),int(x_stop)+1)
            y: range = range(int(y_start),int(y_stop)+1)
            z: range = range(int(z_start),int(z_stop)+1)
            new_brick = Brick([x,y,z])
            # new_brick.letter = LETTERS[index]
            bricks.append(new_brick)
        return bricks

    def __ge__(self, a: int) -> bool:
        return self.intervals[Z].start >= a

    def __gt__(self, a: int) -> bool:
        return self.intervals[Z].start > a

    def decrease(self) -> None:
        self.intervals[Z] = range(
            self.intervals[Z].start-1, self.intervals[Z].stop-1
           )
        return

    def increase(self) -> None:
        self.intervals[Z] = range(
            self.intervals[Z].start+1, self.intervals[Z].stop+1
            )
        return

    def intersect(self:Brick, other:Brick) -> bool:
        def cap(r1:range,r2:range) -> bool:
            return r1.start <= r2.stop - 1 and r2.start <= r1.stop - 1
        return all( cap(a,b) for a,b in zip(self.intervals,other.intervals))

    @staticmethod
    def fall(sorted_bricks: list[Brick]) -> None: 
        for brick in sorted_bricks:
            is_falling: bool = True
            while is_falling:
                brick.decrease()
                for other_brick in sorted_bricks:
                    if other_brick == brick:
                        continue
                    elif brick.intersect(other_brick):
                        brick.below.append(other_brick)
                        other_brick.above.append(brick)
                        is_falling = False
                is_falling = is_falling and brick >= 1
            brick.increase()
        return

    def __str__(self) -> str:
        """
            Used for debugging
        """
        return (
            f"{self.letter} "
            f"{self.intervals[X].start},"
            f"{self.intervals[Y].start},"
            f"{self.intervals[Z].start}~"
            f"{self.intervals[X].stop-1},"
            f"{self.intervals[Y].stop-1},"
            f"{self.intervals[Z].stop-1}"
        )

    def __hash__(self) -> int:
        return hash( tuple( (i.start,i.stop) for i in self.intervals) )

    def __eq__(self,other) -> bool:
        if not isinstance(other,Brick) :
            return False
        return all( i == j for i,j in zip(self.intervals,other.intervals) )

    def can_disintegrate(self) -> bool:
        return all( len(brick.below) > 1 for brick in self.above )

if __name__=="__main__":
    # Parse
    bricks : list[Brick] = []
    with open("input.txt") as f:
       lines = f.readlines()
       bricks = Brick.parse_bricks(lines)
    # Sort
    bricks.sort(key= lambda brick : brick.intervals[Z].start)
    # Fall
    Brick.fall(bricks)
    # Disintegrate
    total = 0
    for brick in bricks:
        if brick.can_disintegrate():
            total += 1
    print(total)
