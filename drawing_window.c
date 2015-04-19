/*
 * Compile me with:
 *   gcc -o Draw drawing_window.c support.h $(pkg-config --cflags --libs gtk+-3.0 gmodule-2.0)
 */

#include <gtk/gtk.h>
#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "support.h"
#define SCREEN_MOVE 10



float scale = 1.0;
float xtranslate, ytranslate = 0;
static void draw_rectangle(cairo_t *, float xpos, float ypos, float size, const char* name, Color genColor);
GeneData genes[100];


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

void Refreh(DrData *data)
{
    gtk_widget_queue_draw (data->drawingarea);
}

void on_button_zoomReset_clicked ( GtkButton *button , DrData *data)
{
    scale = 1;
    xtranslate = ytranslate = 0;
    Refreh(data);
}

void on_button_zoomIn_clicked ( GtkButton *button , DrData *data)
{
    scale += 0.1;
    Refreh(data);
}

void on_button_zoomOut_clicked ( GtkButton *button , DrData *data)
{
    scale -= 0.1;
    if (scale < 0)
        scale = 0.1;
    Refreh(data);
}

/*
#define GDK_KEY_Left 0xff51
#define GDK_KEY_Up 0xff52
#define GDK_KEY_Right 0xff53
#define GDK_KEY_Down 0xff54
*/

void on_key_press_event (GtkWidget *widget,
               GdkEventKey  *event,
               DrData *data)
{

    if (event->keyval == GDK_KEY_Left)
    {
        xtranslate += SCREEN_MOVE;
    }

    if (event->keyval == GDK_KEY_Right)
    {
       xtranslate -= SCREEN_MOVE;
    }

    if (event->keyval == GDK_KEY_Up)
    {
        ytranslate += SCREEN_MOVE;
    }

    if (event->keyval == GDK_KEY_Down)
    {
        ytranslate -= SCREEN_MOVE;
    }
    Refreh(data);
}

gboolean draw_cb (GtkWidget *widget, GdkEventExpose *event)
{
    cairo_t *cr;
    cr = gdk_cairo_create (gtk_widget_get_window (widget));
    cairo_scale(cr, scale, scale);
    cairo_translate(cr, xtranslate, ytranslate);
    int i, x, y;
    x = 30;
    y = 20;
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
    GW(result_window);
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
    gtk_widget_show( data->result_window );

    /* Start main loop */
    gtk_main();

    g_slice_free(DrData, data);
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    /* Init GTK+ */
    gtk_init( &argc, &argv );

    FillChromosomes();
    CreateDrawWindow();
    return( 0 );
}
