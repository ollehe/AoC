#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Change this according to the size of your input
#define MAX_LINES 2000
#define MAX_LINE_LENGTH 50
#define MAX(x,y) ( (x > y) ? x : y )
struct Instruction{
    int is_executed; 
    char* type;
    int value;
};
void print_instruction(struct Instruction instruction){
    printf("%s %d\n",instruction.type,instruction.value);
}

int read_file_as_strings(const char *filename, char *lines[], int max_lines) {
    FILE *file = fopen(filename, "r");
    int max_length;
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

void parse_instructions(char* lines[], struct Instruction instructions[], int nbr_of_lines) {
    for( int i = 0 ; i < nbr_of_lines ; i++) {
        char* type = strtok(lines[i]," ");
        int value = atoi(strtok(NULL," "));
        instructions[i].type = type; 
        instructions[i].is_executed = 0;
        instructions[i].value = value;   
    }
}

int value_of_accumulator(struct Instruction instructions[],int nbr_of_instructions){
    int accumulator =0;
    int i = 0 ; 
    while(i<nbr_of_instructions ) {
        if( instructions[i].is_executed){
            // printf("We've entered an infinite loop! Accumulator value: %d\n",accumulator);
            return accumulator;
        }else {
            instructions[i].is_executed=1;
            //strcmp returns 0 if two strings are equal (which explains the !)
            if( !strcmp( instructions[i].type , "acc")){
                accumulator+=instructions[i].value;
                i++;
            } else if(!strcmp( instructions[i].type ,"jmp")) {
                i+=instructions[i].value;
            } else {
                i++;
            }
        }
    }
    printf("We excited the loop! Accumulator value: %d\n",accumulator);
    return accumulator;
}

void reset_instructions(struct Instruction instructions[],int nbr_of_instructions){
    for( int i = 0 ; i<nbr_of_instructions ; i++){
        instructions[i].is_executed=0;
    }
}

void fix_the_program(struct Instruction instructions[],int nbr_of_instructions){
    printf("Fixing the program\n");
    for( int i = 0 ; i < nbr_of_instructions ; i++){
        //Change the "nop" to jmp or change the jmp to nop
        if(!strcmp(instructions[i].type,"nop") ){
            instructions[i].type ="jmp";
            value_of_accumulator(instructions, nbr_of_instructions);
            instructions[i].type = "nop";
            // reset the is_executed logic
            reset_instructions(instructions,nbr_of_instructions); 
        } else if( !strcmp(instructions[i].type,"jmp")){
            instructions[i].type ="nop";
            value_of_accumulator(instructions, nbr_of_instructions);
            instructions[i].type = "jmp";
            // reset the is_executed logic
            reset_instructions(instructions,nbr_of_instructions);
        }
    }
}

int main(){
    char* lines[MAX_LINES];
    int count = read_file_as_strings("Day8.txt", lines, MAX_LINES);
    struct Instruction instructions[count];
    parse_instructions(lines, instructions, count);
    printf("value of accumulator is %d\n",value_of_accumulator(instructions,count));
    fix_the_program(instructions,count);
}