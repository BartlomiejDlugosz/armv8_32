#ifndef OPCODES_H
#define OPCODES_H

// all arrays are null terminated, for easy iteration when passing to functions
char *data_processing_opcodes[] = { "add", "adds", "sub", "subs", "cmp", "cmn", "neg", "negs", "and", "ands", "bic", "bics", "eor", "eon", "orr", "orn", "tst", "mvn", "mov", "movn", "movk", "movz", "madd", "msub", "mul", "mneg", NULL };
char *branch_opcodes[] = { "b", "br", "b.eq", "b.ne", "b.ge", "b.lt", "b.gt", "b.le", "b.al", NULL };
char *single_data_transfer_opcodes[] = { "ldr", "str", NULL };
char *directive_opcodes[] = { ".int", NULL };

#endif // OPCODES_H
