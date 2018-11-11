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
gboolean user_edited_current_document;
gchar *existing_filename;

static void text_buffer_changed(GtkTextBuffer *buffer, gpointer user_data)
{
	user_edited_current_document = TRUE;
}

static void save_to_file(const gchar *filename)
{
	GtkTextBuffer *textbuffer;
	GtkTextIter start, end;
	textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
	gtk_text_buffer_get_start_iter(textbuffer, &start);
	gtk_text_buffer_get_end_iter(textbuffer, &end);
	const gchar *contents = gtk_text_buffer_get_text(textbuffer, &start, &end, TRUE);
	g_file_set_contents(filename, contents, -1, NULL);
}


static void open_file(const gchar *filename)
{
	GtkTextBuffer *textbuffer;
	textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gchar *contents = NULL;
	g_file_get_contents(filename, &contents, NULL, NULL);
	gtk_text_buffer_set_text(textbuffer, contents, -1);
	gtk_text_buffer_set_modified(textbuffer, FALSE);
	user_edited_current_document = FALSE;
	g_signal_connect(textbuffer, "modified-changed", G_CALLBACK(text_buffer_changed), NULL);
	g_free(contents);
}



static void set_title(const gchar *filename)
{
	gchar *name = g_strdup_printf("%s: %s", g_get_application_name(), g_filename_display_basename(filename));
	gtk_window_set_title(GTK_WINDOW(window), name); 
}

static void save_as(GtkButton *button, gpointer user_data)
{
GtkWidget *dialog;
GtkFileChooser *chooser;
GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
gint res;

dialog = gtk_file_chooser_dialog_new ("Zapisz plik...",
                                      GTK_WINDOW(window),
                                      action,
                                      "_Porzuć",
                                      GTK_RESPONSE_CANCEL,
                                      "_Zapisz",
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);
chooser = GTK_FILE_CHOOSER (dialog);

gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);

if (user_edited_current_document)
  gtk_file_chooser_set_current_name (chooser,
                                     "Dokument bez tytułu");
else
  gtk_file_chooser_set_filename (chooser,
                                 existing_filename);

res = gtk_dialog_run (GTK_DIALOG (dialog));
if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;

    filename = gtk_file_chooser_get_filename (chooser);
	save_to_file (filename);
	set_title(filename);
	g_free (filename);
  }

gtk_widget_destroy (dialog);
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
	existing_filename = filename;
	set_title(filename);
    g_free (filename);
  }

gtk_widget_destroy (dialog);
}



int main(int argc, char **argv)
{
    GtkWidget *button;
    GtkWidget *button1;
    GtkWidget *box;
    GtkWidget *scrolled;
    GtkWidget *buttonbox;

    gtk_init(&argc, &argv);
	
    existing_filename = "Dokument bez tytułu";
    user_edited_current_document = FALSE;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
    gtk_window_set_default_icon_name("gedit");

    button = gtk_button_new_with_label("Otwórz Plik");
    button1 = gtk_button_new_with_label("Zapisz jako...");

    g_signal_connect(button, "clicked", G_CALLBACK(file_open), NULL);
    g_signal_connect(button1, "clicked", G_CALLBACK(save_as), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);

    gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonbox), GTK_BUTTONBOX_START);		

    gtk_box_pack_start(GTK_BOX(buttonbox), button, FALSE, FALSE, 2);
	gtk_box_pack_end(GTK_BOX(buttonbox), button1, FALSE, FALSE, 2);
    gtk_box_pack_start(GTK_BOX(box), buttonbox, FALSE, FALSE, 2);

	textview = gtk_text_view_new();
	
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), TRUE);

    scrolled = gtk_scrolled_window_new(NULL, NULL);

    gtk_container_add(GTK_CONTAINER(scrolled), textview);		
    gtk_box_pack_end(GTK_BOX(box), scrolled, 1, 1, 2);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(box));

	g_set_application_name("GtkFileChooserDialog Example");

    gtk_widget_show_all(window);

    gtk_main();
    
    return 0;
}

