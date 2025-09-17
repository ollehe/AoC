#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
// Change this according to the size of your input
#define MAX_LINES 2000
#define MAX_LINE_LENGTH 50
#define MAX(x,y) ( (x > y) ? x : y )


int read_file_as_strings(const char *filename, char *lines[]) {
    FILE *file = fopen(filename, "r");
    int max_length=0;
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
/*
Condenses an interval according a string consisting of a fixed set of rules.
Assumes the string instructions only contains lower and upper chars 
*/
int condense(
    int min, int max, char lower, char upper, 
    char* instructions, int first_instruction, int last_instruction) {
    for(int i = first_instruction ; i < last_instruction ; i++) {
        int mid_point = (min+max)/2;
        // take lower part of interval 
        if( instructions[i] == lower ) {
            max = mid_point;
        }
        // else we take the upper half
        else {
            min = mid_point+1;
        }
    }
    //last instruction determines the value 
    if( instructions[last_instruction] ==lower){
        return min;
    } else {
        return max;
    }
}
/*
Determines the seat id according to the formula 8*row+column 
*/
int determine_seat_id(char *boarding_pass) {
    if( strlen(boarding_pass) != 11) {
        printf("Not correctly formated, length is %d.\n",(int) strlen(boarding_pass));
        return -1;
    }
    return condense(0,127,'F','B',boarding_pass,0,6)*8+condense(0,7,'L','R',
            boarding_pass,7,strlen(boarding_pass)-1);

}
//needed for qsort algorithm
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}


int main(){
    char* boarding_passes[MAX_LINES];
    int number_of_lines = read_file_as_strings("Day5.txt", boarding_passes);
    int max=0;
    int seat_ids[number_of_lines];
    for(int i = 0 ; i<number_of_lines ; i ++) {
        seat_ids[i] = determine_seat_id(boarding_passes[i]);
        printf("%d\n",seat_ids[i]);
        // this is part 1
        max = MAX(max, determine_seat_id(boarding_passes[i]));
    }
    // this is for part 2
    int n = sizeof(seat_ids) / sizeof(seat_ids[0]);
    qsort(seat_ids,n,sizeof(int),compare);
    for(int i = 0 ; i< number_of_lines -1 ; i++){
        if(seat_ids[i+1]-seat_ids[i] == 2) {
            printf("%d\n",seat_ids[i]+1);
        }
    }
    printf("%d\n",max);
}