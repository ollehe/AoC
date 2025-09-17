#include <stdio.h>
#include <stdlib.h>

#define MAX_LINES 200
#define MAX_LINE_LENGTH 10

int read_file_as_integers(const char *filename, int *numbers, int max_lines) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;  // Return an error code if the file cannot be opened
    }

    char buffer[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL && count < max_lines) {
        numbers[count] = atoi(buffer);  // Convert the line to an integer and store it in the array
        count++;
    }

    fclose(file);
    return count;  // Return the number of lines read
}
/* Part 1 : 
 Find the two entries that sum to 2020 and then multiply those two numbers together
 */
int find_product_of_pair(const int* numbers, int count){
    int product=0;
    for( int i = 0 ; i< count ; i++ ) {
        for( int j =i+1 ; j <count ; j++ ) {
            if( numbers[i]+numbers[j] == 2020) {
                product = numbers[i]*numbers[j];
            }
        }
    }
    return product;
}
/*Part 2 
What is the product of the three entries that sum to 2020
 */ 
int find_product_of_triple(const int* numbers, int count){
    int product=0;
    for( int i = 0 ; i< count ; i++ ) {
        for( int j =i+1 ; j <count ; j++ ) {
            for ( int k = i+2 ; k<count ; k++) {
                if( numbers[i]+numbers[j] +numbers[k]== 2020) {
                    product = numbers[i]*numbers[j]*numbers[k];
                }
            }

        }
    }
    return product;
}
int main() {
    const char* file_name = "Day1.txt";
    int numbers[MAX_LINES];
    int count = read_file_as_integers(file_name, numbers, MAX_LINES);
    int pair_product = find_product_of_pair(numbers,count);
    int triple_product = find_product_of_triple(numbers,count);
    printf("The pair product is: %d and the triple product is %d\n", pair_product,triple_product);
    return 0;
}