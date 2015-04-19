#include <gtk/gtk.h>
#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "support.h"

#define UI_FILE "new_main.glade"

//static void draw_rectangle(cairo_t *, float xpos, float ypos, float size, const char* name, Color genColor);
void FillChromosomes()
{
    int i = 0;
    for  (i = 0; i < 100; i++)
    {
        char str[12];
        int rnd = 20 + (rand() % 30);
        float double_rnd = (float)rnd/100.0f;
        sprintf(str, "gen%d %1.2f", i, double_rnd);

        GeneData temp;
        strcpy(temp.name, str);
        temp.distance = rnd;
        temp.color = getColor();
        genes[i] = temp;
    }

}

void CreateDrawWindow()
{
    DrData *data;
    GtkBuilder *builder;
    GError     *error = NULL;
    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if( ! gtk_builder_add_from_file( builder, "drawing_window.glade", &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }

    data = g_slice_new(DrData);

#define GW(name) CH_GET_WIDGET(builder, name, data)
    //GW(result_window);
    GW(drawingarea);
    GW(btn_zoomReset);
    GW(btn_zoomIn);
    GW(btn_zoomOut);
#undef GW

    /* Connect signals */
    gtk_builder_connect_signals( builder, data );

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );


    /* Show window. All other widgets are automatically shown by GtkBuilder */
    //gtk_widget_show( data->result_window );

    /* Start main loop */
    //gtk_main();

    g_slice_free(DrData, data);
}

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
    CreateDrawWindow();

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
  GW(btnmi_save);
  GW(btnmi_open);
#undef GW

  gtk_builder_connect_signals(builder, data);
  g_object_unref(G_OBJECT(builder));

  gtk_widget_show(data->main_window);
  create_list(data);


  gtk_main();
  g_slice_free(ChData, data);

  return 0;
}
