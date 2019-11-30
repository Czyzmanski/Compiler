
#include <stdio.h>
#include <stdlib.h>
#include <bool.h>

#define GROWTH_FACTOR 2
#define INITIAL_CAPACITY 2

#define NUM_OF_PROC 27 
#define MISSING_ADDR -1

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
		command* comm = malloc(sizeof(command*));
		comm->a = c == '-' ? PUSH_0 : PUSH_1;
		comm->b = get_stack_number(stack_name);
		add_element(commands, comm);
		c = next();
	} while(is_bit(c));
}

void operate_on_stack(int stack_name, vector* commands){
	int c = next();
	if(is_bit(c)){
		push_bits_on_stack(c, stack_name, commands);
	} else {
		// will be opcode 4
	}
}

void read_block_definition(int proc_name, vector* commands, int* proc_addr){
	int key = get_proc_number(proc_name);
	proc_addr[key] = commands->size;
	
	int c = next();
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
			read_block_definition(c, commands, proc_addr);
		} 
		c = next();
	}
}

int main(){
	vector commands;
	init(&commands);
	
	int proc_addr[NUM_OF_PROC];
	for(int i = 0; i < NUM_OF_PROC; i++){
		proc_addr[i] = MISSING_ADDR;
	}
	
	read_program(&commands, proc_addr);
	
	dispose(&commands);
	return 0;
}
