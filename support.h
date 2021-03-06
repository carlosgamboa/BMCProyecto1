#ifndef _SUPPORT_H_
#define _SUPPORT_H_

#include <gtk/gtk.h>

/* Macros para obtener los objetos del UI file */
#define CH_GET_OBJECT(builder, name, type, data) \
  data->name = type(gtk_builder_get_object(builder, #name))
#define CH_GET_WIDGET(builder, name, data) \
  CH_GET_OBJECT(builder, name, GTK_WIDGET, data)

#define ARRAY_SIZE(a) ROWS_NUM(a)
#define ROWS_NUM(a) (sizeof(a) / sizeof(a[0]))
#define COL_NUM(a) (sizeof(a[0]) / sizeof(a[0][0]))

/* Main data structure definition */

typedef struct _GenMatrix GenMatrix;


struct _GenMatrix {
  int id;
  int column_array_size;
  int matrix_size;
  char columns_array[255][255];
  double values[255][255];
};

typedef struct _ChData ChData;

struct _ChData {
  int id;
  GtkWidget *main_window;
  GtkWidget *tree_view;
  GtkWidget *btn_add;
  GtkWidget *txt_gene1;
  GtkWidget *txt_gene2;
  GtkWidget *sp_frecuency;
  GtkWidget *btn_save;
  GtkWidget *btn_open;
  GenMatrix *matrix;
  GtkWidget *drawingarea;
  GtkWidget *btn_zoomReset;
  GtkWidget *btn_zoomIn;
  GtkWidget *btn_zoomOut;
	GtkWidget *btn_up;
	GtkWidget *btn_down;
	GtkWidget *btn_left;
	GtkWidget *btn_right;
  GtkWidget *statusbar1;
	GtkWidget *btn_generate;
};


typedef struct _Color Color;
struct _Color
{
    float r;
    float g;
    float b;
};
typedef struct _GeneData GeneData;
struct _GeneData {
    char name[12];
    int distance;
    Color color;
};

void FillChromosomes();

#endif

