/*
 * main.c
 *
 *  Created on: 29 mar 2018
 *      Author: majsterklepka
 */

#include "iban_val.h"

void app_init(App *app)
{
	app->builder = gtk_builder_new();

	gtk_builder_add_from_file(app->builder, "interface.glade", NULL);

	app->objects = gtk_builder_get_objects(app->builder);
	gtk_builder_connect_signals(app->builder, app);

}

GObject *get_ui_element(App *app, const gchar *name)
{

	const gchar *widget;
	GSList *list;

	list = app->objects;

	do{
		widget = gtk_buildable_get_name(list->data);
		if (g_strcmp0(widget, name) == 0){
			return list->data;
		}
	}while((list = g_slist_next(list)));

	g_slist_free(list);

	return NULL;
}

void close_activate(GtkWidget * widget, App *app)
{
	g_application_quit(G_APPLICATION(app->application));
}

static void activate(GApplication *application, App *app) {

	UI_ELEMENT(GtkWindow, window);

	gtk_application_add_window(GTK_APPLICATION(app->application), window);
	gtk_widget_show_all(GTK_WIDGET(window));

}

int main(int argc, char **argv) {
	int status;
	App *app;
	app = (App*) g_new(App, 1);


	app->application = gtk_application_new("io.github.majsterklepka", G_APPLICATION_FLAGS_NONE);
	g_application_register(G_APPLICATION(app->application), NULL, NULL);

	app_init(app);

	g_signal_connect(app->application, "activate", G_CALLBACK (activate), app);
	status = g_application_run(G_APPLICATION(app->application), argc, argv);

	g_object_unref(app->application);

	return status;
}
