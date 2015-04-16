#include <jansson.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024

char *get_file_data(char file_location[]) {
  static char buffer[BUFFER_SIZE];
  FILE *fileptr;
  int file_char;

  fileptr = fopen(file_location,"r");
  fread(buffer, BUFFER_SIZE, 1, fileptr);

  fclose(fileptr);

  return buffer;
}

int write_file(char file_data[], char file_location[]) {
  FILE *fileptr;

  fileptr = fopen(file_location, "w");
  fwrite(file_data, 1, BUFFER_SIZE, fileptr);

  fclose(fileptr);

  return(1);
}

json_t *get_json_obj(char file_location[]) {
  char *json_data;
  json_t *root;
  json_error_t error;

  json_data = get_file_data(file_location);
  root = json_loads(json_data, 0, &error);

  return root;
}

void get_json_array_values(json_t *json, size_t col_size, char string_array[col_size][255]) {
  json_t *data, *inner_json;
  int i;

  for(i = 0; i < json_array_size(json) && (inner_json = json_array_get(json, i)); i++) {
    char *string_section;

    string_section = strndup(json_string_value(inner_json), BUFFER_SIZE);
    strcpy(string_array[i], json_string_value(inner_json));
  }
}

char *get_json_root_data(json_t *json, char key[]) {
  json_t *data;
  static char *value = "";
  data = json_object_get(json, key);
  value = strndup(json_string_value(data), BUFFER_SIZE);

  return value;
}

int main() {
  int col_val = 3;
  json_t *json, *array_json;
  int i = 0;

  json = get_json_obj("json_test.json");
  array_json = json_object_get(json, "col");

  size_t elements_count = json_array_size(array_json); 
  char value[elements_count][255];
  get_json_array_values(array_json, elements_count, value);

  for(i = 0; i < elements_count; i++) {
    printf("values for col: %s\n", value[i]);
  }

  return 0;
}

