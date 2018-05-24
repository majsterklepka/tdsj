/*  main.c
    Title: GNU Hello World Example with Gtk+
    Author: Paweł Sobótka
    Date: 16-05-2018
    License: GPL-3.0
    Copyright: (C)Paweł Sobótka, 2018
*/

#include "hello.h"


int main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *label;
	gchar *text;
	gchar *title;
	gint x,y;
	GtkWidget *box;

	gtk_init(&argc, &argv);
	
	#ifdef ENABLE_NLS 
	bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
	#endif
	text = _("Hello! How are you?");
	title = _("GNU Hello World Example");
	label = gtk_label_new(text);
	
	x = 300;
	y = 250;
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), title);
	gtk_window_set_default_size(GTK_WINDOW(window), x, y);
	
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_end(GTK_BOX(box), label, 1, 1, 0);
	gtk_container_add(GTK_CONTAINER(window), box);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(window);
	
	gtk_main(); 

	return 0;
}

