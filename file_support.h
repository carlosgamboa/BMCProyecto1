#ifndef _FILE_SUPPORT_H_
#define _FILE_SUPPORT_H_

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

char *get_file_data(char file_location[]) {
  static char buffer[BUFFER_SIZE];
  FILE *fileptr;

  fileptr = fopen(file_location,"r");
  fread(buffer, BUFFER_SIZE, 1, fileptr);

  fclose(fileptr);

  return buffer;
}

int write_file(char file_data[], char file_location[]) {
  FILE *fileptr;
  int index = 0;

  while(file_data[index] != '\0') {
    index++;
  }

  fileptr = fopen(file_location, "w");
  fwrite(file_data, 1, index, fileptr);

  fclose(fileptr);

  return(1);
}

char *double_to_string(double value) {
  char string[4];
  sprintf(string, "%f", value); 
  return string;
}

void write_matrix_in_file(char file_name[], char columns[255][255], double values[255][255], int columns_size) {
  char file_string[1024*6];
  int i, j;
  strcpy(file_string, "/");
  for(i = 0; i < columns_size; i++) {
    strcat(file_string, columns[i]);
    if ( i+1 < columns_size) {
      strcat(file_string, ",");
    }
  }

  strcat(file_string,"/");

  for(i = 0; i < columns_size; i++) {
    for(j = 0; j < columns_size; i++) {
      if ( i == 0) {
       strcat(file_string, "!");
      }
      strcat(file_string, double_to_string(values[i][j]));
      if(i+1 == columns_size) {
        strcat(file_string, "!");
      } else {
        strcat(file_string, "|");
      }
    }
  }

  strcat(file_string, "/");

  write_file(file_string, file_name);
}

#endif
