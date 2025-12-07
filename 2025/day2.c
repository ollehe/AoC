#include <regex.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
	uint64_t lower;
	uint64_t upper;
} Range;

static uint64_t MAX_LINE = 2048;

static int parseFile(const char *filename, Range *ranges)
{
	int numberOfRanges = 0;
	FILE *file = fopen(filename, "r");
	if (!file) {
		return -1;
	}

	char line[MAX_LINE];
	char *result = fgets(line, sizeof(line), file);
	if (result == NULL) {
		fclose(file);
		return -1;
	}
	char *ptr = line;
	// Find all pairs of integers
	while (sscanf(ptr, "%zu-%zu", &ranges->lower, &ranges->upper) == 2) {
		// Find next comma
		char *nextComma = strchr(ptr, ',');
		// If no comma found we are at the end
		if (!nextComma) {
			break;
		}
		// Advance pointer to next integer
		ptr = nextComma + 1;
		// Increment range pointer
		ranges++;
		numberOfRanges++;
	}
	// Parse last range
	sscanf(ptr, "%zu-%zu", &ranges->lower, &ranges->upper);
	numberOfRanges++;
	fclose(file);
	return numberOfRanges;
}

static uint64_t numberOfDigits(uint64_t number)
{
	uint64_t exponent = 0;

	while (number >= 10) {
		number /= 10;
		exponent++;
	}
	// Add one to account for 1-9
	return exponent + 1;
}

static uint64_t pow10(uint64_t exponent)
{
	uint64_t result = 1;
	for (uint64_t i = 0; i < exponent; i++) {
		result *= 10;
	}
	return result;
}

static bool is_repeated_pattern(uint64_t number)
{
	uint64_t digits = numberOfDigits(number);

	// For each possible pattern length
	for (uint64_t ii = 1; ii <= digits / 2; ii++) {
		if (digits % ii != 0) {
			continue;
		}

		uint64_t factor = pow10(ii);
		uint64_t potential_subpattern = number % factor;
		uint64_t remainder = number;
		bool is_repeated = true;
		// If integer is divisible by pattern length

		for (uint64_t jj = 0; jj < digits / ii; jj++) {
			if (remainder % factor != potential_subpattern) {
				is_repeated = false;
				break;
			}
			remainder /= factor;
		}
		if (is_repeated) {
			return true;
		}
	}
	return false;
}

static bool is_repeated_twice(uint64_t number)
{
	uint64_t digits = numberOfDigits(number);
	uint64_t split = digits / 2;

	if (digits % 2 == 0) {
		split = digits / 2;
		uint64_t left = number / pow10(split);
		uint64_t right = number % pow10(split);
		if (left == right) {
			return true;
		}
	}
	return false;
}

static uint64_t check_ranges(Range *ranges, int numberOfRanges, bool part2)
{
	uint64_t sum = 0;
	for (int ii = 0; ii < numberOfRanges; ii++) {
		for (uint64_t jj = ranges[ii].lower; jj <= ranges[ii].upper;
		     jj++) {
			if (!part2) {
				if (is_repeated_twice(jj)) {
					sum += jj;
				}
			} else {
				if (is_repeated_pattern(jj)) {
					sum += jj;
				}
			}
		}
	}
	return sum;
}

int main()
{
	Range ranges[MAX_LINE];
	int numberOfRanges = parseFile("input.txt", ranges);
	uint64_t sum = check_ranges(ranges, numberOfRanges, false);
	printf("Part 1:%lu\n", sum);
	sum = check_ranges(ranges, numberOfRanges, true);
	printf("Part 2:%lu\n", sum);

	return 0;
}
