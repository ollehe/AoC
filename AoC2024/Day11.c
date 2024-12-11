#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Change this according to the size of your input
#define MAX_LINES 500
#define MAX_LINE_LENGTH 50
#define MAX(x, y) ((x > y) ? x : y)

struct TypeOfRock
{
	long long int type;
	long long int numberOfRocks;
};

int
readFileAsStrings(const char *fileName, char *lines[])
{
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
		maxLength = MAX(maxLength, strlen(lines[count]));
		count++;
	}
	fclose(file);
	printf("Number of lines read: %llu \n", count);
	printf("Longest line read: %llu \n", maxLength);
	return count; // Return the number of lines read
}

int
computeNumberOfDigits(long long int num)
{
	int numberOfDigits = 1;
	while (num > 9)
	{
		num /= 10;
		numberOfDigits++;
	}
	return numberOfDigits;
}

int 
findRock(struct TypeOfRock *types, int numberOfTypes, struct TypeOfRock type)
{
	for (int i = 0; i < numberOfTypes; i++)
	{
		if (types[i].type == type.type)
		{
			return i;
		}
	}
	return -1;
}
// Assumes number to have an even amount of digits
void
splitNumber(long long int number, long long int *firstHalf, long long int *secondHalf)
{
	long long int numberOfDigits = (int)log10(number) + 1;
	long long int halfDigits = numberOfDigits / 2;
	long long int divisor = pow(10, halfDigits);
	*firstHalf = number / divisor;
	*secondHalf = number % divisor;
}

void
update(struct TypeOfRock **types, int *endPosition, struct TypeOfRock rockType)
{
	// See if type is already present
	int pos = findRock(*types, *endPosition, rockType);
	if (pos >= 0)
	{
		(*types)[pos].numberOfRocks += rockType.numberOfRocks;
	} // otherwise we add it to the list of newly generated types and increment the end position
	else
	{
		(*types)[*endPosition] = rockType;
		(*endPosition)++;
	}
}
// returns the number of new rocks
int
blink(struct TypeOfRock **types, int numberOfTypes)
{
	/*
	 *	List that will contain the newly generated types.
	 *	If all of the types split, then we will need at least twice the size of the previous list
	 */
	struct TypeOfRock *newTypes = (struct TypeOfRock *)malloc((2 * numberOfTypes + 1) * sizeof(struct TypeOfRock));
	int numberOfNewTypes = 0; // Points to next index to be allocated
	for (int i = 0; i < numberOfTypes; i++)
	{
		struct TypeOfRock currentRockType = (*types)[i];
		int numberOfDigits = computeNumberOfDigits(currentRockType.type);
		/*
		 *	For each of the cases do the following.
		 *	Update the type. Check if the type is already present.
		 *	If this is the case add them to the already inserted.
		 *	Otherwise create new
		 */
		if (currentRockType.type == 0)
		{
			// Update the type
			struct TypeOfRock newRockType = {.type = 1, .numberOfRocks = currentRockType.numberOfRocks};
			update(&newTypes, &numberOfNewTypes, newRockType);
		}
		else if (numberOfDigits % 2 == 0)
		{
			// First create the two types:
			long long int firstType = 0;
			long long int secondType = 0;
			splitNumber(currentRockType.type, &firstType, &secondType);
			// Edge case (if the type is something like 101101 etc)
			if (firstType == secondType)
			{
				struct TypeOfRock newRockType = {.type = firstType, .numberOfRocks = 2*currentRockType.numberOfRocks};
				update(&newTypes, &numberOfNewTypes, newRockType);
			}
			else
			{
				struct TypeOfRock firstRockType = {.type = firstType, .numberOfRocks = currentRockType.numberOfRocks};
				struct TypeOfRock secondRockType = {.type = secondType, .numberOfRocks = currentRockType.numberOfRocks};
				update(&newTypes, &numberOfNewTypes, firstRockType);
				update(&newTypes, &numberOfNewTypes, secondRockType);
			}
		}
		else
		{
			long long newType = 2024*currentRockType.type;
			struct TypeOfRock newRockType = {.type = newType, .numberOfRocks = currentRockType.numberOfRocks};
			update(&newTypes, &numberOfNewTypes, newRockType);
		}
	}

	free(*types);
	*types = newTypes;
	return numberOfNewTypes;
}

int
initializeRocks(struct TypeOfRock **types, char *input)
{
	int number_of_rocks = 1;
	for (int i = 0; i < strlen(input); i++)
	{
		if (input[i] == ' ')
		{
			number_of_rocks++;
		}
	}
	// Allocate size for initial types
	*types = (struct TypeOfRock *)malloc(number_of_rocks * sizeof(struct TypeOfRock));
	if (*types == NULL)
	{
		printf("Allocation failed\n");
		return -1;
	}

	int rockPos = 0;

	char *rockType = (char *)malloc((strlen(input) + 1) * sizeof(char));
	if (rockType == NULL)
	{
		printf("Allocation failed \n");
		return -1;
	}

	int pos = 0;

	for (int i = 0; i < strlen(input); i++)
	{
		if (input[i] != ' ')
		{
			rockType[pos] = input[i];
			pos++;
		}
		else if ((input[i] == ' ') && i < strlen(input) - 1)
		{
			rockType[pos] = '\0';
			struct TypeOfRock r =
			    {
				.type = atoi(rockType), .numberOfRocks = 1};
			(*types)[rockPos] = r;
			rockPos++;
			// Reset type
			pos = 0;
		}
		if (i == strlen(input) - 1)
		{
			rockType[pos] = '\0';
			struct TypeOfRock r =
			    {
				.type = atoi(rockType), .numberOfRocks = 1};
			(*types)[rockPos] = r;
			rockPos++;
			// Reset type
			rockType = (char *)malloc(strlen(input) * sizeof(char));
			if (rockType == NULL)
			{
				printf("Allocation failed\n");
				return -1;
			}
			pos = 0;
		}
	}
	free(rockType);
	return rockPos;
}


void
printRocks(struct TypeOfRock *types, int numberOfTypes)
{
	for (int i = 0; i < numberOfTypes; i++)
	{
		printf("(Type,Number) =(%lld,%lld)\n", types[i].type, types[i].numberOfRocks);
	}
}
long long 
computeTotalNumberOfRocks(struct TypeOfRock *types, int numberOfTypes)
{
	long long totalNumber = 0;
	for( int i = 0 ; i < numberOfTypes ; i++)
	{
		totalNumber += types[i].numberOfRocks;
	}
	return totalNumber;
}
int
main()
{
	char *data[MAX_LINES];
	int rows = readFileAsStrings("input.txt", data);
	struct TypeOfRock *types;
	int numberOfRocks = initializeRocks(&types, *data);
	//printf("Initial configuration\n");
	//printRocks(types,numberOfRocks);
	int numberOfBlinks = 75;
	for (int i = 1; i <= numberOfBlinks; i++)
	{
		numberOfRocks = blink(&types, numberOfRocks);
		if( i == 25)
		{
			printf("%lld\n",computeTotalNumberOfRocks(types,numberOfRocks));
		}
		//printf("Blink %d\n",i);
		//printRocks(types,numberOfRocks);
	}


	printf("%lld\n",computeTotalNumberOfRocks(types,numberOfRocks));
	free(types);

	return 0;
}