/*
 * main.c
 *
 *  Created on: 29 mar 2018
 *      Author: majsterklepka
 */

#include "iban_val.h"

void close_activate(GtkWidget * widget, GApplication *app)
{
	g_application_quit(app);
}

static void activate(GApplication *app, gpointer user_data) {

	GObject *window;
	GtkBuilder *builder;

	builder = gtk_builder_new_from_file("interface.glade");
	gtk_builder_connect_signals(builder, app);

	window = gtk_builder_get_object(builder, "window");

	gtk_application_add_window(GTK_APPLICATION(app), GTK_WINDOW(window));
	gtk_widget_show_all(GTK_WIDGET(window));
	g_object_unref(builder);
}

int main(int argc, char **argv) {
	GtkApplication *app;
	int status;

	app = gtk_application_new("io.github.majsterklepka", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
