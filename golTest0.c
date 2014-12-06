#include <stdio.h>
#include <stdlib.h>

int progArguments(char **, int);
int nextState(char **, char **, int);
int checkNeighbours(char **, char**, int, int, int);
char ** allocateGridMemory(int);
//void fillGrid(char **, int, char * gridState);
void printGrid(char **, int);
void fillGrid(char **, int);
void freeGrid(char **, int);
void * errChkdMalloc(int);
void prepareExit(void);
void clearScreen(void);
void binaryPrint(int, int);

//gridSize can be given as a command line argument
int main(int argc, char * argv[]){
  int i;
  int k;
  int gridSize; //size of row in grid
  char flagVar = 0;
  char ** grid;
  char ** copyGrid;
  
  gridSize = progArguments(argv, argc);
  grid = allocateGridMemory(gridSize);
  copyGrid = allocateGridMemory(gridSize);
  
  printf("in main grid == 0x%08x\n", (unsigned int)grid);
  
  fillGrid(grid, gridSize);
  fillGrid(copyGrid, gridSize);
  
  /*
  printf("testing binaryPrint with 00000100\n");
  binaryPrint(0b00000100, 1);
  */
  
  do{
    //delay loop
    /*
    for (i = 0; i < 1000000; i++){
      for (k = 0; k < 1000000; k++){
        
      }
    }
    */
    getchar();
    if (0 == (flagVar & 0b10000000)){
      printf("***executing if***\n");
      flagVar = flagVar | nextState(grid, copyGrid, gridSize);
      flagVar = flagVar | 0b10000000;
      printGrid(copyGrid, gridSize);
      printf("Content of flagVar: ");
      binaryPrint(flagVar, 1);
    }
    else{
      printf("***executing else***\n");
      flagVar = flagVar | nextState(copyGrid, grid, gridSize);
      flagVar = flagVar & 0b00000001;
      printGrid(grid, gridSize);
      printf("Content of flagVar: ");
      binaryPrint(flagVar, 1);
    }
  }while (0 == (flagVar & 0b00000001));
  
  free(grid);
  free(copyGrid);
  
  //freeGrid(grid);
  //freeGrid(copyGrid);
  
  return 0;
}

void printGrid(char ** grid, int size){
  int i;
  int k;
  
  clearScreen();
  
  for (i = 0; i < size; i++){
    for (k = 0; k < size; k++){
      printf("%c", grid[i][k]);
    }
    printf("\n");
  }
}

int nextState(char ** currGrid, char ** nextGrid, int size){
  //return 1 if grid will be empty
  
  int i;
  int k;
  int assignDone = 0;
  
  //---------copy grid to copyGrid-----------
  
  for (i = 0; i < size; i++){
    for (k = 0; k < size; k++){
      if (1 == checkNeighbours(currGrid, nextGrid, i, k, size)){
        assignDone = 1;
      }
    }
  }
  
  if (1 == assignDone){
    return 0;
  }
  else{
    return 1;
  }
}

//int checkNeighbours(char ** grid, char ** copyGrid, int y, int x, int size){
int checkNeighbours(char ** currGrid, char ** nextGrid, int y, int x, int size){
  char neighbourCount = 0;
  int i;
  int k;
  int stopX;
  int stopY;
  int origX = x;
  int origY = y;
  
  /*
  printf("origX == %d\n", origX);
  printf("origY == %d\n", origY);
  */
  
  if (y + 1 <= size - 1)
    stopY = y + 1;
  else
    stopY = y;
  
  if (x + 1 <= size - 1)
    stopX = x + 1;
  else
    stopY = x;
  
  if (y - 1 >= 0)
    y = y - 1;
    
  if (x - 1 >= 0)
    x = x - 1;
  
  k = x;
  i = y;
  
  //check surrounding '*'
  do{
    do{
      //if ('*' == grid[i][k] && (i != origY && k != origX))
      if (('*' == currGrid[i][k]) && ((i != origY) && (k != origX))){
        printf("found neighbour at: %d,%d\n", i, k);
        neighbourCount++;
      }
      k++;
    }while (k <= stopX);
    k = x;
    i++;
  }while (i <= stopY);
  
  //printf("neighbourCount == %d\n", neighbourCount);
  
  if ('*' == currGrid[origY][origX]){
    printf("neighbourCount == %d\n", neighbourCount);
    
    printf("y == %d, x == %d\n", y, x);
    printf("origY == %d, origX == %d\n", origY, origX);
    printf("stopY == %d, stopX == %d\n", stopY, stopX);
    if (neighbourCount > 3 || neighbourCount < 2){
      //grid[origY][origX] = '-';
      nextGrid[origY][origX] = '-';
      //printf("returning 1\n");
      return 1;
    }
    else
      return 0;
  }
  else{
    if (3 == neighbourCount){
      //grid[origY][origX] = '*';
      nextGrid[origY][origX] = '*';
      //printf("returning 1\n");
      return 1;
    }
    else
      return 0;
  }
}

void fillGrid(char ** grid, int size){
  int i;
  int k;
  
  
  for (i = 0; i < size; i++){
    for (k = 0; k < size; k++){
      grid[i][k] = '-';
    }
  }
  
  grid[10][10] = '*';
  grid[10][11] = '*';
  grid[11][10] = '-';
  grid[11][11] = '*';
}

void * errChkdMalloc(int size){
  void * memPointer;
  
  memPointer = malloc(size);
  if (NULL == memPointer){
    printf("\n\nError while allocating memory. Program will stop.\n");
    exit(0);
  }
  else{
    return memPointer;
  }
}

char ** allocateGridMemory(int size){
  //return 1 if error allocating memory
  int i;
  char ** allocHelper;
  /*
  printf("%08x\n", &(grid[0][0]));
  printf("%08x\n", &(grid[0][19]));
  */
  
  //printf("size == %d\nsizeof(char**) == %d\n", size, sizeof(char**));
  //grid = (char **)errChkdMalloc(4 * size);
  allocHelper = (char **)errChkdMalloc(4 * size);
  
  //printf("%08x\n", &(grid[0][0]));
  //printf("%08x\n", &(grid[0][19]));
  
  for (i = 0; i < size; i++){
    //printf("before alloc 0x%08x, ", grid[i]);
    //grid[i] = (char *)errChkdMalloc(size);
    allocHelper[i] = (char *)errChkdMalloc(size);
    //printf("after alloc 0x%08x\n", grid[i]);
    //printf("allocLoop %d\n", i);
  }
  //printf("%08x\n", &(grid[0][0]));
  //printf("%08x\n", &(grid[0][19]));
  
  //grid[0][0] = 1;
  //allocHelper[0][0] = 1;
  printf("assign done\n");
  printf("in allocateGridMemory allocHelper == 0x%08x\n", (unsigned int)allocHelper);
  //printf("in allocateGridMemory grid == 0x%08x\n", grid);
  return allocHelper;
}

int progArguments(char ** argStrings, int argAmount){
  int gridSize;
  
  if (1 == argAmount){
    gridSize = 20;
  }
  else if(2 == argAmount){
    //if (1 == strToInt(argStrings[1], gridSize))
    gridSize = strToInt(argStrings[1]);
  }
  
  return gridSize;
}

int strToInt(char * convStr){
  int i;
  char k;
  int converted;
  int multiplier;
  char helpCounter = 1;
  
  //printf("in strToInt convStr == %s", convStr);
  
  for (i = 0; convStr[i] != '\n' && convStr[i] != '\0'; i++){
    if (convStr[i] < '0' || convStr[i] > '9'){
      printf("\nInvalid command line argument given. Program will stop\n");
      exit(0);
    }
  }
  //now i is the index of the end of convStr
  
  converted = convStr[i - 1] - 48;
  for (i = i - 2; i >= 0; i--){
    multiplier = 10;
    for (k = 0; k < helpCounter - 1; k++){
      multiplier = multiplier * 10;
    }
    converted = converted + ((convStr[i] - 48) * multiplier);
    printf("convStr[i] -48 == %d, multiplier == %d, ", convStr[i] - 48, multiplier);
    printf("adding %d to previous value\n", ((convStr[i] - 48) * multiplier ));
    helpCounter++;
  }
  
  printf("converted command line argument is: %d\n", converted);
  
  return converted;
}

void clearScreen(void){
  char i = 0;
  
  while (i < 40){
    printf("\n");
    i++;
  }
}

void prepareExit(void){
  //write necessary data to files
}

void freeGrid(char ** grid, int size){
  
}

void binaryPrint(int printVar, int byteSize){
  char i;
  //unsigned short mask = 0x0080;
  unsigned int mask;
  
  if (byteSize > 4 || byteSize < 1){
    printf("error: binaryPrint function does not support given byte size.\n");
  }
  else{
    //make mask correct for current byteSize
    if (byteSize == 1){
      mask = 0x00000080;
    }
    else if(byteSize == 2){
      mask = 0x00008000;
    }
    else if(byteSize == 3){
      mask = 0x00800000;
    }
    else{
      mask = 0x80000000;
    }
    
    for (i = 0; i < (byteSize * 8); i++){
      if (0 != (printVar & mask)){
        putchar('1');
      }
      else{
        putchar('0');
      }
      /*
      printf("\nmask == 0x%08x\n", mask);
      printf("printVar == 0x%08x\n", printVar);
      printf("printVar & mask == 0x%08x\n\n", (printVar & mask));
      */
      mask = mask >> 1;
    }
    printf("\n");
  }
}