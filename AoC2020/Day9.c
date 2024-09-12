#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
// Change this according to the size of your input
#define MAX_LINES 2000
#define MAX_LINE_LENGTH 50
#define MAX(x,y) ( (x > y) ? x : y )
#define MIN(x,y) ( (x > y) ? y : x )

int read_file_as_integers(const char *filename, int *numbers, int max_lines) {
    FILE *file = fopen(filename, "r");
    int max_length=0;

    if (file == NULL) {
        perror("Error opening file");
        return -1;  // Return an error code if the file cannot be opened
    }

    char buffer[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL && count < max_lines) {
        numbers[count] = atoi(buffer);  // Convert the line to an integer and store it in the array
        max_length = MAX(max_length,strlen(buffer));
        count++;
    }

    fclose(file);
    printf("Number of lines read: %d \n", count);
    printf("Longest line read: %d \n", max_length);

    return count;  // Return the number of lines read
}

int is_sum(int number_to_check, int numbers[], int first, int last) {
    for ( int i = first ; i < last ; i++) {
        for( int j = i+1 ; j < last ; j ++) {
            if( number_to_check == numbers[i]+numbers[j]){
                return 1;
            }
        }
    }
    return 0;
}
int partial_sum(int numbers[],int first, int last) {
    int sum = 0;
    for (int i = first; i <= last ; i++ ) {
        sum += numbers[i];
    }
    return sum;
}
int decypher(int numbers[], int length, int preamble_length){
    int not_valid;
    //First we find the non-valid number
    for(int i = preamble_length ; i < length ; i++) {
        if( ! is_sum(numbers[i], numbers, i-preamble_length,i) ){
            printf("Not a valid number: %d\n", numbers[i]);
            not_valid = numbers[i];
            break;
        }
    }

    int first=0;
    int last=1; 
    int current_sum = partial_sum(numbers,first,last);
    // Then we find the contiguous set where 
    // first and last are the first and last index of this set.
    while( current_sum != not_valid  ){
        if( current_sum < not_valid ) {
            last+=1;
        }else if( current_sum>not_valid ) {
            first +=1;
        }
        current_sum = partial_sum(numbers,first,last);
    }
    int min = INT_MAX;
    int max = 0;
    for( int i = first ; i <= last ; i++) {
        min = MIN(numbers[i],min);
        max = MAX(numbers[i],max);
    }
    printf("Smallest number in set: %d\n Largest number in set : %d\n", min,max);
    printf("Their sum is : %d\n", min+max);
}

int main() {
    const char* file_name = "input.txt";
    int numbers[MAX_LINES];
    int count = read_file_as_integers(file_name, numbers, MAX_LINES);
    decypher(numbers,count,25);
    return 0;
}