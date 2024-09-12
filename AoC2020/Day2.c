#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Change this according to the size of your input
#define MAX_LINES 2000
#define MAX_LINE_LENGTH 50
#define MAX(x,y) ( (x > y) ? x : y )
struct Policy {
    int min;
    int max;
    char specifier;
    char* password;
};
int read_file_as_strings(const char *filename, char *lines[], int max_lines) {
    FILE *file = fopen(filename, "r");
    uint max_length;
    if (file == NULL) {
        perror("Error opening file");
        return -1;  // Return an error code if the file cannot be opened
    }
    char buffer[MAX_LINE_LENGTH];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL && count < max_lines) {
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
void remove_spaces(char* str) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (str[i] != ' ') {
            str[j] = str[i];
            j++;
        }
        i++;
    }
    str[j] = '\0';  // Null-terminate the string
}

void parsePolicies(char* lines[], int number_of_lines, struct Policy policies[]){
    for( int i = 0 ; i < number_of_lines ; i ++ ){
        //printf("%s", lines[i]);
        //printf(" %d \n", (int) strlen(lines[i]));
        remove_spaces(lines[i]);


        char* min = strtok(lines[i],"-:");
        char* max_and_spec = strtok(NULL,"-:");
        char* password = strtok(NULL,"-:");

        policies[i].min = atoi(min);
        policies[i].max = atoi(max_and_spec);
        policies[i].specifier = max_and_spec[strlen(max_and_spec)-1];
        policies[i].password = password;
        
    } 
}

void printPolicy( struct Policy policy) {
    printf(
        "Interval: [%d,%d]. Specifier: %c. Password: %s ", 
        policy.min,policy.max,policy.specifier, policy.password 
    );
}

int countValidPasswords(struct Policy policies[], int number_of_policies){
    int number_of_valid_passwords=0;
    for( int i = 0 ; i < number_of_policies ; i++) {
        /*
        int number_of_occurrences = 0;
        while( *policies[i].password != '\0' ){
            if( *policies[i].password == policies[i].specifier ){
                number_of_occurrences++;
            }
            policies[i].password++;
        }
        if( number_of_occurrences>= policies[i].min && number_of_occurrences <= policies[i].max){
            number_of_valid_passwords++;
        }
        */
        int first = policies[i].min;
        int second = policies[i].max;
        int occurences = (policies[i].password[first-1] == policies[i].specifier)
            +(policies[i].password[second-1] == policies[i].specifier);
        
        if( occurences ==1 ) {
            printPolicy(policies[i]);
            number_of_valid_passwords++;
        }
    }

    return number_of_valid_passwords;
}
int main(){
    char* lines[MAX_LINES];
    int count = read_file_as_strings("Day2.txt", lines, MAX_LINES);
    struct Policy policies[count]; 
    parsePolicies(lines,count, policies);
    printf("Number of valid policies: %d \n", countValidPasswords(policies,count));
}