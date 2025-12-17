#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const uint64_t MAX_LINES = 4096;
static const uint64_t MAX_LINE_LENGTH = 4096;
typedef struct {
	uint64_t row;
	uint64_t col;
} Coord;

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

static void locateStartingPoint(char **grid, uint64_t rows, uint64_t *row,
				uint64_t *col)
{
	uint64_t columns = strlen(grid[0]);
	for (uint64_t i = 0; i < rows; i++) {
		for (uint64_t j = 0; j < columns; j++) {
			if (grid[i][j] == 'S') {
				*row = i;
				*col = j;
				return;
			}
		}
	}
}

static bool split(char ***grid, uint64_t columns, uint64_t row, uint64_t col)
{
	if ((*grid)[row][col] == '.') {
		(*grid)[row][col] = '|';
		return false;
	} else if ((*grid)[row][col] == '^') {
		for (int columnShift = -1; columnShift <= 1; columnShift++) {
			// Skip zero shift
			if (columnShift == 0) {
				continue;
			}
			// Check if new column is within bounds
			long newColumn = col + columnShift;
			if (newColumn >= 0 && newColumn < (long long)columns) {
				(*grid)[row][newColumn] = '|';
			}
		}
		return true;
	}
	return false;
}

static void countNumberOfSplits(char **grid, uint64_t rows)
{
	uint64_t columns = strlen(grid[0]);
	uint64_t startingRow;
	uint64_t startingCol;
	locateStartingPoint(grid, rows, &startingRow, &startingCol);
	uint64_t numberOfSplits = 0;

	// Initialize the grid with the starting point
	grid[startingRow + 1][startingCol] = '|';
	for (uint64_t row = 1; row < rows - 1; row++) {
		for (uint64_t col = 0; col < columns; col++) {
			if (grid[row][col] == '|') {
				if (split(&grid, columns, row + 1, col)) {
					numberOfSplits++;
				}
			}
		}
	}
	printf("Number of splits: %lu\n", numberOfSplits);
}
static void copyGrid(char **grid, uint64_t rows, uint64_t columns,
		     char ***newGrid)
{
	*newGrid = malloc(rows * sizeof(**newGrid));
	if (*newGrid == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	for (uint64_t i = 0; i < rows; i++) {
		(*newGrid)[i] = malloc(columns + 1);
		if ((*newGrid)[i] == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		memcpy((*newGrid)[i], grid[i], columns + 1); // includes '\0'
	}
}

static void branch(char **grid, uint64_t columns, uint64_t row, uint64_t col,
		   uint64_t *previousTimelines, uint64_t *currentTimelines)
{
	if (previousTimelines[col] == 0) {
		return;
	}
	if (grid[row][col] == '.') {
		currentTimelines[col] += previousTimelines[col];
	} else if (grid[row][col] == '^') {
		if (col >= 1) {
			currentTimelines[col - 1] += previousTimelines[col];
		}
		if (col + 1 < columns) {
			currentTimelines[col + 1] += previousTimelines[col];
		}
	}
}

static void countNumberOfTimelines(char **grid, uint64_t rows)
{
	uint64_t columns = strlen(grid[0]);
	uint64_t startingRow;
	uint64_t startingCol;
	locateStartingPoint(grid, rows, &startingRow, &startingCol);
	uint64_t *previousTimelines = calloc(columns, sizeof(uint64_t));
	uint64_t *currentTimelines = calloc(columns, sizeof(uint64_t));
	previousTimelines[startingCol] = 1;
	for (uint64_t row = startingRow + 1; row < rows; row++) {
		memset(currentTimelines, 0, columns * sizeof(uint64_t));
		for (uint64_t col = 0; col < columns; col++) {
			branch(grid, columns, row, col, previousTimelines,
			       currentTimelines);
		}
		memcpy(previousTimelines, currentTimelines,
		       columns * sizeof(uint64_t));
	}
	uint64_t numberOfTimelines = 0;
	for (uint64_t col = 0; col < columns; col++) {
		numberOfTimelines += previousTimelines[col];
	}
	printf("Number of timelines: %lu\n", numberOfTimelines);
	free(previousTimelines);
	free(currentTimelines);
}
int main(void)
{
	char *grid[MAX_LINES];
	uint64_t rows = readFile("input.txt", grid);

	uint64_t columns = strlen(grid[0]);
	char **newGrid = NULL;
	copyGrid(grid, rows, columns, &newGrid);

	countNumberOfSplits(grid, rows);
	countNumberOfTimelines(newGrid, rows);

	for (uint64_t i = 0; i < rows; i++)
		free(grid[i]);

	for (uint64_t i = 0; i < rows; i++)
		free(newGrid[i]);
	free(newGrid);

	return 0;
}
