#include <stdio.h>

#include "command.h"

void print_command(command_t *comm) {
    switch (comm->opcode) {
        case PUSH_0:
        case PUSH_1:
            printf("%d %d", comm->opcode, comm->address);
            break;
        case OUTPUT_0:
        case OUTPUT_1:
        case RETURN:
        case HALT:
            printf("%d", comm->opcode);
            break;
        case POP_BRANCH:
            printf("%d %d %d", comm->opcode, comm->address, comm->stack);
            break;
        case INPUT_BRANCH:
        case JUMP:
        case CALL:
            printf("%d %d", comm->opcode, comm->address);
            break;
    }

    printf("\n");
}
