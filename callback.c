#include "support.h"
#include "file_support.h"
#include <stdio.h>
#include <stdlib.h>

enum {
  COL_FIRST_GENE = 0,
  COL_SECOUND_GENE,
  COL_FRECUENCY,
  N_COLUMNS
};

int column_array_size = 0;
int matrix_size = 0;
char columns_array[255][255];
double values[255][255];

gboolean foreach_func(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, ChData *data) {
  gchar *gen1, *gen2, *tree_view_path;
  guint frecuency;

  gtk_tree_model_get(
    model,
    iter,
    COL_FIRST_GENE, &gen1,
    COL_SECOUND_GENE, &gen2,
    COL_FRECUENCY, &frecuency
  );

  tree_view_path = gtk_tree_path_to_string(path);
  g_print("Row %s: gen1 %s - gen2 %s, frecuency %i\n", tree_view_path, gen1 , gen2, frecuency);

  g_free(tree_view_path);
  g_free(gen1);
  g_free(gen2);

  return FALSE;
}

void init_matrix() {
  int i,j;
  matrix_size = 0;
  for(i = 0; i < 255; i++) {
    for(j = 0; j < 255; j++) {
        values[i][j] = -1;
    }
  }
}

void init_columns_array() {
  int i;
  column_array_size = 0;
  for(i = 0; i < 255; i++) {
    strcpy(columns_array[i], "");
  }
}

void read_values_from_file(char file_name[]) {
  char file_data[1024*6] = "";
  int index = 0;
  int section_delimiter = 0;
  int init_string_index = 0;
  int finish_string_index = 0;
  int current_row = 0;
  int current_column = 0;

  init_matrix();
  init_columns_array();

  strcpy(file_data, get_file_data(file_name));

  while(file_data[index] != '\0') {
    if (file_data[index] == '/') {
      init_string_index++;
      section_delimiter++;
    }

    if (section_delimiter == 1) {
      if(file_data[index] == ',') {
        char column[255] = "";
        finish_string_index = index - init_string_index;
        strncpy(column, file_data+init_string_index, finish_string_index);
        init_string_index = index + 1;
        strcpy(columns_array[column_array_size], column);
        column_array_size++;
      }
    }

    if (section_delimiter == 2) {
      if(file_data[index] == '!') {
        current_row++;
        current_column = 0;
        init_string_index++;
      }

      if (file_data[index] == '|') {
        char cell[255] = "";
        double cell_value = 0.0;
        current_column++;
        finish_string_index = index - init_string_index;
        strncpy(cell, file_data+init_string_index, finish_string_index);
        init_string_index = index + 1;
        cell_value = atof(cell);
        values[current_row - 1][current_column - 1] = atof(cell);
        matrix_size++;
      }
    }
    index++;
  }
}

int is_column_already_in_buffer(const char *column, char columns_array[255][255], int array_size){
  int i;
  int alredy_in_buffer = 0;
  for(i = 0; i < array_size; i++) {
    if(strcmp(columns_array[i], column) == 0) {
      alredy_in_buffer = 1;
    }
  }

  return alredy_in_buffer;
}

char *columns_to_string() {
  char i;
  char file_string[1024] = "";

  for(i = 0; i < column_array_size; i++) {
    strcat(file_string, columns_array[i]);
    strcat(file_string, ",");
  }

  return file_string;
}

char *matrix_to_string() {
  int i,j;
  char file_string[1024*6] = "";

  for(i = 0; i < column_array_size; i++) {
    if (i == 0) {
      strcat(file_string, "!");
    }
    for(j = 0; j < column_array_size; j++) {

      if (j == i) {
        strcat(file_string, "-1");
      } else {
        char double_value[4];
        strcpy(double_value, double_to_string(values[i][j]));
        strcat(file_string, double_value);
      }
      strcat(file_string, "|");
    }
    if (i < column_array_size) {
      strcat(file_string, "!");
    }
  }

  return file_string;
}

int get_index_column(const char *column, char column_array[255][255], int array_size) {
  int i;
  int column_index = -1;
  for(i = 0; i < array_size; i++) {
    if(strcmp(column_array[i], column) == 0) {
      column_index = i;
    }
  }

  return column_index;
}

int write_matrix_file(char file_name[]) {
  char file_string[1024*6] = "";
  strcpy(file_string, "/");
  strcat(file_string, columns_to_string());
  strcat(file_string, "/");
  strcat(file_string, matrix_to_string());
  strcat(file_string, "/");
  write_file(file_string, file_name);
}

void include_new_row(ChData *data) {
  GtkTreeModel *model;
  GtkListStore *list_store;
  GtkTreeIter iter;
  GtkTreeView *tree_view;

  const char *gene1, *gene2;
  double frecuency_value;
  int column_count, gene1_column_index, gene2_column_index;
  double frecuency_value_in_array = 0.00;
  int frecuency_already_added = -1;

  gene1 = gtk_entry_get_text(GTK_ENTRY(data->txt_gene1));
  gene2 = gtk_entry_get_text(GTK_ENTRY(data->txt_gene2));
  frecuency_value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(data->sp_frecuency));

  gene1_column_index = get_index_column(gene1, columns_array, column_array_size);
  gene2_column_index = get_index_column(gene2, columns_array, column_array_size);
  if (gene1_column_index >= 0 && gene2_column_index >= 0) {
    frecuency_value_in_array = values[gene1_column_index][gene2_column_index];
  }

  if (frecuency_value_in_array <= 0) {
    if (is_column_already_in_buffer(gene1, columns_array, column_array_size) == 0) {
        strcpy(columns_array[column_array_size], gene1);
        column_array_size = column_array_size + 1;
      }

      if (is_column_already_in_buffer(gene2, columns_array, column_array_size) == 0) {
        strcpy(columns_array[column_array_size], gene2);
        column_array_size = column_array_size + 1;
      }

      gene1_column_index = get_index_column(gene1, columns_array, column_array_size);
      gene2_column_index = get_index_column(gene2, columns_array, column_array_size);

      values[gene1_column_index][gene2_column_index] = frecuency_value;
      values[gene2_column_index][gene1_column_index] = frecuency_value;
      matrix_size = matrix_size + 1;

      tree_view = GTK_TREE_VIEW(data->tree_view);
      model = gtk_tree_view_get_model(tree_view);
      list_store = GTK_LIST_STORE(model);

      gtk_list_store_append(list_store, &iter);

      gtk_list_store_set(list_store, &iter,
        COL_FIRST_GENE, gene1,
        COL_SECOUND_GENE, gene2,
        COL_FRECUENCY, frecuency_value, -1);
    }
  //write_file(columns_to_string(columns_array), "new_value.bmc");
  //write_file(matrix_to_string(), "new_value.bmc");
  //write_matrix_in_file("new_value.txt", columns_array, values, column_array_size);
}

void btn_add_clicked(GtkButton *button, ChData *data) {
  //GtkTreeModel *model;

  //model = gtk_tree_view_get_model(GTK_TREE_VIEW(data->tree_view));

  //gtk_tree_model_foreach(model, (GtkTreeModelForeachFunc)foreach_func, NULL);
  include_new_row(data);
}

void btnmi_save_activate_cb(ChData *data) {
  write_matrix_file("new_values.bmc");
}

void btnmi_open_activate_cb(ChData *data) {
  read_values_from_file("new_values.bmc");
}
