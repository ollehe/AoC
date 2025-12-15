#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const uint64_t MAX_LINES = 4096;
static const uint64_t MAX_LINE_LENGTH = 4096;

typedef struct {
	uint64_t *numbers;
	uint64_t numberOfNumbers;
	char operand;
} Problem;

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

static uint64_t countNumberOfProblems(char *row)
{
	//Literally just grabbed from strol doc at cppref
	uint64_t count = 0;
	const char *p = row;
	for (;;) {
		char *end;
		strtoull(p, &end, 10);
		if (end == p) {
			break;
		}
		p = end;
		count++;
	}
	return count;
}

static uint64_t parseDataRowWise(char **data, uint64_t rows, Problem **problems)
{
	uint64_t numberOfProblems = countNumberOfProblems(data[0]);
	*problems = malloc(sizeof(Problem) * numberOfProblems);
	if (*problems == NULL) {
		perror("Error allocating memory");
		exit(EXIT_FAILURE);
	}
	// Allocate memory for each problem's numbers array.
	for (uint64_t i = 0; i < numberOfProblems; i++) {
		// Last row contains operand
		(*problems)[i].numbers = malloc(sizeof(uint64_t) * (rows - 1));
		(*problems)[i].numberOfNumbers = rows - 1;
		if ((*problems)[i].numbers == NULL) {
			perror("Error allocating memory");
			exit(EXIT_FAILURE);
		}
	}
	// Read all numbers in to problems structure.
	// Skip last line as it encodes operand.
	uint64_t problemIndex;
	// Parse each row and populate the numbers array.
	for (uint64_t row = 0; row < rows - 1; row++) {
		const char *p = data[row];
		problemIndex = 0;
		char *end = NULL;
		for (uint64_t value = strtoull(p, &end, 10); p != end;
		     value = strtoull(p, &end, 10)) {
			(*problems)[problemIndex].numbers[row] = value;
			problemIndex++;
			p = end;
		}
	}
	char *operands = data[rows - 1];
	problemIndex = 0;
	while (*operands != '\0') {
		if (*operands == '*' || *operands == '+') {
			(*problems)[problemIndex].operand = *operands;
			problemIndex++;
		}
		operands++;
	}
	return numberOfProblems;
}

static bool isSeparator(char **data, uint64_t column, uint64_t rows)
{
	for (uint64_t row = 0; row < rows; row++) { // include operand row
		if (data[row][column] != ' ')
			return false;
	}
	return true;
}

static uint64_t parseColumn(char **data, uint64_t column, uint64_t rows,
			    bool *has_digit)
{
	uint64_t value = 0;
	bool seen = false;

	for (uint64_t row = 0; row < rows - 1; row++) { // skip operand row
		char ch = data[row][column];
		if (ch >= '0' && ch <= '9') {
			seen = true;
			value = value * 10 +
				(uint64_t)(ch -
					   '0'); // correct base-10 accumulation
		}
	}

	*has_digit = seen;
	return value;
}

static uint64_t parseDataColumnWise(char **data, uint64_t rows,
				    Problem **problems)
{
	uint64_t numberOfProblems = countNumberOfProblems(data[0]);

	uint64_t columns = strlen(data[0]); // only safe if all rows same width

	*problems = malloc(sizeof(Problem) * numberOfProblems);
	if (!*problems) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	for (uint64_t i = 0; i < numberOfProblems; i++) {
		(*problems)[i].numbers =
			malloc(sizeof(uint64_t) * columns); // overshoot ok
		(*problems)[i].numberOfNumbers = 0;
		(*problems)[i].operand = 0;
		if (!(*problems)[i].numbers) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
	}

	uint64_t problemIndex = 0;
	uint64_t numberIndex = 0;

	for (uint64_t col = 0; col < columns; col++) {
		if (isSeparator(data, col, rows)) {
			if (problemIndex + 1 < numberOfProblems) {
				problemIndex++;
				numberIndex = 0; // reset per problem
			}
			continue;
		}

		char op = data[rows - 1][col];
		if (op == '+' || op == '*') {
			(*problems)[problemIndex].operand = op;
		}

		bool has_digit = false;
		uint64_t v = parseColumn(data, col, rows, &has_digit);
		if (has_digit) {
			(*problems)[problemIndex].numbers[numberIndex++] = v;
			(*problems)[problemIndex].numberOfNumbers++;
		}
	}

	// optionally: validate every operand was found
	// and that you parsed the expected number of problems.

	return numberOfProblems;
}

static uint64_t evaluateExpression(Problem problem)
{
	if (problem.operand == '+') {
		uint64_t result = 0;
		for (uint64_t i = 0; i < problem.numberOfNumbers; i++) {
			result += problem.numbers[i];
		}
		return result;
	} else {
		uint64_t result = 1;
		for (uint64_t i = 0; i < problem.numberOfNumbers; i++) {
			result *= problem.numbers[i];
		}
		return result;
	}
}

static uint64_t computeGrandTotal(Problem *problems, uint64_t numberOfProblems)
{
	uint64_t grandTotal = 0;
	for (uint64_t i = 0; i < numberOfProblems; i++) {
		grandTotal += evaluateExpression(problems[i]);
	}
	return grandTotal;
}

int main(void)
{
	char *data[MAX_LINES];
	uint64_t rows = readFile("input.txt", data);

	// Part 1 (row-wise)
	Problem *problemsRowWise = NULL;
	uint64_t numberOfRows = parseDataRowWise(data, rows, &problemsRowWise);
	uint64_t part1 = computeGrandTotal(problemsRowWise, numberOfRows);
	printf("Part 1: %lu\n", part1);

	for (uint64_t i = 0; i < numberOfRows; i++) {
		free(problemsRowWise[i].numbers);
	}
	free(problemsRowWise);

	// Part 2 (column-wise)
	Problem *problemsColumnWise = NULL;
	uint64_t numberOfColumns =
		parseDataColumnWise(data, rows, &problemsColumnWise);
	uint64_t part2 = computeGrandTotal(problemsColumnWise, numberOfColumns);
	printf("Part 2: %lu\n", part2);

	for (uint64_t i = 0; i < numberOfColumns; i++) {
		free(problemsColumnWise[i].numbers);
	}
	free(problemsColumnWise);

	// Free input lines
	for (uint64_t i = 0; i < rows; i++) {
		free(data[i]);
	}

	return 0;
}
