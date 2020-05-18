#ifndef COMPILATION_H
#define COMPILATION_H

#include "vector.h"

#define NUM_OF_PROC 26

void compile_program(vector_t *commands, int *proc_addr);

void update_call_commands(vector_t *commands, const int proc_addr[NUM_OF_PROC]);

static inline void write_compiled_code(vector_t *commands) {
    for (int i = 0; i < commands->size; i++) {
        print_command(commands->arr[i]);
    }
}

#endif // COMPILATION_H
