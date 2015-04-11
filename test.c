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

char **get_json_array_values(json_t *json, char key[]) {
  json_t *data;
  static char **string_array;
  int i;
  data = json_object_get(json, key);

  for(i = 0; i < json_is_array(data); i++) {
    char *string_section ="";
    json_t *section_in_json;

    section_in_json = json_array_get(data, i);
    string_section = strndup(json_string_value(section_in_json), BUFFER_SIZE);
    strcpy(string_array[i], string_section);
  }

  return string_array;
}

char *get_json_root_data(json_t *json, char key[]) {
  json_t *data;
  static char *value = "";
  data = json_object_get(json, key);
  value = strndup(json_string_value(data), BUFFER_SIZE);

  return value;
}

int main() {
  char **value;
  json_t *json;

  json = get_json_obj("json_test.json");
  value = get_json_array_values(json, "col");

  printf("json key1 value: %zu\n", sizeof(value));
  return 0;
}

