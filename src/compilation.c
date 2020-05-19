#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "compilation.h"

#define BIT_0 '-'
#define BIT_1 '+'

#define COMMENT_BEGINNING ';'
#define BLOCK_BEGINNING '{'
#define BLOCK_ENDING '}'

#define SPECIAL_NAME '$'
#define FIRST_STACK_NAME 'a'
#define FIRST_PROCEDURE_NAME 'A'

static inline bool is_bit(int c) {
    return c == BIT_0 || c == BIT_1;
}

static inline int get_stack_number(int stack_name) {
    return stack_name - FIRST_STACK_NAME;
}

static inline int get_procedure_number(int proc_name) {
    return proc_name - FIRST_PROCEDURE_NAME;
}

static inline int next() {
    int c = getchar();

    while (c != EOF && isspace(c)) {
        c = getchar();
    }

    return c;
}

static inline int skip_comment() {
    int c = getchar();

    while (c != EOF && c != '\n') {
        c = getchar();
    }

    return c;
}

static inline int check_for_comment(int c) {
    while (c == COMMENT_BEGINNING) {
        c = skip_comment();

        if (c != EOF) {
            c = next();
        }
    }

    return c;
}

static inline void call_procedure(int proc_name, vector_t *commands) {
    command_t *call_comm = get_new_command(CALL, proc_name, NO_VAL);
    add_element(commands, call_comm);
}

static int push_bits_on_stack(int c, int stack_name, vector_t *commands) {
    c = check_for_comment(c);

    do {
        command_t *push_comm;
        int stack_number = get_stack_number(stack_name);

        if (c == BIT_0) {
            push_comm = get_new_command(PUSH_0, stack_number, NO_VAL);
        }
        else {
            push_comm = get_new_command(PUSH_1, stack_number, NO_VAL);
        }

        add_element(commands, push_comm);

        c = next();
    } while (is_bit(c));

    return c;
}

static int read_block_definition(int c, vector_t *commands);

static int branch(int c, command_t *branch_comm, vector_t *commands) {
    read_block_definition(c, commands);

    command_t *jump_comm = get_new_command(JUMP, MISSING_VAL, NO_VAL);
    add_element(commands, jump_comm);

    branch_comm->b = commands->size;

    c = next();
    c = check_for_comment(c);
    c = read_block_definition(c, commands);

    jump_comm->b = commands->size;

    return c;
}

static inline void pop_branch(int c, int stack_name, vector_t *commands) {
    int stack_number = get_stack_number(stack_name);

    command_t *pop_branch_comm = get_new_command(POP_BRANCH, MISSING_VAL, stack_number);
    add_element(commands, pop_branch_comm);

    branch(c, pop_branch_comm, commands);
}

static int operate_on_stack(int stack_name, vector_t *commands) {
    int c = next();

    while (c != EOF && (is_bit(c) || c == COMMENT_BEGINNING)) {
        c = check_for_comment(c);

        if (is_bit(c)) {
            c = push_bits_on_stack(c, stack_name, commands);
        }
        else if (c == COMMENT_BEGINNING) {
            c = check_for_comment(c);
        }
        else if (c == BLOCK_BEGINNING) {
            pop_branch(c, stack_name, commands);
            c = next();
        }
    }

    return c;
}

static int write_bits_to_output(int c, vector_t *commands) {
    c = check_for_comment(c);

    do {
        command_t *output_comm;

        if (c == BIT_0) {
            output_comm = get_new_command(OUTPUT_0, NO_VAL, NO_VAL);
        }
        else {
            output_comm = get_new_command(OUTPUT_1, NO_VAL, NO_VAL);
        }

        add_element(commands, output_comm);

        c = next();
    } while (is_bit(c));

    return c;
}

static inline void input_branch(int c, vector_t *commands) {
    command_t *input_branch_comm = get_new_command(INPUT_BRANCH, MISSING_VAL, NO_VAL);
    add_element(commands, input_branch_comm);

    branch(c, input_branch_comm, commands);
}

static int operate_on_output(vector_t *commands) {
    int c = next();

    while (c != EOF && (is_bit(c) || c == COMMENT_BEGINNING)) {
        c = check_for_comment(c);

        if (is_bit(c)) {
            c = write_bits_to_output(c, commands);
        }
        else if (c == COMMENT_BEGINNING) {
            c = check_for_comment(c);
        }
        else if (c == BLOCK_BEGINNING) {
            input_branch(c, commands);
            c = next();
        }
    }

    return c;
}

static int read_block_definition(int c, vector_t *commands) {
    c = next();

    while (c != EOF && c != BLOCK_ENDING) {
        if (isupper(c)) {
            call_procedure(c, commands);
            c = next();
        }
        else if (islower(c)) {
            c = operate_on_stack(c, commands);
        }
        else if (c == SPECIAL_NAME) {
            c = operate_on_output(commands);
        }
        else {
            c = check_for_comment(c);
        }
    }

    return c;
}

void update_call_commands(vector_t *commands, const int proc_addr[NUM_OF_PROC]) {
    for (int i = 0; i < commands->size; i++) {
        command_t *comm = commands->arr[i];
        if (comm->a == CALL) {
            int proc_name = comm->b;
            int proc_number = get_procedure_number(proc_name);
            comm->b = proc_addr[proc_number];
        }
    }
}

void compile_program(vector_t *commands, int *proc_addr) {
    add_element(commands, get_new_command(JUMP, MISSING_VAL, NO_VAL));

    int c = next();
    while (c != EOF) {
        if (c == COMMENT_BEGINNING) {
            c = skip_comment();
        }
        else if (isupper(c)) {
            int key = get_procedure_number(c);
            proc_addr[key] = commands->size;

            c = next();
            c = check_for_comment(c);
            c = read_block_definition(c, commands);

            command_t *return_comm = get_new_command(RETURN, NO_VAL, NO_VAL);
            add_element(commands, return_comm);
        }
        else if (c == BLOCK_BEGINNING) {
            command_t *jump_comm = commands->arr[0];
            jump_comm->b = commands->size;

            c = read_block_definition(c, commands);

            command_t *halt_comm = get_new_command(HALT, NO_VAL, NO_VAL);
            add_element(commands, halt_comm);
        }

        if (c != EOF) {
            c = next();
        }
    }
}
