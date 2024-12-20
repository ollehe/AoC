#include <math.h>
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
findRobot( char ** grid, int rows, int cols, Robot * robot )
{
	for( int row = 0 ; row < rows ; row ++ )
	{
		for( int col = 0 ; col < cols ; col ++)
		{
			if(grid[row][col] == '@')
			{
				robot -> row = row;
				robot -> col = col;
				robot -> row_dir = 0;
				robot -> col_dir = 0;
			}
		}
	}
}
void
draw(char ** grid, int rows)
{
	for(int row = 0 ; row < rows ; row++)
	{
		printf("%s\n",grid[row]);
	}
}

/*
 *	Check if we can move. If the next character is # we cannot while if it is . we can. 
 *	If it is '0' we check the next. 
 */
bool
canMove(char ** grid , Robot r )
{
	// printf("Current position: (%d,%d)\n",r.row,r.col);
	int nextRow = r.row + r.row_dir;
	int nextCol = r.col + r.col_dir;
	// printf("Checking if we can move next: %d,%d\n",nextRow,nextCol);
	if( grid[nextRow][nextCol] == '#' )
	{
		printf("Can't move\n");
		return false;
		
	}
	else if (grid[nextRow][nextCol] == '.')
	{
		printf("Can move\n");
		return true;
	}
	else
	{
		r.row += r.row_dir;
		r.col += r.col_dir;
		return true && canMove(grid,r); 	
	}
}

void
setDirection(Robot *r, char direction )
{
	if( direction == '^')
	{
		r->row_dir = -1;
		r->col_dir = 0;
	}
	else if(direction =='v')
	{
		r->row_dir = 1;
		r->col_dir = 0;
	}
	else if(direction == '<')
	{
		r->row_dir = 0;
		r->col_dir = -1;
	}
	else if(direction == '>')
	{
		r->row_dir = 0;
		r->col_dir = 1;
	}
}

void
move(char ***gridByReference , Robot *r)
{

	int currentRow = r->row;
	int currentCol = r->col;
	r->row += r->row_dir;
	r->col += r->col_dir;
	
	char current ;
	char prev = '.';
	// printf("Shifting row %s\n", (*gridByReference)[currentRow] );
	// printf("First char: %c\n", (*gridByReference)[currentRow][currentCol] );
	
	while( (*gridByReference)[currentRow][currentCol] != '.')
	{
		// printf("Shifting: %c -> ", (*gridByReference)[currentRow][currentCol]);

		current = (*gridByReference)[currentRow][currentCol];
		(*gridByReference)[currentRow][currentCol] = prev;
		// printf("Previous: %c, Current: %c\n", prev, current);

		prev = current;
		currentRow += r->row_dir;
		currentCol += r->col_dir;
	}
	(*gridByReference)[currentRow][currentCol] = prev;

	// printf("Shifted row %s\n", (*gridByReference)[currentRow] );
}

void
simulate(char ***gridByReference, int rows, int cols, char ** listOfInstructions, int numberOfLines, Robot r )
{
	for( int line = 0 ; line < numberOfLines ; line ++ )
	{
		for(int i = 0 ; i < strlen(listOfInstructions[line]); i++ )
		{
			printf("Try to move %c\n", listOfInstructions[line][i]);
			setDirection(&r,listOfInstructions[line][i]);
			if(canMove(*gridByReference,r))
			{
				move(gridByReference,&r);
				draw(*gridByReference,rows);
			}
		}
	}

}

int
computeGSPCoordinates(char **grid, int rows, int cols )
{
	int sum = 0;
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			if(grid[row][col] == 'O')
			{
				sum += 100*row+col;
			}
		}
	}
	return sum;
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

	getMap(data, numberOfLines, &grid, &rows, &cols);
	getInstructions(data, numberOfLines, &instructions, &linesOfInstructions);
	findRobot(grid,rows,cols,&r);
	printf("Initial step\n");
	draw(grid,rows);
	simulate(&grid,rows,cols, instructions,linesOfInstructions,r);

	printf("Total GPS sum %d\n", computeGSPCoordinates(grid,rows,cols));
	/*
	 *	Free memory
	 */
	for (int row = 0; row < rows; row++)
	{
		free(grid[row]);
	}
	free(grid);
	for (int i = 0; i < linesOfInstructions; i++)
	{
		free(instructions[i]);
	}
	free(instructions);
	return 0;
}
