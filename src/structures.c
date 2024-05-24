#include "structures.h"
#include <string.h>

void initialise_cpu(CPU *cpu) {
    memset(cpu, 0, sizeof(CPU));
    cpu->pstate.Z = 1; // INITIAL PSTATE value with Z flag set
}
