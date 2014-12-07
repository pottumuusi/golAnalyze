/*
 * This program takes names of text files containing game of life patterns as command
 * line arguments and converts them so that they can be used by the test programs and
 * maybe even the final program. Who knows.
 */

#include <stdio.h>

int checkFileContent(FILE *);

int main(int argc, char * argv[]){
  FILE * gridFile;
  
  gridFile = fopen(argv[1], "r");
  
  return 0;
}

int checkFileContent(FILE * gridFile){
  /*check that file contains a grid filled with '-' and '*' characters*/
  
  return 0;
}