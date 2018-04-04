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


int iban_validation(const char *iban)
{
	char number[64];
	int len, i, k;
	long int sum;;

	char prefix[5];
	char suffix[25];
	char output[64];

	number[0] = '\0';
	prefix[0] = '\0';
	suffix[0] = '\0';
	output[0] = '\0';

	len = 0;
	i = 0;
	k = 0;

	for ( i = 0; i < strlen(iban); i++)
	{
		if(!isspace(*(iban+ i)))
		{
			number[k] = *(iban + i);
			k++;
		}
	}

	number[k] = '\0';

	len = strlen(number);
	if ( len != 28 ){
		return 0;
	}

	for(i = 0; i < 2; i++){
		if (!isalpha(number[i]))
				return -1;
	}
	for ( i = 2; i < len; i++){
		if(!isdigit(number[i]))
			return -1;
	}

	strncpy(prefix, number, 4);
	prefix[4] = '\0';
	strncpy(suffix, number+4, 24);
	suffix[24] = '\0';

	strcat(output,suffix);
	strcat(output, prefix);
	char *p;
	div_t result;
	result = div(strtol(output, &p, 10),97);
	sum = abs(result.rem);
	if ( sum == 1)
		return 1;
	else
		return 0;

}

void show_icon(gint input, App *app)
{
	gchar *image_1 = g_build_path(G_DIR_SEPARATOR_S, "./icons", "ikona-ok-48x48.png", NULL);
	gchar *image_2 = g_build_path(G_DIR_SEPARATOR_S, "./icons", "ikona-no-ok-48x48.png", NULL);
	UI_ELEMENT(GtkImage, image1);
	if (input == 0)
	{
		gtk_widget_hide(GTK_WIDGET(image1));
	}else if( input == 1){
		GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(image_1, NULL);
				gtk_image_set_from_pixbuf(image1, pixbuf);
				gtk_widget_show(GTK_WIDGET(image1));
	}else{
		GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(image_2, NULL);
				gtk_image_set_from_pixbuf(image1, pixbuf);
				gtk_widget_show(GTK_WIDGET(image1));
	}


}

void button1_clicked_cb(GtkWidget * widget, App *app)
{
	UI_ELEMENT(GtkEntry, entry1);
	UI_ELEMENT(GtkLabel, label1);
	const gchar *text = gtk_entry_get_text(entry1);
	int response = iban_validation(text);
	if (response == 0){
		gtk_label_set_text(label1, _("To nie jest numer IBAN!"));
		show_icon(2, app);
	}else if (response == -1){
		gtk_label_set_text(label1, _("podany ciąg znaków zawiera błędy"));
		show_icon(2, app);
	}else if (response == 1){
		gtk_label_set_text(label1, _("To jest porawny numer IBAN"));
		show_icon(1, app);
	}else{
		gtk_label_set_text(label1, _("To nie jest numer IBAN!"));
		show_icon(2, app);
	}

}



void button2_clicked_cb(GtkWidget * widget, App *app)
{
	UI_ELEMENT(GtkEntry, entry1);
	UI_ELEMENT(GtkLabel, label1);
	gtk_entry_set_text(entry1, " ");
	gtk_label_set_text(label1, " ");
	show_icon(0, app);

}

void button3_clicked_cb(GtkWidget * widget, App *app)
{
	g_application_quit(G_APPLICATION(app->application));
}

static void activate(GApplication *application, App *app) {

	UI_ELEMENT(GtkWindow, window);
	UI_ELEMENT(GtkButton, button1);
	UI_ELEMENT(GtkButton, button2);
	UI_ELEMENT(GtkButton, button3);
	UI_ELEMENT(GtkImage, image1);
	UI_ELEMENT(GtkLabel, label1);

	gtk_button_set_label(button1, _("Weryfikuj"));
	gtk_button_set_label(button2, _("Wyczyść"));
	gtk_button_set_label(button3, _("Zamknij"));
	gtk_label_set_text(label1, " ");
	gtk_widget_hide(GTK_WIDGET(image1));

	gtk_application_add_window(GTK_APPLICATION(app->application), window);
	gtk_widget_show(GTK_WIDGET(window));

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
	g_object_unref(app->builder);
	g_slist_free(app->objects);

	return status;
}
