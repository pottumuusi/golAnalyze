#include <stdio.h>
#include <stdlib.h>

int progArguments(char **, int);
int nextState(char **, char **, int);
int checkNeighbours(char **, int, int, int);
char ** allocateGridMemory(int);
//void fillGrid(char **, int, char * gridState);
void printGrid(char **, int);
void fillGrid(char **, int);
void * errChkdMalloc(int);
void prepareExit(void);
void clearScreen(void);

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
  
  printf("in main grid == 0x%08x\n", grid);
  
  fillGrid(grid, gridSize);
  
  do{
    //delay loop
    for (i = 0; i < 1000000; i++){
      for (k = 0; k < 1000000; k++){
        
      }
    }
    getch();
    if (0 == flagVar & 0b10000000){
      flagVar = flagVar | nextState(grid, copyGrid, gridSize);
      // binaryPrint(flagVar);
      flagVar = flagVar | 0b10000000;
      printGrid(copyGrid, gridSize);
    }
    else{
      flagVar = flagVar | nextState(copyGrid, grid, gridSize);
      // binaryPrint(flagVar);
      flagVar = flagVar & 0b00000001;
      printGrid(grid, gridSize);
    }
  }while (0 == flagVar & 0b00000001);
  
  free(grid);
  
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

int nextState(char ** grid, char ** copyGrid, int size){
  //return 1 if grid will be empty
  
  int i;
  int k;
  int assignDone = 0;
  
  for (i = 0; i < size; i++){
    for (k = 0; k < size; k++){
      if (1 == checkNeighbours(grid, i, k, size)){
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

int checkNeighbours(char ** grid, int y, int x, int size){
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
  
  if (y - 1 >= 0)
    y = y - 1;
    
  if (x - 1 >= 0)
    x = x - 1;
  
  if (y + 1 <= size - 1)
    stopY = y + 1;
  else
    stopY = y;
  
  if (x + 1 <= size - 1)
    stopX = x + 1;
  else
    stopY = x;
  
  k = x;
  i = y;
  
  do{
    do{
      if ('*' == grid[i][k] && (i != origY && k != origX))
        neighbourCount++;
      k++;
    }while (k <= stopX);
    k = x;
    i++;
  }while (i <= stopY);
  
  //printf("neighbourCount == %d\n", neighbourCount);
  
  if ('*' == grid[origY][origX]){
    printf("neighbourCount == %d\n", neighbourCount);
    if (neighbourCount > 3 || neighbourCount < 2){
      grid[origY][origX] = '-';
      //printf("returning 1\n");
      return 1;
    }
    else
      return 0;
  }
  else{
    if (3 == neighbourCount){
      grid[origY][origX] = 'x';
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
  printf("in allocateGridMemory allocHelper == 0x%08x\n", allocHelper);
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