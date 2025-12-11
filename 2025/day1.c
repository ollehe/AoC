#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
static const uint64_t MAX_LINES = 5000;
static const uint64_t MAX_LINE_LENGTH = 10;
static const char LEFT = 'L';
static const char RIGHT = 'R';
static const int STARTING_POSITION = 50;
static const int DIAL_SIZE = 100;

#define MAX(x, y) ((x > y) ? x : y)

static int readFile(const char *fileName, char **lines)
{
	//	printf("Buffer size: Lines: %luLine length %lu\n", MAX_LINES, MAX_LINE_LENGTH);
	FILE *file = fopen(fileName, "r");
	uint64_t maxLength = 0;
	if (file == NULL) {
		perror("Error opening file");
		return -1; // Return an error code if the file cannot be opened
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
			return -1;
		}

		strcpy(lines[count], buffer); // Copy the line into the array

		uint64_t len = strlen(lines[count]);
		if (len > 0 && lines[count][len - 1] == '\n') {
			lines[count][len - 1] =
				'\0'; // Replace newline with null terminator
		}
		maxLength = MAX(maxLength, strlen(lines[count]));
		count++;
	}
	fclose(file);
	//	printf("Number of lines read: %lu \n", count);
	//	printf("Longest line read: %lu \n", maxLength);
	return count; // Return the number of lines read
}

static int turnDial(char **instructions, int numberOfInstructions, bool part2)
{
	int currentPosition = STARTING_POSITION;
	int previousPosition = currentPosition;
	char direction;
	int numberOfSteps;
	int numberOfZeros = 0;
	int numberOfPasses = 0;
	for (int i = 0; i < numberOfInstructions; i++) {
		direction = instructions[i][0];
		numberOfSteps = atoi(instructions[i] + 1);

		numberOfPasses += numberOfSteps / DIAL_SIZE;
		numberOfSteps %= DIAL_SIZE;
		previousPosition = currentPosition;
		if (direction == LEFT) {
			currentPosition -= numberOfSteps;
		} else if (direction == RIGHT) {
			currentPosition += numberOfSteps;
		}
		if (previousPosition != 0 &&
		    (currentPosition > 99 || currentPosition < 1)) {
			numberOfPasses++;
		}
		currentPosition = (currentPosition + DIAL_SIZE) % DIAL_SIZE;
		if (currentPosition == 0) {
			numberOfZeros++;
		}
	}
	return part2 ? numberOfPasses : numberOfZeros;
}

int main()
{
	char *instructions[MAX_LINES];
	int numberOfLines = readFile("input.txt", instructions);
	int numberOfZeros = turnDial(instructions, numberOfLines, false);
	printf("Part 1: Number of zeros: %d\n", numberOfZeros);
	numberOfZeros = turnDial(instructions, numberOfLines, true);
	printf("Part 2: Number of zeros: %d\n", numberOfZeros);

	for (int i = 0; i < numberOfLines; i++) {
		free(instructions[i]);
	}

	return 0;
}
