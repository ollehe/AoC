#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const uint64_t MAX_LINES = 2048;
static const uint64_t MAX_LINE_LENGTH = 1024;
static const char PAPER_ROLL = '@';
static const char EMPTY = '.';
static const uint8_t THRESHOLD = 4;
#define MAX(x, y) ((x > y) ? x : y)

typedef struct {
	int row;
	int column;
} Coordinate;

static inline int maximum(long a, long b)
{
	return (a > b) ? a : b;
}

static uint64_t readFile(const char *fileName, char **lines)
{
	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	char buffer[MAX_LINE_LENGTH];
	uint64_t count = 0;
	while (fgets(buffer, sizeof(buffer), file) != NULL &&
	       count < MAX_LINES) {
		lines[count] = malloc(strlen(buffer) +
				      1); // Allocate memory for each line
		if (lines[count] == NULL) {
			perror("Error allocating memory");
			fclose(file);
			exit(EXIT_FAILURE);
		}

		strcpy(lines[count], buffer); // Copy the line into the array

		uint64_t len = strlen(lines[count]);
		if (len > 0 && lines[count][len - 1] == '\n') {
			lines[count][len - 1] =
				'\0'; // Replace newline with null terminator
		}
		count++;
	}
	fclose(file);
	return count; // Return the number of lines read
}

static Coordinate shift(Coordinate c, Coordinate shift)
{
	return (Coordinate){ c.row + shift.row, c.column + shift.column };
}
static bool isValid(Coordinate coord, int rows, int columns)
{
	return (coord.row >= 0 && coord.row < rows && coord.column >= 0 &&
		coord.column < columns);
}

static uint64_t getNumberOfNeighbors(Coordinate coord, int rows, int columns)
{
	uint64_t count = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;
			Coordinate neighbor =
				shift(coord, (Coordinate){ i, j });
			if (isValid(neighbor, rows, columns)) {
				count++;
			}
		}
	}

	return count;
}

static void getNeighbors(Coordinate coord, int rows, int columns,
			 uint64_t *numberOfNeighbors, Coordinate **neighbors)
{
	*numberOfNeighbors = getNumberOfNeighbors(coord, rows, columns);
	*neighbors = malloc(*numberOfNeighbors * sizeof(Coordinate));
	if (!*neighbors) {
		perror("Unable to malloc in getNeighbors");
		return;
	}

	int index = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;
			Coordinate neighbor =
				shift(coord, (Coordinate){ i, j });
			if (isValid(neighbor, rows, columns)) {
				(*neighbors)[index++] = neighbor;
			}
		}
	}
}

static uint64_t countNumberOfIncidentPaperRolls(Coordinate coord, int rows,
						int columns, char **grid)
{
	uint64_t count = 0;
	uint64_t numberOfNeighbors;
	Coordinate *neighbors = NULL;
	getNeighbors(coord, rows, columns, &numberOfNeighbors, &neighbors);
	for (uint64_t i = 0; i < numberOfNeighbors; i++) {
		if (grid[neighbors[i].row][neighbors[i].column] == PAPER_ROLL)
			count++;
	}
	free(neighbors);
	return count;
}
static bool canRemovePaperRoll(Coordinate coord, int rows, int columns,
			       char **grid)
{
	return countNumberOfIncidentPaperRolls(coord, rows, columns, grid) <
	       THRESHOLD;
}

static bool findAccessiblePaperRoll(int rows, int columns, char **grid,
				    Coordinate *coord)
{
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < columns; column++) {
			if (grid[row][column] == PAPER_ROLL &&
			    canRemovePaperRoll((Coordinate){ row, column },
					       rows, columns, grid)) {
				coord->row = row;
				coord->column = column;
				return true;
			}
		}
	}

	return false;
}

static uint64_t findNumberOfAccessiblePaperRolls(int rows, int columns,
						 char **grid)
{
	uint64_t numberOfAccessiblePaperRolls = 0;
	for (int row = 0; row < rows; row++) {
		for (int column = 0; column < columns; column++) {
			if (grid[row][column] == PAPER_ROLL &&
			    canRemovePaperRoll((Coordinate){ row, column },
					       rows, columns, grid)) {
				numberOfAccessiblePaperRolls++;
			}
		}
	}

	return numberOfAccessiblePaperRolls;
}

static uint64_t removePaperRolls(int rows, int columns, char **grid)
{
	uint64_t count = 0;
	Coordinate coord = { 0 };
	while (findAccessiblePaperRoll(rows, columns, grid, &coord)) {
		grid[coord.row][coord.column] = EMPTY;
		count++;
	}
	return count;
}

int main()
{
	char *grid[MAX_LINES];
	uint64_t rows = readFile("input.txt", grid);
	uint64_t columns = strlen(grid[0]);
	printf("Number of accessible paper rolls: %zu\n",
	       findNumberOfAccessiblePaperRolls(rows, columns, grid));
	printf("Number of removed paper rolls: %zu\n",
	       removePaperRolls(rows, columns, grid));
	for (uint64_t i = 0; i < rows; i++) {
		free(grid[i]);
	}
}
