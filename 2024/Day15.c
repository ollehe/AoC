§#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINES 1000
#define MAX_LINE_LENGTH 2000
#define MAX(x, y) ((x > y) ? x : y)
typedef struct
{
	int row;
	int col;
	int row_dir;
	int col_dir;
} Robot;

typedef struct 
{
	int row;
	int leftCol;
	int rightCol;
} Box;

int
readFileAsStrings(const char *fileName, char *lines[])
{
	printf("Buffer size: Lines: %d Line lineth %d\n", MAX_LINES, MAX_LINE_LENGTH);
	FILE *file = fopen(fileName, "r");
	uint64_t maxLength = 0;
	if (file == NULL)
	{
		perror("Error opening file");
		return -1; // Return an error code if the file cannot be opened
	}
	char buffer[MAX_LINE_LENGTH];
	uint64_t count = 0;
	while (fgets(buffer, sizeof(buffer), file) != NULL && count < MAX_LINES)
	{
		lines[count] = malloc(strlen(buffer) + 1); // Allocate memory for each line
		if (lines[count] == NULL)
		{
			perror("Error allocating memory");
			fclose(file);
			return -1;
		}

		strcpy(lines[count], buffer); // Copy the line into the array

		size_t len = strlen(lines[count]);
		if (len > 0 && lines[count][len - 1] == '\n')
		{
			lines[count][len - 1] = '\0'; // Replace newline with null terminator
		}
		maxLength = MAX(maxLength, strlen(lines[count]));
		count++;
	}
	fclose(file);
	printf("Number of lines read: %llu \n", count);
	printf("Longest line read: %llu \n", maxLength);
	return count; // Return the number of lines read
}

void
getMap(char **data, int numberOfLines, char ***gridByReference, int *numberOfRows, int *numberOfColumns)
{
	int line = 0;

	int cols = strlen(data[0]);
	int rows = 0;

	while (strlen(data[rows]) > 0)
	{
		rows++;
	}
	*gridByReference = (char **)malloc(rows * sizeof(char *));
	if (*gridByReference == NULL)
	{
		printf("UHOH!\n");
		return;
	}
	for (int i = 0; i < rows; i++)
	{
		(*gridByReference)[i] = (char *)malloc((cols + 1) * sizeof(char));
		if (*(*gridByReference) == NULL)
		{
			printf("UHOH!\n");
			return;
		}
	}
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			(*gridByReference)[row][col] = data[row][col];
		}
		(*gridByReference)[row][cols] = '\0';
	}
	*numberOfRows = rows;
	*numberOfColumns = cols;
	return;
}

void
getInstructions(char **data, int numberOfRows, char ***instructionsByReference, int *numberOfLinesOfInstructions)
{
	int startRow = 0;
	while (strlen(data[startRow]) > 0)
	{
		startRow++;
	}
	startRow++;

	*numberOfLinesOfInstructions = numberOfRows - startRow;

	*instructionsByReference = (char **)malloc((numberOfRows - startRow) * sizeof(char *));

	for (int i = startRow; i < numberOfRows; i++)
	{
		(*instructionsByReference)[i - startRow] = (char *)malloc((strlen(data[i]) + 1) * sizeof(char));
		strcpy((*instructionsByReference)[i - startRow], data[i]);
	}
}

void
findRobot(char **grid, int rows, int cols, Robot *robot)
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if (grid[row][col] == '@')
			{
				robot->row = row;
				robot->col = col;
				robot->row_dir = 0;
				robot->col_dir = 0;
			}
		}
	}
}
void
draw(char **grid, int rows)
{
	for (int row = 0; row < rows; row++)
	{
		printf("%s\n", grid[row]);
	}
}

/*
 *	Check if we can move. If the next character is # we cannot while if it is . we can.
 *	If it is '0' we check the next.
 */
bool
canMove(char **grid, Robot r)
{
	int nextRow = r.row + r.row_dir;
	int nextCol = r.col + r.col_dir;
	if (grid[nextRow][nextCol] == '#')
	{
		return false;
	}
	else if (grid[nextRow][nextCol] == '.')
	{
		return true;
	} // Modification for part 2. If robot is facing up or down and next row is a box
	else if (r.col_dir == 0 && (grid[nextRow][nextCol]=='[' ||  grid[nextRow][nextCol]==']' ) )
	{
		r.row += r.row_dir;
		r.col += r.col_dir;
		/*
		 *	Separate into cases (and the reflected ones) : 
		 *	[]    and     []
		 *	@.            .@
		 */
		if(grid[nextRow][nextCol]=='[') 
		{
			Robot shiftedToTheRight = r;
			shiftedToTheRight.col+=1;
			return true && canMove(grid,r) && canMove(grid,shiftedToTheRight);
		}
		else 
		{
			Robot shiftedToTheLeft = r;
			shiftedToTheLeft.col-=1;
			return true && canMove(grid,r) && canMove(grid,shiftedToTheLeft);
		}
	}
	else
	{
		r.row += r.row_dir;
		r.col += r.col_dir;
		return true && canMove(grid, r);
	}
}

void
setDirection(Robot *r, char direction)
{
	if (direction == '^')
	{
		r->row_dir = -1;
		r->col_dir = 0;
	}
	else if (direction == 'v')
	{
		r->row_dir = 1;
		r->col_dir = 0;
	}
	else if (direction == '<')
	{
		r->row_dir = 0;
		r->col_dir = -1;
	}
	else if (direction == '>')
	{
		r->row_dir = 0;
		r->col_dir = 1;
	}
}


bool
isPresent(Box * boxes, int numberOfBoxes, Box b)
{
	for ( int i = 0 ; i < numberOfBoxes ; i++)
	{
		if( (boxes[i].leftCol == b.leftCol) && (boxes[i].rightCol == b.rightCol )&& (boxes[i].row == b.row))
		{
			return true;
		}
	}
	return false;
}

void
findBoxes(Box *boxes, int* numberOfBoxes, int row, int col, int rowDirection, char ** grid)
{
	int nextRow = row+rowDirection;
	if (grid[nextRow][col] == '.')
	{
		return;
	}
	else if (grid[nextRow][col] == '[')
	{
		Box b = {.row = nextRow, .leftCol = col, .rightCol = col+1};
		// Add check to see if box already has been added
		if (!isPresent(boxes, *numberOfBoxes, b))
		{
			boxes[*numberOfBoxes] = b;
			(*numberOfBoxes)++;
		}
		// Look up/downwards to the next two contact points
		findBoxes(boxes,numberOfBoxes,nextRow,col,rowDirection,grid);
		findBoxes(boxes,numberOfBoxes,nextRow,col+1,rowDirection,grid);
	}
	else if (grid[nextRow][col] == ']')
	{
		Box b = {.row = nextRow, .leftCol = col-1, .rightCol = col};
		// Check to see if box already has been added
		if (!isPresent(boxes, *numberOfBoxes, b))
		{
			boxes[*numberOfBoxes] = b;
			(*numberOfBoxes)++;
		}
		// Look up/downwards to the next two contact points
		findBoxes(boxes,numberOfBoxes,nextRow,col-1,rowDirection,grid);
		findBoxes(boxes,numberOfBoxes,nextRow,col,rowDirection,grid);
	}
}


void
move(char ***gridByReference, Robot *r, int rows, int cols, bool part1)
{
	int currentRow = r->row;
	int currentCol = r->col;
	if( r->row_dir == 0 || part1 )
	{ 
		// This part works for Part 1, i.e. both horizontally and vertically 

		r->row += r->row_dir;
		r->col += r->col_dir;

		char current;
		char prev = '.';

		while ((*gridByReference)[currentRow][currentCol] != '.')
		{
			current = (*gridByReference)[currentRow][currentCol];
			(*gridByReference)[currentRow][currentCol] = prev;
			prev = current;
			currentRow += r->row_dir;
			currentCol += r->col_dir;
		}
		(*gridByReference)[currentRow][currentCol] = prev;
	}
	else // Part 2 modification.
	{
		Box *boxes = (Box *)malloc(rows * cols * sizeof(Box));
		int numberOfBoxes = 0;
		findBoxes(boxes,&numberOfBoxes,r->row,r->col,r->row_dir, *gridByReference);

		if( numberOfBoxes == 0 )
		{
			(*gridByReference)[currentRow][currentCol] ='.';
			r->row += r->row_dir;
			r->col += r->col_dir;
			(*gridByReference)[r->row][r->col] ='@';
		} else 
		{
			printf("Found %d boxes to push\n", numberOfBoxes);

			for(int i = 0 ; i < numberOfBoxes ; i ++)
			{
				printf("Row: %d, Columns :(%d,%d)\n", boxes[i].row, boxes[i].leftCol, boxes[i].rightCol);
			}
			for( int i = 0 ; i < numberOfBoxes ; i ++)
			{
				(*gridByReference)[boxes[i].row][boxes[i].leftCol] = '.';
				(*gridByReference)[boxes[i].row][boxes[i].rightCol] = '.';
			}
			for (int i = 0; i < numberOfBoxes; i++)
			{
				(*gridByReference)[boxes[i].row+r->row_dir][boxes[i].leftCol] = '[';
				(*gridByReference)[boxes[i].row+r->row_dir][boxes[i].rightCol] = ']';
			}

			(*gridByReference)[currentRow][currentCol] = '.';
			r->row += r->row_dir;
			r->col += r->col_dir;
			(*gridByReference)[r->row][r->col] ='@';
		}
		free(boxes);	
	}
}

void
simulate(char ***gridByReference, int rows, int cols, char **listOfInstructions, int numberOfLines, Robot r, bool part1)
{
	for (int line = 0; line < numberOfLines; line++)
	{
		for (int i = 0; i < strlen(listOfInstructions[line]); i++)
		{
			printf("Try to move %c\n", listOfInstructions[line][i]);
			setDirection(&r, listOfInstructions[line][i]);
			if (canMove(*gridByReference, r))
			{
				printf("Can move\n");
				move(gridByReference, &r,rows,cols, part1);
				draw(*gridByReference, rows);
			}
		}
	}
}

int
computeGPSCoordinates(char **grid, int rows, int cols)
{


	int sum = 0;
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if ((grid[row][col] == 'O') || grid[row][col] == '[')
			{
				sum += 100 * row + col;
			}
		}
	}

	int numberOfBoxes = 0;
	Box *boxes = (Box *)malloc(cols * rows * sizeof(Box));


	return sum;
}

void
enlargeGrid(char **originalGrid, char ***largerGrid, int rows, int cols)
{
	(*largerGrid) = (char **) malloc(rows*sizeof(char*));
	for( int row = 0 ; row < rows ; row ++)
	{
		(*largerGrid)[row] = (char *) malloc(2*cols*sizeof(char)+1);
	}
	size_t doubleCol = 0;
	for(int row = 0 ; row < rows ; row ++)
	{
		doubleCol = 0;
		for( int col = 0 ; col < cols ; col ++)
		{
			if(originalGrid[row][col] == 'O')
			{
				(*largerGrid)[row][doubleCol++] = '[';
				(*largerGrid)[row][doubleCol++] = ']';
			}
			else if( originalGrid[row][col] == '@')
			{
				(*largerGrid)[row][doubleCol++] = '@';
				(*largerGrid)[row][doubleCol++] = '.';
			}
			else 
			{
				(*largerGrid)[row][doubleCol++] = originalGrid[row][col];
				(*largerGrid)[row][doubleCol++] = originalGrid[row][col];
			}
		}
		(*largerGrid)[row][2*cols] = '\0';
	}
}
int
main()
{
	char *data[MAX_LINES];
	int numberOfLines = readFileAsStrings("input.txt", data);
	/*
	 *	Initialize
	 */
	char **grid;
	int rows = 0;
	int cols = 0;

	char **instructions;
	int linesOfInstructions = 0;
	Robot r;
	char **secondGrid;

	getMap(data, numberOfLines, &grid, &rows, &cols);
	getInstructions(data, numberOfLines, &instructions, &linesOfInstructions);
	findRobot(grid, rows, cols, &r);
	enlargeGrid(grid,&secondGrid, rows,cols);
	
	/*
	 *	Part 1
	 */
	printf("Initial step\n");
	draw(grid, rows);
	simulate(&grid, rows, cols, instructions, linesOfInstructions, r, true);
	printf("Total GPS sum %d\n", computeGPSCoordinates(grid, rows, cols));

	/*
	 *	Part 2
	 */
	draw(secondGrid,rows);
	findRobot(secondGrid,rows,2*cols,&r);
	simulate(&secondGrid,rows,2*cols,instructions,linesOfInstructions,r,false);
	printf("Total GPS sum %d \n", computeGPSCoordinates(secondGrid,rows, 2*cols));
	/*
	 *	Free memory
	 */
	for (int row = 0; row < rows; row++)
	{
		free(grid[row]);
		free(secondGrid[row]);
	}
	free(grid);
	free(secondGrid);
	for (int i = 0; i < linesOfInstructions; i++)
	{
		free(instructions[i]);
	}
	free(instructions);
	return 0;
}
