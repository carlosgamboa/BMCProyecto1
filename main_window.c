#include <gtk/gtk.h>
#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "support.h"

#define UI_FILE "new_main.glade"


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

	FillChromosomes();

  data = g_slice_new(ChData);

  /* Get object from UI */
#define GW(name) CH_GET_WIDGET(builder, name, data)
	GW(main_window);
	GW(tree_view);
	GW(btn_add);
	GW(txt_gene1);
	GW(txt_gene2);
	GW(sp_frecuency);
	GW(btn_save);
	GW(btn_open);
	GW(drawingarea);
	GW(btn_zoomReset);
	GW(btn_zoomIn);
	GW(btn_zoomOut);
	GW(btn_up);
	GW(btn_down);
	GW(btn_left);
	GW(btn_right);
  GW(statusbar1);
	GW(btn_generate);
#undef GW

  gtk_builder_connect_signals(builder, data);
  g_object_unref(G_OBJECT(builder));

  gtk_widget_show(data->main_window);
  create_list(data);


  gtk_main();
  g_slice_free(ChData, data);

  return 0;
}
