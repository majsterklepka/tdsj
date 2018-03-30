/*
 * main.c
 *
 *  Created on: 29 mar 2018
 *      Author: majsterklepka
 */

#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <locale.h>


static void activate(GtkApplication *app, gpointer user_data)
{
	GtkBuilder *builder;
	GtkWidget *window;
	GError *error = NULL;

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "interface.glade", &error);
	if(error)
	{
		g_printerr("%s\n", error->message);
		g_error_free(error);
	}
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
	gtk_application_add_window(app, GTK_WINDOW(window));
	gtk_builder_connect_signals(builder, NULL);
	gtk_widget_show_all(window);



}

int main(int argc, char *argv[])
{
	GtkApplication *app;
	int status;

	app = gtk_application_new("io.github.majsterklepka", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;

}
