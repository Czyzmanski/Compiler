#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>
#include <assert.h>

#define MISSING_VAL -1
#define NO_VAL -2

enum opcode {
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
};

typedef enum opcode opcode_t;

struct command {
    opcode_t a;
    int b;
    int c;
};

typedef struct command command_t;

void print_command(command_t *comm);

static inline command_t *get_new_command(opcode_t a, int b, int c) {
    command_t *comm = malloc(sizeof(command_t));
    assert(comm != NULL);

    comm->a = a;
    comm->b = b;
    comm->c = c;

    return comm;
}

#endif // COMMAND_H
