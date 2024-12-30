#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 3000
#define MAX_LINE_LENGTH 2000
#define MAX(x, y) ((x > y) ? x : y)
#define PRUNE 16777216
#define PATTERN_LENGTH 4
#define REPRESENTATION_SIZE 21
#define PURCHASE_SIZES REPRESENTATION_SIZE*REPRESENTATION_SIZE*REPRESENTATION_SIZE*REPRESENTATION_SIZE
#define SAMPLE_SIZE 2001

int
readFileAsStrings(const char *fileName, char *lines[])
{
	printf("Buffer size: Lines: %d Line length %d\n", MAX_LINES, MAX_LINE_LENGTH);
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
convert(char *data[MAX_LINES], int numberOfLInes, uint64_t *inits)
{
	for (int i = 0; i < numberOfLInes; i++)
	{
		(inits)[i] = (uint64_t)atoi(data[i]);
	}
}

void
mix(uint64_t *secretNumber, uint64_t result)
{
	(*secretNumber) = (*secretNumber) ^ result;
}

void
prune(uint64_t *secretNumber)
{
	(*secretNumber) = (*secretNumber) % PRUNE;
}

void
newSecretNumber(uint64_t *secretNumber)
{
	uint64_t result = 64 * (*secretNumber);
	mix(secretNumber, result);
	prune(secretNumber);
	result = (*secretNumber) / 32;
	mix(secretNumber, result);
	prune(secretNumber);
	result = (*secretNumber) * 2048;
	mix(secretNumber, result);
	prune(secretNumber);
}
void
generateNthSecretNumber(uint64_t *secretNumber, uint64_t steps)
{
	for (int i = 0; i < steps; i++)
	{
		newSecretNumber(secretNumber);
	}
}
uint64_t
sum(uint64_t *secretNumbers, int numberOfNumbers)
{
	uint64_t total = 0;
	for (int i = 0; i < numberOfNumbers; i++)
	{
		total += secretNumbers[i];
	}
	return total;
}

uint64_t
hashMap(int sequence[PATTERN_LENGTH])
{
	uint64_t index = 1 * (sequence[0] + 9) 
		+ REPRESENTATION_SIZE * (sequence[1] + 9) 
		+ REPRESENTATION_SIZE * REPRESENTATION_SIZE * (sequence[2] + 9) 
		+ REPRESENTATION_SIZE * REPRESENTATION_SIZE * REPRESENTATION_SIZE * (sequence[3] + 9);
	return index;
}

void
priceSequences(uint64_t possiblePurchases[PURCHASE_SIZES], uint64_t *secretNumber)
{
	int sequence[PATTERN_LENGTH];
	int prices[SAMPLE_SIZE ] = {0};
	int maximalPrices[PURCHASE_SIZES] = {0};
	for( int i = 0 ; i < PURCHASE_SIZES ; i++)
	{
		maximalPrices[i] = -1;
	}


	// Generate prices and differences in one pass
	for (int i = 0; i < SAMPLE_SIZE ; i++)
	{
		prices[i] = (*secretNumber) % 10;
		newSecretNumber(secretNumber);
	}
	// Generate possible purchases by finding matching price change sequences
	for (int i = 0; i < SAMPLE_SIZE - PATTERN_LENGTH; i++)
	{
		// Extract the price change sequence
		for (int j = 0; j < PATTERN_LENGTH; j++)
		{
			sequence[j] = prices[i + j + 1] - prices[i + j];
		}
		// Get the hash index for the sequence
		uint64_t index = hashMap(sequence);
		if( maximalPrices[index]<0)
		// Add the current price to the corresponding index in the hash map
		{
			maximalPrices[index] = prices[i + PATTERN_LENGTH];
		}
	}
	for (int i = 0; i < PURCHASE_SIZES; i++)
	{
		if( maximalPrices[i]>0)
		{
			possiblePurchases[i] += maximalPrices[i];
		}
	}
}

uint64_t
buy(uint64_t *secretNumbers, int numberOfSecretNumbers)
{
	uint64_t possiblePurchases[PURCHASE_SIZES] = {0};
	for (int i = 0; i < numberOfSecretNumbers; i++)
	{
		priceSequences(possiblePurchases, &secretNumbers[i]);
	}
	uint64_t best = 0;
	int argMax;
	for (int i = 0; i < PURCHASE_SIZES; i++)
	{
		best = MAX(best, possiblePurchases[i]);
	}
	return best;
}

int
main()
{
	char *data[MAX_LINES];
	int numberOfLines = readFileAsStrings("input.txt", data);

	uint64_t *secretNumbers = (uint64_t *)malloc(numberOfLines * sizeof(uint64_t));
	convert(data, numberOfLines, secretNumbers);
	uint64_t stored;

	for (int i = 0; i < numberOfLines; i++)
	{
		generateNthSecretNumber(&secretNumbers[i], 2000);
	}
	uint64_t total = sum(secretNumbers, numberOfLines);
	printf("Part 1: %llu\n", total);

	convert(data, numberOfLines, secretNumbers);
	for (int i = 0; i < numberOfLines; i++)
	{
		free(data[i]); // Free each allocated line after processing
	}

	printf("Part 2 : %llu\n", buy(secretNumbers, numberOfLines));

	free(secretNumbers);
}