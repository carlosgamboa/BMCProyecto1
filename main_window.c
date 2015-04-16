#include "support.h"

#define UI_FILE "main_window.glade"

void create_list(ChData *data) {
  GtkListStore *list_store;

  list_store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_DOUBLE);

  gtk_tree_view_set_model(GTK_TREE_VIEW(data->tree_view), GTK_TREE_MODEL(list_store));
}

int main(int argc, char *argv[]) {

  ChData *data;
  GtkBuilder *builder;
  GError *error = NULL;

  gtk_init(&argc, &argv);
  builder = gtk_builder_new();

  if (!gtk_builder_add_from_file(builder, UI_FILE, &error)) {
    g_warning("%s", error->message);
    g_free(error);
    return(1);
  }

  data = g_slice_new(ChData);
  data->matrix = g_slice_new(GenMatrix);
  data->matrix->id = 2;
  data->matrix->column_array_size = 0;
  data->matrix->matrix_size = 0;
  /* Get object from UI */
#define GW(name) CH_GET_WIDGET(builder, name, data)
  GW(main_window);
  GW(tree_view);
  GW(btn_add);
  GW(txt_gene1);
  GW(txt_gene2);
  GW(sp_frecuency);
#undef GW

  gtk_builder_connect_signals(builder, data); 
  g_object_unref(G_OBJECT(builder));

  gtk_widget_show(data->main_window);
  create_list(data);

  gtk_main();

  g_slice_free(ChData, data);

  return 0;
}
