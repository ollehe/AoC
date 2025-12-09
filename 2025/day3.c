#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hash.h"

static const uint64_t MAX_LINES = 2048;
static const uint64_t MAX_LINE_LENGTH = 1024;
static const char ZERO_CHAR = '0';
#define MAX(x, y) ((x > y) ? x : y)

static uint64_t readFile(const char *fileName, char **lines)
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

		size_t len = strlen(lines[count]);
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

static uint64_t pow10(uint64_t exponent)
{
	uint64_t result = 1;
	for (uint64_t i = 0; i < exponent; i++) {
		result *= 10;
	}
	return result;
}

static uint64_t computeMaximalJoltage(char *batteryBank,
				      size_t numberOfActivated,
				      size_t currentPosition, size_t k,
				      HashTable *memo)
{
	uint64_t result;
	size_t totalNumberOfBatteries = strlen(batteryBank);
	size_t remaining = totalNumberOfBatteries - currentPosition;
	size_t needed = k - numberOfActivated;

	// Base: already picked k digits
	if (numberOfActivated == k) {
		return 0;
	}

	Key key = (Key){ numberOfActivated, currentPosition };
	if (lookUpKey(memo, key, &result)) {
		return result;
	}

	// Not enough digits left to reach k → impossible path
	if (remaining < needed) {
		result = 0;
		insertKey(memo, key, result);
		return result;
	}

	// Must take all remaining digits
	if (remaining == needed) {
		result = 0;
		for (size_t i = currentPosition; i < totalNumberOfBatteries;
		     ++i) {
			result = result * 10 +
				 (uint64_t)(batteryBank[i] - ZERO_CHAR);
		}
		insertKey(memo, key, result);
		return result;
	}

	// Either take or skip current digit
	uint64_t digit = (uint64_t)(batteryBank[currentPosition] - ZERO_CHAR);

	uint64_t take = digit * pow10(needed - 1) +
			computeMaximalJoltage(batteryBank,
					      numberOfActivated + 1,
					      currentPosition + 1, k, memo);

	uint64_t skip = computeMaximalJoltage(batteryBank, numberOfActivated,
					      currentPosition + 1, k, memo);

	result = MAX(take, skip);
	insertKey(memo, key, result);
	return result;
}

static uint64_t computeTotalJoltage(char **batteries, uint64_t numberOfLines,
				    uint8_t k)
{
	uint64_t totalJoltage = 0;
	HashTable memo;
	initializeHashTable(&memo, 0);

	for (uint64_t ii = 0; ii < numberOfLines; ii++) {
		totalJoltage +=
			computeMaximalJoltage(batteries[ii], 0, 0, k, &memo);
		clearHashTable(&memo);
	}

	freeHashTable(&memo);
	return totalJoltage;
}

int main()
{
	char *batteries[MAX_LINES];
	uint64_t numberOfLines = readFile("input.txt", batteries);
	uint64_t totalJoltage =
		computeTotalJoltage(batteries, numberOfLines, 2);
	printf("Total Joltage: %lu\n", totalJoltage);
	totalJoltage = computeTotalJoltage(batteries, numberOfLines, 12);
	printf("Total Joltage: %lu\n", totalJoltage);
	// Free mallocs
	for (uint64_t i = 0; i < numberOfLines; i++) {
		free(batteries[i]);
	}
}
