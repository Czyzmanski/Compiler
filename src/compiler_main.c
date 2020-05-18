#include "vector.h"
#include "compilation.h"

int main() {
    vector_t commands;
    init(&commands);

    int proc_addr[NUM_OF_PROC];
    for (int i = 0; i < NUM_OF_PROC; i++) {
        proc_addr[i] = MISSING_VAL;
    }

    compile_program(&commands, proc_addr);
    update_call_commands(&commands, proc_addr);
    write_compiled_code(&commands);

    dispose(&commands);

    return 0;
}
