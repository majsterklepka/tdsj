/*
 * main.c
 *
 *  Created on: 29 mar 2018
 *      Author: majsterklepka
 */

#include "iban_val.h"




GObject *
app_get_ui_element(App *app, const gchar *name)
{

	const gchar *s;
	GSList *list;

	list = app->object;
	do{
		s = gtk_buildable_get_name(list->data);
		if (g_strcmp0(s, name) == 0){
			return list->data;
		}
	}while((list = g_slist_next(list)));
	g_slist_free(list);
	return NULL;
}

void
app_init(App *app)
{
	GError *error = NULL;

	app->builder = gtk_builder_new();
	gtk_builder_add_from_file(app->builder, "interface.glade", &error);

	if(error != NULL){
		g_printerr("Error while loading app definitions file: %s\n", error->message);
		g_error_free(error);
		gtk_main_quit();
	}
	gtk_builder_connect_signals(app->builder, app);
	app->object = gtk_builder_get_objects(app->builder);
}


void close_activate(GtkWidget *widget, App *app)
{
	gtk_main_quit();
}


int main(int argc, char *argv[])
{
	App *app;
 	app = (App *) g_new(App, 1);

 	gtk_init(&argc, &argv);

 	app_init(app);



 	GET_UI_ELEMENT(GtkWindow, window);
 	gtk_widget_show_all(GTK_WIDGET(window));

  	gtk_main();
	return 0;

}
