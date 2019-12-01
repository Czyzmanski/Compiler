
#include <stdio.h>
#include <stdlib.h>
#include <bool.h>

#define GROWTH_FACTOR 2
#define INITIAL_CAPACITY 2

#define NUM_OF_PROC 27 
#define MISSING_VAL -1
#define NO_VAL -2

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
	return comm
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
	v->arr[size] = elem;
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

int update_bracket_count(int c, int bracket_count){
	if(c == '{'){
		return bracket_count + 1;
	} else if(c == '}'){
		return bracket_count - 1;
	} else {
		return bracket_count;
	}
}

void remove_comment(){
	int c = getchar();
	while(c != EOF && c != '\n'){
		c = getchar();
	}
}

void call_procedure(int proc_name, vector* commands){
	command* comm = (command*) malloc(sizeof(command*));
	comm->a = CALL;
	comm->b = proc_name;
	add_element(commands, comm);
}

void push_bits_on_stack(int c, int stack_name, vector* commands){
	do {
		int stack_number = get_stack_number(stack_name);
		command* comm = get_new_command(MISSING_VAL, stack_number, NO_VAL);
		comm->a = c == '-' ? PUSH_0 : PUSH_1;
		add_element(commands, comm);
		c = next();
	} while(is_bit(c));
}

void read_block_definition(vector* commands);

void operate_on_stack(int stack_name, vector* commands){
	int c = next();
	if(is_bit(c)){
		push_bits_on_stack(c, stack_name, commands);
	} else {
		int stack_number = get_stack_number(stack_name);
		
		command* pop_branch_comm = get_new_command(POP_BRANCH, MISSING_VAL, stack_number);
		add_element(commands, pop_branch_comm);
		
		read_block_definition(c, commands);
		
		command* jump_comm = get_new_command(JUMP, MISSING_VAL, NO_VAL);
		add_element(commands, jump_comm);
		
		pop_branch_comm->b = commands->size;
		
		read_block_definition(c, commands);
		
		jump_comm->b = commands->size;
	}
}

void read_block_definition(int c, vector* commands){
	int bracket_count = 1;
	
	while(c != EOF && bracket_count > 0){
		c = next();
		bracket_count = update_bracket_count(c, bracket_count);
		if(is_uppercase_letter(c)){
			call_procedure(proc_name, commands);
		} else if(is_lowercase_letter(c)){
			operate_on_stack(c, commands);
		}
	}
}

void read_program(vector* commands, int* proc_addr){
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
		} 
		c = next();
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
	
	dispose(&commands);
	return 0;
}
