/*
=========================================================================
file:		file-1.c
Author:		Paweł Sobótka <48721262935pl@gmail.com>
License:	GNU GPLv3 or later
Copyright:	©2018 Paweł Sobótka all rights reserved
Comment:	to compile just type `make`, to run just type ./file-1
Description:	GtkFileChooserDialog example written in C
=========================================================================
*/

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

GtkWidget *window;
GtkWidget *textview;
gchar *namefile;

static void set_title(const gchar *filename)
{
	gchar *name = g_strdup_printf("%s: %s", g_get_application_name(), g_filename_display_basename(filename));
	gtk_window_set_title(GTK_WINDOW(window), name); 
}


static void open_file(const gchar *filename)
{
	GtkTextBuffer *textbuffer;
	textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
	gchar *contents = NULL;
	g_file_get_contents(filename, &contents, NULL, NULL);
	gtk_text_buffer_set_text(textbuffer, contents, -1);
	g_free(contents);
}


static void file_open(GtkButton *button, gpointer user_data)
{ 
GtkWidget *dialog;
GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
gint res;

dialog = gtk_file_chooser_dialog_new ("Otwórz plik",
                                      GTK_WINDOW(window),
                                      action,
                                      "_Porzuć",
                                      GTK_RESPONSE_CANCEL,
                                      "_Otwórz",
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

res = gtk_dialog_run (GTK_DIALOG (dialog));
if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    filename = gtk_file_chooser_get_filename (chooser);
    open_file (filename);
    set_title(filename);
    namefile = filename;
    g_free (filename);
  }

gtk_widget_destroy (dialog);
}

int main(int argc, char **argv)
{
    GtkWidget *button;
    GtkWidget *box;
    GtkWidget *scrolled;
    GtkWidget *buttonbox;	

    gtk_init(&argc, &argv);
  
    namefile = NULL;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

    button = gtk_button_new_with_label("Otwórz Plik");

    g_signal_connect(button, "clicked", G_CALLBACK(file_open), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

    gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonbox), GTK_BUTTONBOX_START);		

    gtk_box_pack_start(GTK_BOX(buttonbox), button, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(box), buttonbox, FALSE, FALSE, 2);

    textview = gtk_text_view_new();

    scrolled = gtk_scrolled_window_new(NULL, NULL);

    gtk_container_add(GTK_CONTAINER(scrolled), textview);		
    gtk_box_pack_end(GTK_BOX(box), scrolled, 1, 1, 2);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(box));

	g_set_application_name("GtkFileChooserDialog Example");

    gtk_widget_show_all(window);

    gtk_main();
    
    return 0;
}

