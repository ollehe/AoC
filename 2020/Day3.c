#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Change this according to the size of your input
#define MAX_LINES 500
#define MAX_LINE_LENGTH 50
#define MAX(x,y) ( (x > y) ? x : y )

int read_file_as_strings(const char *filename, char *lines[]) {
    FILE *file = fopen(filename, "r");
    uint max_length=0;
    if (file == NULL) {
        perror("Error opening file");
        return -1;  // Return an error code if the file cannot be opened
    }
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL && count < MAX_LINES) {
        lines[count] = malloc(strlen(buffer) + 1);  // Allocate memory for each line
        if (lines[count] == NULL) {
            perror("Error allocating memory");
            fclose(file);
            return -1;
        }

        strcpy(lines[count], buffer);  // Copy the line into the array
        max_length = MAX(max_length,strlen(lines[count]));
        count++;
    }
    fclose(file);
    printf("Number of lines read: %d \n", count);
    printf("Longest line read: %d \n", max_length);
    return count;  // Return the number of lines read
}

int countTrees(char* grid[], int rows, int cols, int row_shift, int col_shift){
    int number_of_trees = 0;
    char tree = '#';
    int row = 0;
    int col = 0;
    while( row < rows  ){
        number_of_trees = number_of_trees+(grid[row][col]==tree);
        row = row + row_shift;
        col = (col + col_shift)%(cols-1);
    }
    return number_of_trees;

}

int main(){
    char* grid[MAX_LINES];
    int rows = read_file_as_strings("Day3.txt", grid);
    int cols = strlen(grid[0]);
    // Part 1
    printf("Number of  trees encountered: %d\n", countTrees(grid, rows,cols,1,3) );
    // Part 2
    printf("Product of number of  trees encountered: %ld\n"
        , (long) countTrees(grid, rows,cols,1,1)*countTrees(grid, rows,cols,1,3)*countTrees(grid, rows,cols,1,5)
        *countTrees(grid, rows,cols,1,7)*countTrees(grid, rows,cols,2,1) 
        );
    
}