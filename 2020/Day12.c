#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// Define constants for maximum lines and line length
#define MAX_LINES 2000
#define MAX_LINE_LENGTH 50
#define MAX(x, y) ((x > y) ? x : y)  // Macro to find the maximum of two values

// Structure to represent the Ferry with its position and direction
struct Ferry {
    int x_pos;   // Current x position
    int y_pos;   // Current y position
    int x_dir;   // Direction along the x-axis (e.g., east or west)
    int y_dir;   // Direction along the y-axis (e.g., north or south)
};

// Function to read lines from a file into an array of strings
int read_file_as_strings(const char *filename, char *lines[]) {
    FILE *file = fopen(filename, "r");
    int max_length = 0;  // Track the length of the longest line
    if (file == NULL) {
        perror("Error opening file");
        return -1; // Return an error code if the file cannot be opened
    }
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    // Read lines from the file until reaching EOF or maximum lines
    while (fgets(buffer, sizeof(buffer), file) != NULL && count < MAX_LINES) {
        lines[count] = malloc(strlen(buffer) + 1);  // Allocate memory for each line
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

// Function to print the current state of the Ferry
void print_ferry(struct Ferry ferry) {
    printf("Position (%d,%d) : Waypoint/Direction  (%d,%d)\n", ferry.x_pos, ferry.y_pos, ferry.x_dir, ferry.y_dir);
}

// Function to calculate the Manhattan distance from the origin
int distance(struct Ferry *ferry) {
    return abs(ferry->x_pos) + abs(ferry->y_pos);
}

// Function to rotate the ferry's direction by 90 degrees clockwise or counterclockwise
void rotate(struct Ferry *ferry, char orientation, int part_2) {
    int tmp = ferry->x_dir;
    if (orientation == 'R') {
        // Rotate 90 degrees clockwise (to the right)
        ferry->x_dir = ferry->y_dir;
        ferry->y_dir = -tmp;
    } else if (orientation == 'L') {
        // Rotate 90 degrees counterclockwise (to the left)
        ferry->x_dir = -ferry->y_dir;
        ferry->y_dir = tmp;
    }
}

// Function to move the ferry in a specified direction by a given value
void move(struct Ferry *ferry, char direction, int value, int part_2) {
    if (part_2) {
        // Move the waypoint in the given direction
        if (direction == 'N') {
            ferry->y_dir += value;
        } else if (direction == 'S') {
            ferry->y_dir -= value;
        } else if (direction == 'E') {
            ferry->x_dir += value;
        } else if (direction == 'W') {
            ferry->x_dir -= value;
        } else if (direction == 'F') {
            // Move the ferry towards the waypoint
            printf("X position %d\n", ferry->x_pos);
            ferry->x_pos += ferry->x_dir * value;
            printf("X position %d\n", ferry->x_pos);
            ferry->y_pos += ferry->y_dir * value;
        }
    } else {
        // Move the ferry in the specified direction
        if (direction == 'N') {
            ferry->y_pos += value;
        } else if (direction == 'S') {
            ferry->y_pos -= value;
        } else if (direction == 'E') {
            ferry->x_pos += value;
        } else if (direction == 'W') {
            ferry->x_pos -= value;
        } else if (direction == 'F') {
            ferry->x_pos += ferry->x_dir * value;
            ferry->y_pos += ferry->y_dir * value;
        }
    }
}

// Function to process a list of navigation instructions and move the ferry accordingly
void travel(char *instructions[], int number_of_instructions, int part_2) {
    // Initialize the ferry's starting position and direction (facing east)
    struct Ferry ferry = {
        .x_pos = 0,
        .y_pos = 0,
        .x_dir = 1,
        .y_dir = 0
    };
    if (part_2) {
        // Set initial waypoint position relative to the ferry
        ferry.x_dir = 10;
        ferry.y_dir = 1;
    }
    print_ferry(ferry);  // Print initial state of the ferry

    // Loop through each instruction and perform the corresponding action
    for (int i = 0; i < number_of_instructions; i++) {
        char type_of_instruction = *instructions[i];  // Get the instruction type (e.g., 'N', 'S', 'E', 'W', 'L', 'R', 'F')
        instructions[i]++;  // Move pointer to the numeric part of the instruction
        int value = atoi(instructions[i]);  // Convert the numeric part to an integer

        if (type_of_instruction == 'L' || type_of_instruction == 'R') {
            // Rotate the ferry or waypoint as specified by the instruction
            int number_of_rotations = value / 90;  // Calculate the number of 90-degree rotations needed
            for (int rotation = 0; rotation < number_of_rotations; rotation++) {
                rotate(&ferry, type_of_instruction, part_2);
            }
        } else {
            // Move the ferry or waypoint in the specified direction
            move(&ferry, type_of_instruction, value, part_2);
        }
        print_ferry(ferry);  // Print the ferry's state after each instruction
    }
    // Print the final Manhattan distance from the origin
    printf("Distance to the origin: %d\n", distance(&ferry));
}

int main() {
    char *instructions[MAX_LINES];
    int number_of_lines = read_file_as_strings("input.txt", instructions);  // Read instructions from a file
    travel(instructions, number_of_lines, 1);  // Start the travel based on the instructions
    return 0;
}
