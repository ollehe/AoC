#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const uint64_t MAX_LINES = 4096;
static const uint64_t MAX_LINE_LENGTH = 2048;

typedef struct {
	uint64_t low;
	uint64_t high;
	bool isFresh;
} Range;

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

static inline uint64_t maximum(uint64_t a, uint64_t b)
{
	return a > b ? a : b;
}

static int compareRanges(const void *a, const void *b)
{
	Range rangeA = *(const Range *)a;
	Range rangeB = *(const Range *)b;
	if (rangeA.low < rangeB.low) {
		return -1;
	}
	if (rangeA.low > rangeB.low) {
		return 1;
	}
	return 0;
}

static uint64_t countNumberOfRanges(char **data, uint64_t rows)
{
	uint64_t count = 0;
	for (uint64_t i = 0; i < rows; i++) {
		// If line is empty, we have passed all ranges
		if (data[i][0] == '\0') {
			return i;
		}
	}
	return count;
}

static void parseRange(const char *str, Range *range)
{
	sscanf(str, "%zu-%zu", &range->low, &range->high);
}

static uint64_t parseData(char **data, uint64_t rows, Range **ranges,
			  uint64_t **ids)
{
	uint64_t numberOfRanges = countNumberOfRanges(data, rows);
	// Since the data contains 1 empty line
	uint64_t numberOfIds = rows - numberOfRanges - 1;
	// Allocate memory for ranges and ids
	*ranges = malloc(numberOfRanges * sizeof(Range));
	if (*ranges == NULL) {
		perror("Error allocating memory");
		exit(EXIT_FAILURE);
	}

	*ids = malloc(numberOfIds * sizeof(uint64_t));
	if (*ids == NULL) {
		perror("Error allocating memory");
		free(*ranges);
		exit(EXIT_FAILURE);
	}

	for (uint64_t i = 0; i < numberOfRanges; i++) {
		parseRange(data[i], &(*ranges)[i]);
		(*ranges)[i].isFresh = false;
	}
	for (uint64_t i = numberOfRanges + 1; i < rows; i++) {
		(*ids)[i - numberOfRanges - 1] = atoll(data[i]);
	}

	return numberOfRanges;
}

static bool isContained(uint64_t id, Range r)
{
	return id >= r.low && id <= r.high;
}

static bool isFresh(uint64_t id, Range *ranges, uint64_t numberOfRanges)
{
	bool isFresh = false;
	for (uint64_t i = 0; i < numberOfRanges; i++) {
		if (isContained(id, ranges[i])) {
			ranges[i].isFresh = true;
			isFresh = true;
		}
	}
	return isFresh;
}

static uint64_t countNumberOfFreshIDs(uint64_t *ids, uint64_t numberOfIds,
				      Range *ranges, uint64_t numberOfRanges)
{
	uint64_t count = 0;
	for (uint64_t i = 0; i < numberOfIds; i++) {
		if (isFresh(ids[i], ranges, numberOfRanges)) {
			count++;
		}
	}
	return count;
}

static uint64_t unionOfRanges(Range *ranges, uint64_t numberOfRanges,
			      Range **result)
{
	qsort(ranges, numberOfRanges, sizeof(Range), compareRanges);
	*result = malloc(numberOfRanges * sizeof(Range));
	if (!*result) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	uint64_t count = 0;
	(*result)[count] = ranges[0];
	for (uint64_t i = 1; i < numberOfRanges; i++) {
		if ((*result)[count].high + 1 < ranges[i].low) {
			(*result)[++count] = (Range){ ranges[i].low,
						      ranges[i].high,
						      ranges[i].isFresh };
		} else {
			(*result)[count].high =
				maximum((*result)[count].high, ranges[i].high);
		}
	}
	return count + 1;
}

static uint64_t countTotalNumberOfFreshIngredients(Range *ranges,
						   uint64_t numberOfRanges)
{
	uint64_t count = 0;
	// Form the unions
	Range *unions;
	uint64_t unionCount = unionOfRanges(ranges, numberOfRanges, &unions);
	printf("Number of fresh ranges: %zu\n", unionCount);
	for (uint64_t i = 0; i < unionCount; i++) {
		count += unions[i].high - unions[i].low + 1;
	}

	free(unions);
	return count;
}

int main()
{
	char *data[MAX_LINES];
	uint64_t rows = readFile("input.txt", data);

	Range *ranges;
	uint64_t *ids;
	uint64_t numberOfRanges = parseData(data, rows, &ranges, &ids);
	uint64_t numberOfIds = rows - numberOfRanges - 1;

	printf("Number of fresh IDs: %zu\n",
	       countNumberOfFreshIDs(ids, numberOfIds, ranges, numberOfRanges));

	printf("Total number of fresh ingredients: %zu\n",
	       countTotalNumberOfFreshIngredients(ranges, numberOfRanges));

	for (uint64_t i = 0; i < rows; i++) {
		free(data[i]);
	}

	free(ranges);
	free(ids);

	return 0;
}
