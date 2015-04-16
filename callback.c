#include "support.h"
#include <stdio.h>
#include <stdlib.h>

enum {
  COL_FIRST_GENE = 0,
  COL_SECOUND_GENE,
  COL_FRECUENCY,
  N_COLUMNS
};

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

int get_index_column(const char *column, char column_array[255][255], int array_size) {
  int i;
  int column_index = 0;
  for(i = 0; i < array_size; i++) {
    if(strcmp(column_array[i], column) == 0) {
      column_index = i;
    }
  }

  return column_index;
}

void include_new_row(ChData *data) {
  GtkTreeModel *model;
  GtkListStore *list_store;
  GtkTreeIter iter;
  GtkTreeView *tree_view;

  const char *gene1, *gene2;
  double frecuency_value;
  int column_count, gene1_column_index, gene2_column_index;
  double *frecuency_value_in_array;
  int frecuency_already_added = -1;
  double **values = data->matrix->values;

  gene1 = gtk_entry_get_text(GTK_ENTRY(data->txt_gene1));
  gene2 = gtk_entry_get_text(GTK_ENTRY(data->txt_gene2));
  frecuency_value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(data->sp_frecuency));
  column_count = data->matrix->column_array_size;

  gene1_column_index = get_index_column(gene1, data->matrix->columns_array, column_count);
  gene2_column_index = get_index_column(gene2, data->matrix->columns_array, column_count);
  frecuency_value_in_array = data->matrix->values[gene1_column_index, gene2_column_index];

  g_print("column %i row %i recuency value %lf\n", gene1_column_index, gene2_column_index, frecuency_value);
  if (0 == 0) {
    if (is_column_already_in_buffer(gene1, data->matrix->columns_array, column_count) == 0) {
        strcpy(data->matrix->columns_array[column_count], gene1);
        column_count = column_count + 1;

        data->matrix->column_array_size = column_count;
      }

      if (is_column_already_in_buffer(gene2, data->matrix->columns_array, column_count) == 0) {
        strcpy(data->matrix->columns_array[column_count], gene2);
        column_count = column_count + 1;

        data->matrix->column_array_size = column_count;
      }

      gene1_column_index = get_index_column(gene1, data->matrix->columns_array, column_count);
      gene2_column_index = get_index_column(gene2, data->matrix->columns_array, column_count);
      values[gene1_column_index][gene2_column_index] = frecuency_value;
      g_print("values of matrix %lf\n", values[0][0]);
      data->matrix->matrix_size = data->matrix->matrix_size + 1;

      tree_view = GTK_TREE_VIEW(data->tree_view);
      model = gtk_tree_view_get_model(tree_view);
      list_store = GTK_LIST_STORE(model);

      gtk_list_store_append(list_store, &iter);

      gtk_list_store_set(list_store, &iter,
        COL_FIRST_GENE, gene1,
        COL_SECOUND_GENE, gene2,
        COL_FRECUENCY, frecuency_value, -1);
  }
}

void btn_add_clicked(GtkButton *button, ChData *data) {
  //GtkTreeModel *model;

  //model = gtk_tree_view_get_model(GTK_TREE_VIEW(data->tree_view));

  //gtk_tree_model_foreach(model, (GtkTreeModelForeachFunc)foreach_func, NULL);
  include_new_row(data);
}
