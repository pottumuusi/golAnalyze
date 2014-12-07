#include <stdio.h>
#include <stdlib.h>

#define PAUSE_LIMIT 2000000000

int progArguments(char **, int);
int nextState(char **, char **, int);
int checkNeighbours(char **, char**, int, int, int);
char ** allocateGridMemory(int);
void printGrid(char **, int);
void fillGrid(char **, int);
void freeGrid(char **, int);
void * errChkdMalloc(int);
void prepareExit(void);
void clearScreen(void);
void binaryPrint(int, int);

//maybe record amount of changes to * and changes to -
//then fill grid with those with more changes to it
//and then add the rest

//need tempGrid?

//gridSize can be given as a command line argument
int main(int argc, char * argv[]){
  int i;
  int k;
  int gridSize; //size of row in grid
  unsigned int pauseFreq;
  unsigned int pauseCounter = 0;
  char flagVar = 0;
  char ** grid;
  char ** copyGrid;
  
  gridSize = progArguments(argv, argc);
  grid = allocateGridMemory(gridSize);
  copyGrid = allocateGridMemory(gridSize);
  
  fillGrid(grid, gridSize);
  fillGrid(copyGrid, gridSize);
  
  printf("\nHow frequently do you want to pause execution?\n");
  printf("\t0 = no pause\n\t1 = pause after every change in state\n");
  printf("\t2 = count two changes and then pause\n");
  printf("\tMaximum value for pause frequency is: %d\n", PAUSE_LIMIT);
  
  do{
    printf("Select pause frequency> ");
    scanf("%d", &pauseFreq);
    if (pauseFreq > PAUSE_LIMIT){
      printf("\nerror: Selected pause frequency was too high\n");
      printf("\tMaximum value for pause frequency is: %d\n\n", PAUSE_LIMIT);
    }
  }while(pauseFreq > PAUSE_LIMIT);
  
  printGrid(grid, gridSize);
  
  do{
    if (pauseFreq > 0){
      if (pauseCounter >= pauseFreq){
        getchar();
        pauseCounter = 0;
      }
      pauseCounter++;
    }
    
    if (0 == (flagVar & 0b10000000)){
      flagVar = flagVar | nextState(grid, copyGrid, gridSize);
      flagVar = flagVar | 0b10000000;
      printGrid(copyGrid, gridSize);
    }
    else{
      flagVar = flagVar | nextState(copyGrid, grid, gridSize);
      flagVar = flagVar & 0b00000001;
      printGrid(grid, gridSize);
    }
  }while (0 == (flagVar & 0b00000001));
  
  freeGrid(grid, gridSize);
  freeGrid(copyGrid, gridSize);
  
  return 0;
}

void printGrid(char ** grid, int size){
  int i;
  int k;
  
  clearScreen();
  
  for (i = 0; i < size; i++){
    printf("%d\t", i);
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
      if (0 != checkNeighbours(currGrid, nextGrid, i, k, size)){
        assignDone++;
      }
    }
  }
  
  if (assignDone > 0){
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
  
  if (y + 1 <= size - 1)
    stopY = y + 1;
  else
    stopY = y;
  
  if (x + 1 <= size - 1)
    stopX = x + 1;
  else
    stopX = x;
  
  if (y - 1 >= 0)
    y = y - 1;
    
  if (x - 1 >= 0)
    x = x - 1;
  
  k = x;
  i = y;
  
  //check surrounding '*'
  do{
    do{
      if (('*' == currGrid[i][k]) && (0 == ((i == origY) && (k == origX)))){
        neighbourCount++;
      }
      k++;
    }while (k <= stopX);
    k = x;
    i++;
  }while (i <= stopY);
  
  if ('*' == currGrid[origY][origX]){
    if (neighbourCount > 3 || neighbourCount < 2){
      nextGrid[origY][origX] = '-';
      return 1;
    }
    else{
      nextGrid[origY][origX] = '*';
      return 0;
    }
  }
  else{
    if (3 == neighbourCount){
      nextGrid[origY][origX] = '*';
      return 2;
    }
    else{
      nextGrid[origY][origX] = '-';
      return 0;
    }
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
  
  grid[9][10] = '*';
  grid[10][11] = '*';
  grid[11][9] = '*';
  grid[11][10] = '*';
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
  
  allocHelper = (char **)errChkdMalloc(4 * size);
  
  for (i = 0; i < size; i++){
    allocHelper[i] = (char *)errChkdMalloc(size);
  }
  
  return allocHelper;
}

int progArguments(char ** argStrings, int argAmount){
  int gridSize;
  
  if (1 == argAmount){
    gridSize = 20;
  }
  else if(2 == argAmount){
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
    helpCounter++;
  }
  
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
  int i;
  
  for (i = size; i >= 0; i--){
    free(grid[i]);
  }
  free(grid);
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
      mask = mask >> 1;
    }
    printf("\n");
  }
}