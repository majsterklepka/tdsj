#define GETTEXT_PACKAGE "bacade"

#include <gtk/gtk.h>
#include <glib/gi18n-lib.h>
#include <langinfo.h>
#include <locale.h>
#include "../bacade-gresource.h"
#include "print.h"

GtkBuilder *builder;
GtkListStore *liststore;
GResource *resource;

gboolean close_screen(gpointer data)
{
  gtk_widget_destroy((GtkWidget*)data);
  gtk_main_quit ();
  return(FALSE);
}


int Show_Splash_Screen(GApplication *app, int tme)
{
  GtkWidget  *image, *window;
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
  gtk_window_set_decorated(GTK_WINDOW (window), FALSE);
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  const gchar *basepath = g_application_get_resource_base_path(app);
  gchar *path = g_build_path("/", basepath, "img", "splashscreen.png", NULL);
  image = gtk_image_new_from_resource(path);
  gtk_container_add(GTK_CONTAINER(window), image);
  gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
  gtk_widget_show_all (window);
  g_timeout_add (tme, close_screen, window);
  gtk_main ();
  return 0;
}


void Set_Icons()
{
	gint dim[] = {1024, 512, 256, 128, 96, 72, 64, 48, 32, 24, 22, 0};
	GList *list = NULL;
	gint i = 0;
	GError *error = NULL;
	GApplication *app = g_application_get_default();
	const gchar *basepath = g_application_get_resource_base_path(app);
	gchar *path = g_build_path("/", basepath, "img", NULL);
		while(dim[i] != 0 )
		{
			gchar *name = g_strdup_printf("icons%dx%d.png", dim[i], dim[i]);
			gchar *resource = g_build_path("/", path, name,  NULL);
			GdkPixbuf *icon = gdk_pixbuf_new_from_resource(resource, &error);
			if(error){
				g_print("error at %s!\n", error->message);
				g_error_free(error);
			}else{
				list = g_list_append(list, icon);
				i++;
			}
		g_free(resource);
		g_free(name);
		}

		gtk_window_set_default_icon_list(list);

		g_list_foreach(list, (GFunc) g_object_unref, NULL);
		g_list_free(list);
}

void response_user(GtkDialog *dialog, gint resp_id, gpointer user_data)
{
	GtkEntry *dialog1_entry1;
	GtkEntry *dialog1_entry2;
	GtkEntry *dialog1_entry3;
	GtkEntry *dialog1_entry4;
	GtkEntry *dialog1_entry5;
	GtkEntry *dialog1_entry6;
	GtkSwitch *dialog1_switch1;
	GtkTreeIter iter;
	const gchar *text_entry1 = NULL, *text_entry2 = NULL, *text_entry3 = NULL, *text_entry4 = NULL, *text_entry5 = NULL, *text_entry6 = NULL, *text_switch1 = NULL;
	gboolean dialog1_switch1_state = FALSE;

	dialog1_entry1 = (GtkEntry*)gtk_entry_new();
	dialog1_entry2 = (GtkEntry*)gtk_entry_new();
	dialog1_entry3 = (GtkEntry*)gtk_entry_new();
	dialog1_entry4 = (GtkEntry*)gtk_entry_new();
	dialog1_entry5 = (GtkEntry*)gtk_entry_new();
	dialog1_entry6 = (GtkEntry*)gtk_entry_new();
	dialog1_switch1 = (GtkSwitch*)gtk_switch_new();
	dialog1_entry1 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry1");
	dialog1_entry2 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry2");
	dialog1_entry3 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry3");
	dialog1_entry4 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry4");
	dialog1_entry5 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry5");
	dialog1_entry6 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry6");
	dialog1_switch1 = (GtkSwitch*)gtk_builder_get_object(builder, "dialog1_switch1");
	switch(resp_id){
		case GTK_RESPONSE_OK:
				gtk_list_store_append(liststore, &iter);
				text_entry1 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry1));
				text_entry2 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry2));
				text_entry3 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry3));
				text_entry5 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry5));
				text_entry6 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry6));
				dialog1_switch1_state = gtk_switch_get_state(dialog1_switch1);

				if (dialog1_switch1_state){
					text_switch1 = "PRZELEW";
					text_entry4 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry4));
				}
				else{
					text_switch1 = "WPŁATA";
					text_entry4 = '\0';
				}

				gtk_list_store_set (liststore, &iter,
									  0, text_entry1,
									  1, text_entry2,
									  2, text_entry3,
									  3, text_switch1,
									  4, text_entry4,
									  5, text_entry5,
									  6, text_entry6,
									  -1);
				break;
		default:
				break;
	}
	gtk_tree_view_set_model(GTK_TREE_VIEW(user_data), GTK_TREE_MODEL(liststore));
	gtk_widget_hide(GTK_WIDGET(dialog));


}

void show_dialog(GtkButton *button, gpointer user_data)
{
	GError *error = NULL;
	GtkWidget *dialog;
	GApplication *app = g_application_get_default();
	const gchar *basepath = g_application_get_resource_base_path(app);
	gchar *path = g_build_path("/", basepath, "Gtk", "dialog1.ui", NULL);
	gtk_builder_add_from_resource(builder, path, &error);
	gtk_builder_connect_signals(builder, NULL);
	dialog = (GtkWidget*)gtk_builder_get_object(builder, "dialog1");
	gtk_window_set_title(GTK_WINDOW(dialog), "Dane do przelewu/wpłaty");
	g_signal_connect(dialog, "response", G_CALLBACK(response_user), user_data);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

void close_window(GtkWidget *widget,  gpointer user_data)
{
	g_application_quit(G_APPLICATION(user_data));
}

void activate(GApplication *app, gpointer user_data){

	GtkWindow *window;
	gchar *title;
	GtkWidget *view;
	GtkCellRenderer *renderer;
	GtkWidget *button3;

	window = (GtkWindow *) gtk_application_window_new(GTK_APPLICATION(app));
	window = (GtkWindow *) gtk_builder_get_object(builder, "window1");
	view = gtk_tree_view_new();
	view = (GtkWidget *) gtk_builder_get_object(builder, "treeview");
	button3 = gtk_button_new();
	button3 = (GtkWidget *) gtk_builder_get_object(builder, "button3");
	title = g_strdup_printf("%s %s", g_get_application_name(), "1.50.1");
	gtk_window_set_title(GTK_WINDOW(window), title);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,
	                                               "Adres Odbiorcy",
	                                               renderer,
	                                               "text", 0,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,
	                                               "IBAN Odbiorcy",
	                                               renderer,
	                                               "text", 1,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,
	                                               "Kwota",
	                                               renderer,
	                                               "text", 2,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,
	                                               "Typ operacji",
	                                               renderer,
	                                               "text", 3,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,
	                                               "IBAN Nadawcy",
	                                               renderer,
	                                               "text", 4,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,
	                                               "Adres Nadawcy",
	                                               renderer,
	                                               "text", 5,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
		gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
		                                               -1,
		                                               "Tytuł operacji",
		                                               renderer,
		                                               "text", 6,
		                                               NULL);


	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), TRUE);

	liststore = gtk_list_store_new (7,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), GTK_TREE_MODEL(liststore));

	g_signal_connect(window, "destroy", G_CALLBACK(close_window), app);
	g_signal_connect(button3, "clicked", G_CALLBACK(show_dialog), view);
	gtk_widget_show_all(GTK_WIDGET(window));
}

void startup(GApplication *app, gpointer user_data){

	GError *error = NULL;

	resource = bacade_get_resource();
	g_application_set_resource_base_path(app, "/org/majsterklepka/bacade");
	g_resources_register(resource);


	builder = gtk_builder_new();
	const gchar *basepath = g_application_get_resource_base_path(app);
	gchar *path = g_build_path("/", basepath, "Gtk", "window_main.ui", NULL);

	gtk_builder_add_from_resource(builder, path, &error);

	gtk_builder_connect_signals(builder, app);

	Set_Icons();

	Show_Splash_Screen(app, 3000);
}


void clear_app()
{
	g_resources_unregister(resource);
	g_resource_unref(resource);
	gtk_list_store_clear(liststore);
	g_object_unref(builder);
}

int main(int argc, char **argv)
{

	GtkApplication *app;
	GError *error;
	gint status = 0;
	GCancellable *cancellable;

	cancellable = g_cancellable_new();
	//GAppInfo *info;
	//GError *error = NULL;
	
	/*setlocale (LC_ALL, "");
 	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
 	textdomain (GETTEXT_PACKAGE);
 	*/
 	
 	g_set_application_name("Bank Cash Deposit");
	g_set_prgname(g_get_application_name());	

 	//info = g_app_info_create_from_commandline("gres", g_get_application_name(), G_APP_INFO_CREATE_NONE, &error);
 	
	const gchar *app_id = "org.majsterklepka.bacade";
	if (g_application_id_is_valid(app_id))
	{

	app = gtk_application_new(app_id, G_APPLICATION_FLAGS_NONE);
	
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	g_signal_connect(app, "startup", G_CALLBACK(startup), NULL);

	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_application_register(G_APPLICATION(app), cancellable, &error);
	
	clear_app();
	}else
		status = -1;

	return status;



}
