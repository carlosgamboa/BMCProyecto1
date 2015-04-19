#include "support.h"
#include "file_support.h"
#include "algorithm.h"
#include <stdio.h>
#include <stdlib.h>

enum {
  COL_FIRST_GENE = 0,
  COL_SECOUND_GENE,
  COL_FRECUENCY,
  N_COLUMNS
};

#define SCREEN_MOVE 20

float scale = 1.0;
float xtranslate, ytranslate = 0;
GeneData genes[100];

int column_array_size = 0;
int matrix_size = 0;
char columns_array[255][255];
double values[255][255];
Color colors[255][255];
int draw = 0;


//Declarations
static void draw_rectangle(cairo_t *cr, float xpos, float ypos, float size, const char* name, Color genColor);
void setColors();
Color getColor();

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
        current_column++;
        finish_string_index = index - init_string_index;
        strncpy(cell, file_data+init_string_index, finish_string_index);
        init_string_index = index + 1;
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
        // char double_value[32];
        sprintf(file_string, "%s%f", file_string, values[i][j]);
        // strcat(file_string, double_to_string();
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
  int gene1_column_index, gene2_column_index;
  double frecuency_value_in_array = 0.00;

  if (column_array_size == 0){
    init_matrix();
  }

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
}

void setColors()
{
	int i, j = 0;
	for (i = 0; i < column_array_size; i++)
	{
		if(path[i][j] == -1)
			break;

		for (j = 0; j < column_array_size; j++)
		{
			colors[i][j] = getColor();
		}
	}
}

void on_generate_clicked(GtkButton *button, ChData *data) {
	g_print("generating");
	printMatrix_double(values, column_array_size);
	create_all_maps(values, column_array_size);
	draw = 1;
	setColors();
	Refreh (data);
}

void btn_add_clicked(GtkButton *button, ChData *data) {
  guint context_id;
  GtkStatusbar *status_bar;

  status_bar = GTK_STATUSBAR(data->statusbar1);
  context_id = gtk_statusbar_get_context_id(status_bar, "GENES:");
  include_new_row(data);

  gtk_statusbar_remove_all(status_bar, context_id);
  gtk_statusbar_push(status_bar, context_id, "FRECUENCY ADDED");
}

void btn_save_clicked_cb(GtkButton *button, ChData *user_data) {
  guint context_id;
  GtkStatusbar *status_bar;
  GtkWidget *dialog;
  GtkWindow *window;

  status_bar = GTK_STATUSBAR(user_data->statusbar1);
  window = GTK_WINDOW(user_data->main_window);
  context_id = gtk_statusbar_get_context_id(status_bar, "GENES:");
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Save File", window, GTK_FILE_CHOOSER_ACTION_SAVE,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    filename = gtk_file_chooser_get_filename (chooser);
    strcat(filename, ".bmc");
    write_matrix_file(filename);
    g_print("BMC CONSOLE OUTPUT : FILE SAVED %s\n", filename);
    gtk_statusbar_remove_all(status_bar, context_id);
    gtk_statusbar_push(status_bar, context_id, "FILE SAVED");
  }

  gtk_widget_destroy (dialog);
}

void btn_open_clicked_cb(GtkButton *button, ChData *app) {
  gint res;
  GtkWindow *window;
  GtkWidget *dialog;

  window = GTK_WINDOW(app->main_window);
  dialog = gtk_file_chooser_dialog_new ("Open File", window, GTK_FILE_CHOOSER_ACTION_OPEN,
    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    filename = gtk_file_chooser_get_filename (chooser);
    read_values_from_file(filename);
    g_print("BMC CONSOLE OUTPUT : FILE OPENED %s\n", filename);
    GtkTreeModel *model;
    GtkListStore *list_store;
    GtkTreeIter iter;
    GtkTreeView *tree_view;
    guint context_id;
    GtkStatusbar *status_bar;

    double inserted_values[255][255];
    int i,j;
    status_bar = GTK_STATUSBAR(app->statusbar1);
    context_id = gtk_statusbar_get_context_id(status_bar, "GENES:");
    tree_view = GTK_TREE_VIEW(app->tree_view);
    model = gtk_tree_view_get_model(tree_view);
    list_store = GTK_LIST_STORE(model);
    gtk_list_store_clear(list_store);

    for(i = 0; i< column_array_size; i++) {
     for(j = 0; j < column_array_size; j++) {
       if (i != j && inserted_values[i][j] != values[j][i] && values[i][j] > 0) {
         gtk_list_store_append (list_store, &iter);
         gtk_list_store_set (list_store, &iter,
           COL_FIRST_GENE, columns_array[i],
           COL_SECOUND_GENE, columns_array[j],
           COL_FRECUENCY, values[i][j],
           -1
         );
         inserted_values[i][j] = values[i][j];
         inserted_values[j][i] = values[j][i];
       }
     }
    }
    gtk_statusbar_remove_all(status_bar, context_id);
    gtk_statusbar_push(status_bar, context_id, "FILE OPENED");
    g_free (filename);
  }

  gtk_widget_destroy (dialog);
}

/***************************************************************/

float randomFloat()
{
      float r = (float)rand() / (float)RAND_MAX;
      return r;
}

Color getColor()
{
    Color color;
    color.r = randomFloat();
    color.g = randomFloat();
    color.b = randomFloat();
    return color;
}

void Refreh(ChData *data)
{
    gtk_widget_queue_draw (data->drawingarea);
}

void on_button_zoomReset_clicked ( GtkButton *button , ChData *data)
{
    scale = 1;
    xtranslate = ytranslate = 0;
    Refreh(data);
}

void on_button_zoomIn_clicked ( GtkButton *button , ChData *data)
{
    scale += 0.1;
    Refreh(data);
}

void on_button_zoomOut_clicked ( GtkButton *button , ChData *data)
{
    scale -= 0.1;
    if (scale < 0)
        scale = 0.1;
    Refreh(data);
}

void on_left_event ( GtkButton *button , ChData *data)
{
	xtranslate += SCREEN_MOVE;
	Refreh(data);
}

void on_right_event ( GtkButton *button , ChData *data)
{
    xtranslate -= SCREEN_MOVE;
	Refreh(data);
}

void on_up_event ( GtkButton *button , ChData *data)
{
    ytranslate += SCREEN_MOVE;
	Refreh(data);
}

void on_down_event ( GtkButton *button , ChData *data)
{
    ytranslate -= SCREEN_MOVE;
    Refreh(data);
}

gboolean draw_cb (GtkWidget *widget, GdkEventExpose *event)
{
    cairo_t *cr;
    cr = gdk_cairo_create (gtk_widget_get_window (widget));
    cairo_scale(cr, scale, scale);
    cairo_translate(cr, xtranslate, ytranslate);
    int x, y;
    x = 30;
    y = 20;

	/*
    for (i = 1; i < 100; i++)
    {
        //char str[5];
        //sprintf(str, "Gen%d", i);
        //int rnd = 20 + (rand() % 30);
        int rnd = genes[i].distance;

        draw_rectangle(cr, x, y, rnd, genes[i].name, genes[i].color);
        y = y + rnd;

        if (y > 700*(2-scale))
        {
            y = 20;
            x = x + 140;
        }
    }*/

	if (draw == 0   )
		return;
	
	//final draw
	int i, j = 0, size;
	for (i = 0; i < column_array_size; i++)
	{
		if(path[i][j] == -1)
			break;

		for (j = 0; j < column_array_size; j++)
		{
			if(path[i][j] != -1)
			{
				char str[5], final[12];
   				
				strcpy(str, columns_array[path[i][j]]);
				size = (int)(cost[i][j] * 1000);
				if (size < 0)
					size = 0;

				if (cost[i][j] < 0)
					sprintf(final, "%s", str);
				else
					sprintf(final, "%s %2.2f", str, cost[i][j]);
				
				//g_print("%s %d \n", final, size);
				draw_rectangle(cr, x, y, size, final, colors[i][j]);
			}
			else 
				break;

			y = y + size;
		}

		y = y + 100;
		if (y > 700*(2-scale))
        {
            y = 20;
            x = x + 140;
        }
	}
	
    return FALSE;
}

static void draw_rectangle(cairo_t *cr, float xpos, float ypos, float size, const char* name, Color genColor)
{

    cairo_set_source_rgb(cr, genColor.r, genColor.g, genColor.b);
    cairo_set_line_width(cr, 1);

    cairo_rectangle(cr, xpos, ypos, 10, size);
    cairo_stroke_preserve(cr);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, xpos-10, ypos);
    cairo_line_to(cr, xpos+20, ypos);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_select_font_face(cr, "Courier",
    CAIRO_FONT_SLANT_NORMAL,
    CAIRO_FONT_WEIGHT_BOLD);

    cairo_set_font_size(cr, 11);

    cairo_move_to(cr, xpos+30, ypos+5);
    cairo_show_text(cr, name);
}

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
