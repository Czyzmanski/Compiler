
#include <stdio.h>
#include <stdlib.h>

#define GROWTH_FACTOR 2
#define INITIAL_CAPACITY 2

#define NUM_OF_PROC 26 
#define MISSING_VAL -1
#define NO_VAL -2

typedef enum {
	false = 0,
	true = 1
} bool;

typedef enum {
    PUSH_0 = 0,
    PUSH_1 = 1,
    OUTPUT_0 = 2,
    OUTPUT_1 = 3,
    POP_BRANCH = 4,
    INPUT_BRANCH = 5,
    JUMP = 6,
    CALL = 7,
    RETURN = 8,
    HALT = 9
} opcode;

typedef struct {
    opcode a;
    int b;
    int c;
} command;

command* get_new_command(opcode a, int b, int c){
	command* comm = (command*) malloc(sizeof(command*));
	comm->a = a;
	comm->b = b;
	comm->c = c;
	return comm;
}

typedef struct {
	command** arr;
	int size;
	int capacity;
} vector;

void init(vector* v){
	v->size = 0;
	v->capacity = INITIAL_CAPACITY;
	v->arr = (command**) malloc(v->capacity * sizeof(command*));
}

void reallocate(vector* v){
	v->capacity = GROWTH_FACTOR * v->capacity;
	v->arr = (command**) realloc(v->arr, v->capacity * sizeof(command*));
}

void add_element(vector* v, command* elem){
	if(v->size == v->capacity){
		reallocate(v);
	}
	v->arr[v->size] = elem;
	v->size++;
}

void dispose(vector* v){
	for(int i = 0; i < v->size; i++){
		free(v->arr[i]);
	}
	free(v->arr);
}

bool is_uppercase_letter(int c){
	return ('A' <= c && c <= 'Z');
}

bool is_lowercase_letter(int c){
	return ('a' <= c && c <= 'z');
}

bool is_whitespace_character(int c){
	return (c == ' ' || c == '\t' || c == '\n');
}

bool is_bit(int c){
	return (c == '-' || c == '+');
}

int next(){
	int c = getchar();
	while(is_whitespace_character(c)){
		c = getchar();
	}
	return c;
}

int get_proc_number(int proc_name){
	return proc_name - 'A';
}

int get_stack_number(int stack_name){
	return stack_name - 'a';
}

void remove_comment(){
	int c = getchar();
	while(c != EOF && c != '\n'){
		c = getchar();
	}
}

void call_procedure(int proc_name, vector* commands){
	command* call_comm = get_new_command(CALL, proc_name, NO_VAL);
	add_element(commands, call_comm);
}

void push_bits_on_stack(int c, int stack_name, vector* commands){
	do {
		command* push_comm;
		int stack_number = get_stack_number(stack_name);
		
		if(c == '-'){
			push_comm = get_new_command(PUSH_0, stack_number, NO_VAL);
		} else {
			push_comm = get_new_command(PUSH_1, stack_number, NO_VAL);
		}
		
		add_element(commands, push_comm);
		
		c = next();
	} while(is_bit(c));
}

void read_block_definition(int c, vector* commands);

void branch(int c, command* branch_comm, vector* commands){
	read_block_definition(c, commands);
	
	command* jump_comm = get_new_command(JUMP, MISSING_VAL, NO_VAL);
	add_element(commands, jump_comm);
	
	branch_comm->b = commands->size;
	
	c = next();
	read_block_definition(c, commands);
	
	jump_comm->b = commands->size;
}

void pop_branch(int c, int stack_name, vector* commands){
	int stack_number = get_stack_number(stack_name);
	
	command* pop_branch_comm = get_new_command(POP_BRANCH, MISSING_VAL, stack_number);
	add_element(commands, pop_branch_comm);
	
	branch(c, pop_branch_comm, commands);
}

void operate_on_stack(int stack_name, vector* commands){
	int c = next();
	if(is_bit(c)){
		push_bits_on_stack(c, stack_name, commands);
	} else {
		pop_branch(c, stack_name, commands);
	}
}

void write_bits_to_output(int c, vector* commands){
	do {
		command* output_comm;
		
		if(c == '-'){
			output_comm = get_new_command(OUTPUT_0, NO_VAL, NO_VAL);
		} else {
			output_comm = get_new_command(OUTPUT_1, NO_VAL, NO_VAL);
		}
		
		add_element(commands, output_comm);
		
		c = next();
	} while(is_bit(c));
}

void input_branch(int c, vector* commands){
	command* input_branch_comm = get_new_command(INPUT_BRANCH, MISSING_VAL, NO_VAL);
	add_element(commands, input_branch_comm);
	
	branch(c, input_branch_comm, commands);
}

void operate_on_input(vector* commands){
	int c = next();
	if(is_bit(c)){
		write_bits_to_output(c, commands);
	} else {
		input_branch(c, commands);
	}
}

void read_block_definition(int c, vector* commands){
	while(c != EOF && c != '}'){
		c = next();
		if(is_uppercase_letter(c)){
			call_procedure(c, commands);
		} else if(is_lowercase_letter(c)){
			operate_on_stack(c, commands);
		} else if(c == '$'){
			operate_on_input(commands);
		}
	}
}

void read_program(vector* commands, int* proc_addr){
	command* jump_comm = get_new_command(JUMP, MISSING_VAL, NO_VAL);
	add_element(commands, jump_comm);
	
	int c = next();
	while(c != EOF){
		if(c == ';'){
			remove_comment();
		} else if(is_uppercase_letter(c)){
			int key = get_proc_number(c);
			proc_addr[key] = commands->size;
			
			c = next();
			read_block_definition(c, commands);
			
			command* return_comm = get_new_command(RETURN, NO_VAL, NO_VAL);
			add_element(commands, return_comm);
		} else if(c == '{'){
			command* jump_comm = commands->arr[0];
			jump_comm->b = commands->size;
			
			read_block_definition(c, commands);
			
			command* halt_comm = get_new_command(HALT, NO_VAL, NO_VAL);
			add_element(commands, halt_comm);
		}
		c = next();
	}
}

void update_call_commands(vector* commands, int* proc_addr){
	for(int i = 0; i < commands->size; i++){
		command* comm = commands->arr[i];
		if(comm->a == CALL){
			int proc_name = comm->b;
			int proc_number = get_proc_number(proc_name);
			comm->b = proc_addr[proc_number];
		}
	}
}

void print_command(command* comm){
	switch(comm->a){
		case PUSH_0:
		case PUSH_1:
			printf("%d %d", comm->a, comm->b);
			break;
		case OUTPUT_0:
		case OUTPUT_1:
		case RETURN:
		case HALT:
			printf("%d", comm->a);
			break;
		case POP_BRANCH:
			printf("%d %d %d", comm->a, comm->b, comm->c);
			break;
		case INPUT_BRANCH:
		case JUMP:
		case CALL:
			printf("%d %d", comm->a, comm->b);
			break;
	}
	printf("\n");
}

void write_compiled_code(vector* commands){
	for(int i = 0; i < commands->size; i++){
		print_command(commands->arr[i]);
	}
}

int main(){
	vector commands;
	init(&commands);
	
	int proc_addr[NUM_OF_PROC];
	for(int i = 0; i < NUM_OF_PROC; i++){
		proc_addr[i] = MISSING_VAL;
	}
	
	read_program(&commands, proc_addr);
	update_call_commands(&commands, proc_addr);
	write_compiled_code(&commands);
	
	dispose(&commands);
	
	return 0;
}
