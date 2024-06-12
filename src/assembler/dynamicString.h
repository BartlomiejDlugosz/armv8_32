#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

typedef struct dynamicString {
    char *data;
    int current_size;
    int max_size;
} * dynamicString;

dynamicString createNewDynamicString(int);
void resizeArray(dynamicString);
void addChar(dynamicString, char);
void addString(dynamicString, char *);
char getChar(dynamicString, int);
char *getString(dynamicString);
void freeDynamicString(dynamicString);

// DYNAMIC_STRING_H
#endif
