#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROGRAM_LENGTH 16

typedef struct 
{
	uint64_t registerA;
	uint64_t registerB;
	uint64_t registerC;
} Computer;
 

void
printComputer(Computer c)
{
	printf("A: %llu B: %llu  C: %llu\n", c.registerA, c.registerB, c.registerC);
}
void
printProgram(int *program, int numberOfInstructions)
{
	for(int i = 0 ; i < numberOfInstructions ; i++)
	{
		printf("%d ",program[i]);
	}
	putchar('\n');
}
uint64_t
combo(Computer c, uint64_t operand)
{
	if (operand  <= 3 && operand >=0) 
	{
		return operand ;
	}
	else if ( operand  == 4)
	{
		return c.registerA;
	}
	else if (operand == 5)
	{
		return c.registerB;
	}
	else if ( operand == 6)
	{
		return c.registerC;
	}
	else {
		printf("Execution failed\n");
		return -1;
	}
}

void
execute(Computer *c, uint64_t opcode, uint64_t operand, uint8_t *instructionPointer, bool *hasJumped, int output[MAX_PROGRAM_LENGTH], int *currentPosition)
{
	// printf("Executing opcode %llu with operand %llu\n", opcode, operand);
	if (opcode == 0)
	{
		c->registerA = (c->registerA) / ((uint64_t)pow(2, combo(*c, operand)));
	}
	else if (opcode == 1)
	{
		c->registerB = (c->registerB) ^ operand;
	}
	else if (opcode == 2)
	{
		c->registerB = combo(*c, operand) % 8;
	}
	else if (opcode == 3)
	{
		if (c->registerA != 0)
		{
			*instructionPointer = operand;
			*hasJumped = true;
		}
	}
	else if (opcode == 4)
	{
		c->registerB = c->registerB ^ c->registerC;
	}
	else if (opcode == 5)
	{
		output[(*currentPosition)] = combo(*c,operand)%8;
		(*currentPosition)++;
	}
	else if(opcode == 6)
	{
		c->registerB = (c->registerA) / ((uint64_t)pow(2, combo(*c, operand)));
	}
	else if(opcode == 7)
	{
		c->registerC = (c->registerA) / ((uint64_t)pow(2, combo(*c, operand)));
	}
}
void
printOutput(int output[MAX_PROGRAM_LENGTH] )
{
	for(int i = 0 ; i < MAX_PROGRAM_LENGTH ; i ++)
	{
		printf("%d,", output[i]);
	}
	putchar('\n');
}


void
run(Computer *c , int* program, int numberOfInstructions, int output[MAX_PROGRAM_LENGTH])
{
	int currentPosition = 0;
	uint8_t instuctionPointer=0;
	while (instuctionPointer <  numberOfInstructions-1)
	{

		bool hasJumped = false;
		uint64_t opcode = program[instuctionPointer] ;
		uint64_t operand = program[instuctionPointer+1];

		execute(c,opcode,operand,&instuctionPointer, &hasJumped, output, &currentPosition);
		if (!hasJumped)
		{
			instuctionPointer += 2;
		}
	}
}

uint64_t
convertThreeBitsToInteger(int * program,int lengthOfProgram)
{
	uint64_t val = 0;
	for( int i = 0 ; i <lengthOfProgram ; i++)
	{
		val += (uint64_t)program[i] * (uint64_t)pow(8, i);
	}
	return val;
}


bool
areEqual(int* program, int lengthOfProgram , int output[MAX_PROGRAM_LENGTH])
{
	for(int i = 0; i < lengthOfProgram ; i ++)
	{
		if( program[i] != output[i])
		{
			return false;
		}
	}
	return true;
}



void
reset(int output[MAX_PROGRAM_LENGTH])
{
	for(int i = 0 ; i < MAX_PROGRAM_LENGTH ; i ++)
	{
		output[i] = 0;
	}
}

long long 
findInput(Computer c, int *program, int lengthOfProgram, int currentPoint)
{
	if (currentPoint < 0)
	{
		return c.registerA;
	}
	for (int i = 0; i < 8; i++)
	{
		uint64_t newValue = 8 * c.registerA + i;
		Computer copy = {.registerA = newValue, .registerB = c.registerB, .registerC = c.registerC};
		int localOutput[MAX_PROGRAM_LENGTH] = {0};
		run(&copy, program, lengthOfProgram, localOutput);
		if (localOutput[0] == program[currentPoint])
		{
			copy.registerA = newValue;
			long long result = findInput(copy, program, lengthOfProgram, currentPoint - 1);
			if (result != -1)
			{
				return result;
			}
		}
	}

	return -1;
}

int
main()
{
	Computer c = {.registerA = 0, .registerB = 0, .registerC = 0};


	/*
	 * Register A: 32916674
	 * Register B: 0
	 * Register C: 0
	 * Program: 2,4,1,1,7,5,0,3,1,4,4,0,5,5,3,0
	 */	

	/*
	 * Register A _ 
	 */
	int output[MAX_PROGRAM_LENGTH] = {0};
	c.registerA = 25358015;
	int program[] = {2,4,1,1,7,5,0,3,4,7,1,6,5,5,3,0};
	int lengthOfProgram = 16;

	printf("Part 1\n");
	run(&c,program,lengthOfProgram,output);
	printOutput(output);
	reset(output);
	int pos = MAX_PROGRAM_LENGTH-1;
	
	printf("Part 2:\n");
	c.registerA = 0;
	c.registerB = 0;
	c.registerC = 0;

	long long sol = findInput(c, program, lengthOfProgram, lengthOfProgram - 1);
	printf("%lld\n",sol);

	return 1;
}