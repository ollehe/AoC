#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// Define constants for maximum lines and line length
#define MAX_LINES 100000
#define MAX_LINE_LENGTH 100
#define NUMBER_OF_BITS 36

#define MAX(x, y) ((x > y) ? x : y)  // Macro to find the maximum of two values

// Structure to represent the memory
struct Memory {
	long long position;  // position of the memory
	char binary_value[NUMBER_OF_BITS];
	long long decimal_value;
};

// Function to read lines from a file into an array of strings
int read_file_as_strings(const char *filename, char *lines[]) {
	FILE *file = fopen(filename, "r");
	int max_length = 0;  // Track the length of the longest line
	if (file == NULL) {
		perror("Error opening file");
		return -1;  // Return an error code if the file cannot be opened
	}
	char buffer[MAX_LINE_LENGTH];
	int count = 0;
	// Read lines from the file until reaching EOF or maximum lines
	while (fgets(buffer, sizeof(buffer), file) != NULL && count < MAX_LINES) {
		lines[count] =
		    malloc(strlen(buffer) + 1);  // Allocate memory for each line
		if (lines[count] == NULL) {
			perror("Error allocating memory");
			fclose(file);
			return -1;
		}
		strcpy(lines[count], buffer);  // Copy the line into the array
		max_length = MAX(max_length, strlen(lines[count]));
		count++;
	}
	fclose(file);
	printf("Number of lines read: %d \n", count);
	printf("Longest line read: %d \n", max_length);
	return count;  // Return the number of lines read
}

// Helper function, removes the new line character from a string '\n'
void strip(char *string) {
	if (string[strlen(string) - 1] == '\n') {
		string[strlen(string) - 1] = '\0';
	}
}
// Helper function, given a string of the form "blbala = number"
// it moves the pointer to pointing to the first digit
char *get_value(char *string) {
	while (*string != '=') {
		string++;
	}
	string++;
	string++;
	return string;
}
void print_binary(char binary[]) {
	for (int i = 0; i < NUMBER_OF_BITS; i++) {
		printf("%c", binary[i]);
	}
}
// Both arrays will always be of the same size (NUMBER_OF_BITS)
void copy_array(char target[], char source[]) {
	for (int i = 0; i < NUMBER_OF_BITS; i++) {
		target[i] = source[i];
	}
}

// Check if the memory position is already added.
// Returns the position of it is added
int is_added(struct Memory mems[], int number_of_mems,
             long long memory_position) {
	for (int i = 0; i < number_of_mems; i++) {
		if (mems[i].position == memory_position) {
			return i;
		}
	}
	return -1;
}
// Applies the bitmask
void apply_bitmask(char binary[], char bitmask[], int part2) {
	if (!part2) {
		for (int i = NUMBER_OF_BITS - 1; i >= 0; i--) {
			if (bitmask[i] != 'X') {
				binary[i] = bitmask[i];
			}
		}
	} else {
		// Part 2
		for (int i = NUMBER_OF_BITS - 1; i >= 0; i--) {
			if (bitmask[i] == 'X' || bitmask[i] == '1') {
				binary[i] = bitmask[i];
			}
		}
	}
}

void compute_binary_representation(char binary[], int value) {
	for (int i = NUMBER_OF_BITS - 1; i >= 0; i--) {
		binary[i] = (value % 2 == 0) ? '0' : '1';
		value = value / 2;
	}
}

int compute_number_of_x(char binary[]) {
	int nbr = 0;
	for (int i = 0; i < NUMBER_OF_BITS; i++) {
		if (binary[i] == 'X') {
			nbr++;
		}
	}
	return nbr;
}

long long compute_decimal(char binary[]) {
	long long decimal = 0;
	for (int i = 0; i < NUMBER_OF_BITS; i++) {
		// Use bitwise shift for computing powers of 2
		if (binary[i] == '1') {
			decimal += (1LL << (NUMBER_OF_BITS - 1 - i));
		}
	}
	return decimal;
}

// Given a string containing 0,1,X this method returns
void compute_memory_addresses(long long addresses[], int number_of_x,
                              char *bitstring, int *current_pos) {
	if (number_of_x == 0) {
		long long val = compute_decimal(bitstring);
		addresses[*current_pos] = val;  // addresses = &val;
		(*current_pos)++;               // addresses++;
		return;
	}
	for (int i = 0; i < strlen(bitstring); i++) {
		if (bitstring[i] == 'X') {
			char one[NUMBER_OF_BITS];
			char zero[NUMBER_OF_BITS];
			strcpy(one, bitstring);
			strcpy(zero, bitstring);
			one[i] = '1';
			zero[i] = '0';
			compute_memory_addresses(addresses, number_of_x - 1, one,
			                         current_pos);
			compute_memory_addresses(addresses, number_of_x - 1, zero,
			                         current_pos);
			return;
		}
	}
}

void add_part1(struct Memory mems[], char *bitmask, int memory_pos,
               long long decimal_value, int *count) {
	int pos = is_added(mems, *count, memory_pos);
	if (pos < 0) {
		struct Memory mem;
		mem.position = memory_pos;
		mem.decimal_value = decimal_value;
		char binary[NUMBER_OF_BITS];
		// In part 1 we compute the binary
		// representation of the decimal value

		compute_binary_representation(binary, decimal_value);
		apply_bitmask(binary, bitmask, 0);
		copy_array(mem.binary_value, binary);
		mems[(*count)++] = mem;

	} else {
		mems[pos].decimal_value = decimal_value;
		char binary[NUMBER_OF_BITS];
		compute_binary_representation(binary, decimal_value);
		apply_bitmask(binary, bitmask, 0);
	}
}

void add_part2(struct Memory mems[], char *bitmask, int memory_pos,
               long long decimal_value, int *count) {
	// In part 2 we compute the binary
	// representation of the memory position
	char binary[NUMBER_OF_BITS];
	compute_binary_representation(binary, memory_pos);
	// Apply the bitmask with flag 1.
	apply_bitmask(binary, bitmask, 1);
	int number_of_x = compute_number_of_x(binary);
	int number_of_addresses = (1 << number_of_x);
	// Find the addresses generated by the binary
	long long addresses[number_of_addresses];
	int pos = 0;
	compute_memory_addresses(addresses, number_of_x, binary, &pos);
	for (int i = 0; i < number_of_addresses; i++) {
		pos = is_added(mems, *count, addresses[i]);

		if (pos < 0) {
			mems[*count].decimal_value = decimal_value;
			mems[*count].position = addresses[i];
			(*count)++;
		} else {
			mems[pos].decimal_value = decimal_value;
		}
	}
}

/*
This is the main function
*/
void parse_instructions(char *lines[], int length, int part2) {
	char *mask_substr = "mask = ";
	char *mem_substr = "mem[";
	struct Memory mems[MAX_LINES];
	int count = 0;
	for (int i = 0; i < length; i++) {
		char *mask_is_present = strstr(lines[i], mask_substr);
		char bitmask[50];
		// Check if the string contains the substring "mask = "
		if (mask_is_present != NULL && i < length) {
			// remove the substring "mask = " by moving the pointer
			lines[i] = lines[i] + strlen(mask_substr);
			// remove the trailing '\n'
			strip(lines[i]);
			strcpy(bitmask, lines[i]);
			i++;
			char *mem_is_present = strstr(lines[i], mem_substr);
			while (mem_is_present != NULL && i < length) {
				strip(lines[i]);
				// Remove the substring "mem[" by moving the pointer
				lines[i] = lines[i] + strlen(mem_substr);
				int memory_pos = atoi(lines[i]);
				long long decimal_value = atoll(get_value(lines[i]));
				// Add the value

				if (!part2) {
					add_part1(mems, bitmask, memory_pos, decimal_value, &count);
				} else {
					add_part2(mems, bitmask, memory_pos, decimal_value, &count);
				}
				// increment
				i++;
				// last line error handling
				if (i < length) {
					mem_is_present = strstr(lines[i], mem_substr);
				}
			}
			i--;
		}
	}
	// This is the end part that tallys all the decimal values
	long long total = 0;
	if (!part2) {
		for (int i = 0; i < count; i++) {
			print_binary(mems[i].binary_value);
			long long dec = compute_decimal(mems[i].binary_value);
			total += dec;
		}
		printf("Part 1: %lld \n", total);
		total = 0;
	} else {
		for (int i = 0; i < count; i++) {
			total += mems[i].decimal_value;
		}
		printf("Part 2: %lld \n", total);
	}
}

int main() {
	char *lines[MAX_LINES];
	int number_of_lines = read_file_as_strings(
	    "input.txt", lines);  // Read instructions from a file
	parse_instructions(lines, number_of_lines, 1);
	return 0;
}
