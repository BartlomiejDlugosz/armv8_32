#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

typedef struct dynamicString {
    char *data;
    int current_size;
    int max_size;
} dynamic_string;

dynamic_string *createNewDynamicString(int);
void resizeArray(dynamic_string *);
void addChar(dynamic_string *, char);
void addString(dynamic_string *, char *);
char getChar(dynamic_string *, int);
char *getString(dynamic_string *);
void freeDynamicString(dynamic_string *);

#endif  // DYNAMIC_STRING_H