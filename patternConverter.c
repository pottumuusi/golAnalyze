/*
 * This program takes names of text files containing game of life patterns as command
 * line arguments and converts them so that they can be used by the test programs and
 * maybe even the final program. Who knows.
 */

/* If program is slow to use due to asking of filename consider processing a second
 * command line argument which would be the name of output file*/

/* First is a byte or many bytes which tell the length of the rows in grid
 * After that the file contains first bits which represent state of a cell
 * zero meaning dead and one alive.*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_ROW_SIZE 10 //change the value
#define FILE_NAME_SIZE 30
#define READ_BUFFER_SIZE 100000
#define WRITE_BUFFER_SIZE 100000
#define CHECK_BUFFER_SIZE 100000000
#define CLEAN_EXIT(str, file) free(str); fclose(file); exit(0);

void askFileName(char *);
void createWriteBuffer(char *);
void writeConvertedContent(char *);
void convertContent(FILE *, unsigned long long);
char * allocMemory(int);
unsigned long long checkFileContent(FILE *, char *);

int main(int argc, char * argv[]){
  unsigned long long rowLength;
  FILE * gridFile;
  
  gridFile = fopen(argv[1], "r");

  rowLength = checkFileContent(gridFile, argv[1]);

  convertContent(gridFile, rowLength);
  return 0;
}

unsigned long long checkFileContent(FILE * gridFile, char * fileName){
  /* check that file contains a grid filled with '-' and '*' characters*/
  /* also check that grid has rows short enough and that they all have same length*/
  /* returns length of the rows*/
  char * row;
  unsigned long long rowLen = 0;
  unsigned long long checkIndex = 0;
  unsigned long long rowCounter = 0;

  row = allocMemory(CHECK_BUFFER_SIZE);

  while (NULL != fgets(row, CHECK_BUFFER_SIZE, gridFile)){
    /*check that file contains no bad characters and set value for checkIndex which is used to
     * determine if file has too long rows*/
    while('\0' != row[checkIndex]){
      if (row[checkIndex] != '-' && row[checkIndex] != '*' && row[checkIndex] != '\n'){
        printf("\nError: Processed file %s has character(s) other than \"*\" or \"-\"", fileName);
        //printf("\n(row #%d has character(%c) @ column %d)\n\n", rowCounter + 1, row[checkIndex], checkIndex);
        if ('\0' == row[checkIndex]){
          printf("\n[DEBUG] \'\\0\' == row[checkIndex] =(\n");
        }
        printf("\n(row #%d has character(%d) ", rowCounter + 1, row[checkIndex], checkIndex);
        printf("@ column %d)\n\n", checkIndex);
        CLEAN_EXIT(row, gridFile)
      }
      checkIndex++;
    }
    /* do not count line feed to length of row */
    if ('\n' == row[checkIndex - 1]){
      checkIndex--;
    }
    printf("\ncheckIndex == %d after while\n\n", checkIndex);

    if (checkIndex > MAX_ROW_SIZE){
      printf("\nError: Processed file %s has too long row of characters\n", fileName);
      printf("Maximum allowed amount of characters per row is: %d\n", MAX_ROW_SIZE);
      printf("(row #%d had ", rowCounter + 1);
      printf("%d characters)\n\n", checkIndex);
      CLEAN_EXIT(row, gridFile)
    }

    if (0 == checkIndex){
      printf("\nError: Processed file %s has an empty row\n", fileName);
      CLEAN_EXIT(row, gridFile)
    }

    if (0 == rowLen){
      rowLen = checkIndex;
    }

    if (checkIndex != rowLen){
      printf("\nError: Processed file %s has rows with different lengths\n", fileName);
      printf("(row #%d)\n", rowCounter + 1);
      CLEAN_EXIT(row, gridFile)
    }

    rowCounter++;
    checkIndex = 0;
  }
  free(row);
  rewind(gridFile);
  return rowLen;
}

char * allocMemory(int allocAmount){
  /*check errors in memory allocation*/
  void * allocHelper = NULL;

  if (NULL != (allocHelper = malloc(allocAmount))){
    return (char *)allocHelper;
  }
  printf("Error: Could not allocate memory for rows\n");
  exit(0);
}

void convertContent(FILE * gridFile, unsigned long long rowLen){
  int i;
  int rowLength = 0;
  //int checkStrSize = 0;
  char * outName;
  char * readBuffer;
  char * writeBuffer;
  char * binWriteBuffer;
  char checkStr[100];
  FILE * outFile;

  //writeBuffer = allocMemory(WRITE_BUFFER_SIZE);
  readBuffer = allocMemory(READ_BUFFER_SIZE);
  outName = allocMemory(FILE_NAME_SIZE + 5);
  /*+5 because file type extension takes 4 bytes (.bin) and then terminating character*/

  askFileName(outName);
  printf("[DEBUG] askFileName executed succesfully\n");
  outFile = fopen(outName, "wb");

  /* write the length of rows to the beginning of the outFile*/
  fwrite(&rowLen, sizeof(unsigned long long), 1, outFile);
  rowLen = rowLen + 1; //DEBUG
  fwrite(&rowLen, sizeof(unsigned long long), 1, outFile);

  while (NULL != fgets(writeBuffer, WRITE_BUFFER_SIZE, gridFile)){
    /* write 0 if processed character is -
     * write 1 if processed character is */
    for (i = 0; writeBuffer[i] != '\0'; i++){
      if ('\n' != writeBuffer[i]){
        if ('-' == writeBuffer[i]){
          checkStr[i] = writeBuffer[i] = 0x00;
        }
        else{
          checkStr[i] = writeBuffer[i] = 0x01;
        }
        fwrite((writeBuffer + i), sizeof(char), 1, outFile);
      }
    }
    checkStr[i] = '\0';
  }

  printf("\nContent of checkStr...\n");

  for (i = 0; '\0' != checkStr[i]; i++){
    printf("%02x | ", checkStr[i]);
    if (0 == (i % 12)){
      printf("\n");
    }
  }
  printf("\n");

  fclose(outFile);
  free(writeBuffer);
  free(outName);
}

void writeConvertedContent(char * writeData){

}

void askFileName(char * fileName){
  char i; //used as index
  char input[FILE_NAME_SIZE + 10];

  do{
    i = 0;
    printf("[IMPORTANT!] in current directory the file to be created will overwrite existing files with same name\n");
    printf("Give filename for output file without the file type extension> ");
    scanf("%s", input);
    /*check length of input variable*/
    while (input[i] != '\0'){
      i++;
    }

    if (i > FILE_NAME_SIZE){
      printf("\nGiven filename is too long. Maximum length for filename is %d\n\n", FILE_NAME_SIZE);
    }
  }while (i > FILE_NAME_SIZE);

  /*Copy given filename to fileName variable*/
  i = 0;
  while (input[i] != '\0'){
    fileName[i] = input[i];
    i++;
  }

  fileName[i] = '.'; fileName[i + 1] = 'b';
  fileName[i + 2] = 'i'; fileName[i + 3] = 'n';
  fileName[i + 4] = '\0';
}

void createWriteBuffer(char * buffer){
  /* convert row of characters to binary string */
}
