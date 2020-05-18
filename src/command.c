#include <stdio.h>

#include "command.h"

void print_command(command_t *comm) {
    switch (comm->a) {
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
