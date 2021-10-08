#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file
#include <string.h>

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}



/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */

bool compare(char* in, char* og){
	char* tmin = in;
	char* tmog = og;
	while(*tmin != '\0' || *tmog != '\0'){ //run as long as there are characters in the token
		if((*tmin != *tmog) || (*tmin == '\0' && *tmog != '\0') || (*tmin != '\0' && *tmog == '\0')){//if onen token runs out of words or a token is different return false
			return false;
		}
		else if(*tmin == *tmog){//if  the tokens continue being the same, this if statement will move to the next character
			tmin++;
			tmog++;
		}
	}
	return true;
}

void addFunc(char* reg1, char* reg2, char* reg3){
	int a = atoi(reg1 + 1);//the +1 gets rid of the "X" and the atoi function gives me the integer value of the char
	int b = atoi(reg2 + 1);
	int c = atoi(reg3 + 1);
	reg[a] = reg[b] + reg[c];//this will add the value of the second and third register and make it the first register
}

//the addi function is exactly like the add function but the third value is just a number instead of a register
void addiFunc(char* reg1, char* reg2, char* reg3){
	int a = atoi(reg1 + 1);
	int b = atoi(reg2 + 1);
	int c = atoi(reg3);
	reg[a] = reg[b] + c;
}

//the store function does the same thing as the addi function with the registers but then
//gets the data of the first register, gets the data of the second register and adds the offset and 
//then stores the data in the new adress of the mem.txt file. 
void swFunc(char* reg1, char* num, char* add1){
	int32_t a = atoi(reg1 + 1);
	int32_t b = atoi(num);
	int32_t c = atoi(add1 + 1);
	int32_t data = reg[a];
	int32_t address = b + reg[c];
	write_address(data, address, "mem.txt");
}

//the load function does the same thing as the addi function with the registers but then
//gets the data of the second register and adds the offset and then makes the data of the first
//register equal to the adress of the second register plus the offset.
void lwFunc(char* reg1, char* num, char* add1){
	int32_t a = atoi(reg1 + 1);
	int32_t b = atoi(num);
	int32_t c = atoi(add1 + 1);
	int32_t address = b + reg[c];
	reg[a] = read_address(address, "mem.txt");
}

bool interpret(char* instr){
	//sets up the words that we will compare to the tokens
	char add[] = "ADD";
	char addi[] = "ADDI";
	char lw[] = "LW";
	char sw[] = "SW";
	//sets up the tokenizer and gets the first token 
	char** tokens = tokenize(instr);
	char* token = tokens[0];
	//the compare funcion takes in the first token and compares it to the key words we set up
	if(compare(token,add)){
		addFunc(tokens[1],tokens[2],tokens[3]);
		free(tokens);
	}
	else if(compare(token,addi)){
		addiFunc(tokens[1],tokens[2],tokens[3]);
		free(tokens);
	}
	else if(compare(token,lw)){
		lwFunc(tokens[1],tokens[2],tokens[3]);
		free(tokens);
	}
	else if(compare(token,sw)){
		swFunc(tokens[1],tokens[2],tokens[3]);
		free(tokens);
	}
	//if the first token does not match any of the key words the program will print out that the command was not found.
	else{
		printf("commnand not found");
	}
	return true;
}
/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(){
	int32_t data_to_write = 0xFFF; // equal to 4095
	int32_t address = 0x98; // equal to 152
	char* mem_file = "mem.txt";

	// Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
	int32_t write = write_address(data_to_write, address, mem_file);
	if(write == (int32_t) NULL)
		printf("ERROR: Unsucessful write to address %0X\n", 0x40);
	int32_t read = read_address(address, mem_file);

	printf("Read address %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}

void print_regs(){
	int col_size = 10;
	for(int i = 0; i < 8; i++){
		printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
		printf(" X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
		printf(" X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
		printf(" X%02i:%.*lld\n", i+24, col_size, (long long int) reg[i+24]);
	}
}


/**
 * Your code goes in the main
 *
 */
int main(){
	// Do not write any code between init_regs
	init_regs(); // DO NOT REMOVE THIS LINE
	print_regs();

	// Below is a sample program to a write-read. Overwrite this with your own code.
	//write_read_demo();

	printf(" RV32 Interpreter.\nType RV32 instructions. Use upper-case letters and space as a delimiter.\nEnter 'EOF' character to end program\n");

	char* instruction = malloc(1000 * sizeof(char));
	bool is_null = false;
	// fgets() returns null if EOF is reached.
	is_null = fgets(instruction, 1000, stdin) == NULL;
	while(!is_null){
		interpret(instruction);
		printf("\n");
		print_regs();
		printf("\n");

		is_null = fgets(instruction, 1000, stdin) == NULL;
	}

	printf("Good bye!\n");

	return 0;
}
