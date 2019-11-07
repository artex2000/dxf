#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cairo.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "list.h"
#include "dxf.h"

double d2r (double d)
{
    return d * M_PI / 180;
}

void draw_contour (cairo_t *cr)
{
	list_item *i;
	arc_t *arc;
	line_t *line;
        double x, y;

	i = get_first_node (&contour);
        printf ("attempt to draw contour\n");
	while (!is_end_list (&contour, i)) {
		line = (line_t *)i->data;
		if (line->type == 0) {
                    cairo_move_to (cr, line->a.x, line->a.y);
                    cairo_line_to (cr, line->b.x, line->b.y);
                } else {
			arc = (arc_t *)i->data;
                            x = arc->c.x + arc->r * cos (d2r (arc->a1));
                            y = arc->c.y + arc->r * sin (d2r (arc->a1));
                            cairo_move_to (cr, x, y);
                            if (arc->a1 > arc->a2)
                                arc->a2 += 360;
                                cairo_arc (cr, arc->c.x, arc->c.y,
                                            arc->r,
                                            d2r (arc->a1), d2r (arc->a2));
                            //else
                            /*
                                cairo_arc_negative (cr, arc->c.x, arc->c.y,
                                            arc->r,
                                            d2r (arc->a1), d2r (arc->a2));
                                            */
		}
		i = i->next;
	}
}

static gboolean
expose_event (GtkWidget *widget, GdkEventExpose *event) 
{
	cairo_t *cr;
	double cw, ch;
	int i;

	cr = gdk_cairo_create (widget->window);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width (cr, 0.5);
	cw = (double)widget->allocation.width;
	ch = (double)widget->allocation.height;
        if (ready)
            draw_contour (cr);
	cairo_stroke (cr);
	cairo_destroy (cr);
	return TRUE;
}

static void g_button_clicked (GObject *button) 
{
}

static void d_button_clicked (GObject *button, GtkWidget *canvas)
{
	gtk_widget_queue_draw (canvas);
}

static void file_select (GtkFileChooser *cr, GtkLabel *label)
{
	gchar *file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (cr));
	if (file != NULL)
		dxf_parser (file);
}

int main(int argc, char *argv[]) 
{
	GtkWidget *window;
	GtkWidget *canvas;
	GtkWidget *g_button;
	GtkWidget *d_button;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *label; 
	GtkWidget *chooser; 
	GtkFileFilter *filter;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW(window), "DXF viewer");

	hbox = gtk_hbox_new (FALSE, 10);
	label = gtk_label_new ("");

	chooser = gtk_file_chooser_button_new ("File chooser", 
			GTK_FILE_CHOOSER_ACTION_OPEN);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (chooser),
			g_get_home_dir());
	filter = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter, "DXF Files");
	gtk_file_filter_add_pattern (filter, "*.dxf");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (chooser), filter);
	d_button = gtk_button_new_with_label ("Draw");
	g_button = gtk_button_new_with_label ("G Code");

	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), chooser, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), d_button, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), g_button, TRUE, TRUE, 0);

	canvas = gtk_drawing_area_new();
	gtk_widget_set_size_request (canvas, 1200, 600);

	vbox = gtk_vbox_new (FALSE, 5);
	gtk_box_pack_start (GTK_BOX (vbox), canvas, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);


	gtk_container_add (GTK_CONTAINER (window), vbox);

	g_signal_connect(window, "destroy",
		G_CALLBACK(gtk_main_quit), NULL);  

	g_signal_connect(G_OBJECT (canvas), "expose_event",
		G_CALLBACK(expose_event), NULL);  

	g_signal_connect(G_OBJECT (g_button), "clicked",
		G_CALLBACK(g_button_clicked), NULL);  

	g_signal_connect(G_OBJECT (d_button), "clicked",
		G_CALLBACK(d_button_clicked), canvas);  

	g_signal_connect(G_OBJECT (chooser), "selection_changed",
		G_CALLBACK(file_select), label);  

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
