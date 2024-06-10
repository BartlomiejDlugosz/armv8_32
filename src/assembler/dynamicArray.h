
typedef struct dynamicArray {
  char *data;
  int current_size;
  int max_size;
} *dynamicArray;

dynamicArray createNewDynamicArray(int);
void resizeArray(dynamicArray);
void addChar(dynamicArray, char);
void addString(dynamicArray, char*);
char getChar(dynamicArray, int);
char *getString(dynamicArray);
