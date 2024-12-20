#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>



#define MAX_LINES 1000
#define MAX_LINE_LENGTH 50
#define MAX(x, y) ((x > y) ? x : y)

enum
{
	xDim = 101,
	yDim = 103,
} dimension;

typedef struct
{
	int pos_x;
	int pos_y;
	int vel_x;
	int vel_y;
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
		maxLength = MAX(maxLength, strlen(lines[count]));
		count++;
	}
	fclose(file);
	printf("Number of lines read: %llu \n", count);
	printf("Longest line read: %llu \n", maxLength);
	return count; // Return the number of lines read
}

void
printRobots(Robot *robots, int numberOfRobots)
{

	if (robots == NULL)
	{
		printf("WTF");
	}
	int test =
	    robots[0].pos_x;
	printf("First robot: %d,%d,%d,%d\n", robots[0].pos_x, robots[0].pos_y, robots[0].vel_x, robots[0].vel_y);

	printf("Printing robots\n");
	for (int i = 0; i < numberOfRobots; i++)
	{
		printf("Robot %d: Pos = (%d,%d), Vel = (%d,%d)\n", i, robots[i].pos_x, robots[i].pos_y, robots[i].vel_x, robots[i].vel_y);
	}
}

void
initiateRobots(Robot **robots, char **input, int numberOfRobots)
{

	/*
	 *	Set up regex
	 */
	regex_t regex;
	regmatch_t match;
	const char *pattern = "[-]?[[:digit:]]+";
	/*
	 *	Compile and hope it works
	 */
	if (regcomp(&regex, pattern, REG_EXTENDED))
	{
		printf("Compilation failed");
		return;
	}

	/*
	 *	Parse the data
	 */
	for (size_t i = 0; i < numberOfRobots; i++)
	{
		int robotData[4];
		int pos = 0;
		while (regexec(&regex, input[i], 1, &match, 0) == 0)
		{
			char *integer = malloc((match.rm_eo - match.rm_so + 1) * sizeof(char));
			for (int j = match.rm_so; j < match.rm_eo; j++)
			{
				integer[j - match.rm_so] = input[i][j];
			}
			integer[match.rm_eo - match.rm_so] = '\0';
			robotData[pos] = atoi(integer);
			input[i] += match.rm_eo; // THis is fine I don't care about changing the input
			pos++;
			free(integer);
		}
		Robot newRobot = {.pos_x = robotData[0], .pos_y = robotData[1], .vel_x = robotData[2], .vel_y = robotData[3]};
		(*robots)[i] = newRobot;
	}
	regfree(&regex);
}

void
move(Robot *r, int numberOfSeconds)
{
	r->pos_x += r->vel_x * numberOfSeconds;
	r->pos_y += r->vel_y * numberOfSeconds;
	r->pos_x %= xDim;
	r->pos_y %= yDim;

	if (r->pos_x < 0)
	{
		r->pos_x += xDim;
	}
	if (r->pos_y < 0)
	{
		r->pos_y += yDim;
	}
}

void
draw(Robot *robots, int numberOfRobots)
{
	for (int y = 0; y < yDim; y++)
	{
		for (int x = 0; x < xDim; x++)
		{
			int counter = 0;
			for (int i = 0; i < numberOfRobots; i++)
			{
				if (robots[i].pos_x == x && robots[i].pos_y == y)
				{
					counter++;
				}
			}
			if (counter > 0)
			{
				//putchar('*');
				printf("%d",counter);
			}
			else
			{
				putchar('.');
			}
		}
		putchar('\n');
	}
}
void
simulate(Robot **robots, int numberOfRobots, int numberOfSeconds)
{
	for (int j = 0; j < numberOfRobots; j++)
	{
		move(&(*robots)[j], numberOfSeconds);
	}
}

int
count(Robot *robots, int numberOfRobots)
{
	int quadrants[4] = {0};
	for (int i = 0; i < numberOfRobots; i++)
	{
		int x = robots[i].pos_x;
		int y = robots[i].pos_y;
		/*
		 *	Determine which quadrant we are in
		 */
		if ((x < xDim / 2) && (y < yDim / 2))
		{
			quadrants[0]++;
		}
		else if ((x > xDim / 2) && (y < yDim / 2))
		{
			quadrants[1]++;
		}
		else if ((x < xDim / 2) && (y > yDim / 2))
		{
			quadrants[2]++;
		}
		else if ((x > xDim / 2) && (y > yDim / 2))
		{
			quadrants[3]++;
		}
	}

	return quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
}

int
main()
{
	char *data[MAX_LINES];
	int numberOfRobots = readFileAsStrings("input.txt", data);
	Robot *robots = (Robot *)malloc(numberOfRobots * sizeof(Robot));
	if (robots == NULL)
	{
		printf("Failed to allocate memory for robots.\n");
		return 1;
	}
	printf("Number of robots: %d\n", numberOfRobots);
	initiateRobots(&robots, data, numberOfRobots);
	bool part2 = true;

	if (!part2)
	{
		// part 1
		simulate(&robots, numberOfRobots, 100);
		printf("%d", count(robots, numberOfRobots));
	}
	else
	{
		/* 
		 * Run the program for 103 times see where you get the two pieces that should form the tree (one thin and one wide)
		 * Note down the number of steps (a and b) it takes for these to appear, then use chinese remainder theorem to solve for steps % 101 = a, steps %103 =b
		 */
		simulate(&robots, numberOfRobots, 7584);
		draw(robots, numberOfRobots);
	}
	free(robots);
	return 0;
}