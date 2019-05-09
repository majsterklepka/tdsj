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


int Show_Splash_Screen(int tme)
{
  GtkWidget  *image, *window;
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
  gtk_window_set_decorated(GTK_WINDOW (window), FALSE);
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER_ALWAYS);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  GApplication *app = g_application_get_default();
  const gchar *basepath = g_application_get_resource_base_path(app);
  gchar *path = g_build_path("/", basepath, "data/splashscreen.png", NULL);
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
	gint dim[] = {256, 128, 48, 32, 24, 22, 0};
	GList *list = NULL;
	gint i = 0;
	GError *error = NULL;
	GApplication *app = g_application_get_default();
	const gchar *basepath = g_application_get_resource_base_path(app);
	
		while(dim[i] != 0 )
		{
		gchar * path = g_strdup_printf("%s/data/bacade%d.png", basepath, dim[i]);
		GdkPixbuf *icon = gdk_pixbuf_new_from_resource(path, &error);
		list = g_list_append(list, icon);
		i++;		
		}
		gtk_window_set_default_icon_list(list);
		g_list_foreach(list, (GFunc) g_object_unref, NULL);
		g_list_free(list);
}

void response_user(GtkDialog *dialog, gint resp_id, gpointer user_data)
{
	GtkEntry *entry1;
	GtkEntry *entry2;
	GtkTreeIter iter;

	entry1 = (GtkEntry*)gtk_entry_new();
	entry2 = (GtkEntry*)gtk_entry_new();
	entry1 = (GtkEntry*)gtk_builder_get_object(builder, "entry1");
	entry2 = (GtkEntry*)gtk_builder_get_object(builder, "entry2");
	switch(resp_id){
		case GTK_RESPONSE_OK:
				gtk_list_store_append(liststore, &iter);
				gtk_list_store_set (liststore, &iter,
									  0,gtk_entry_get_text(GTK_ENTRY(entry1)),
									  1,gtk_entry_get_text(GTK_ENTRY(entry2)) ,
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
	gchar *path = g_build_path("/", basepath, "dialog_input.ui", NULL);
	gtk_builder_add_from_resource(builder, path, &error);
	gtk_builder_connect_signals(builder, NULL);
	dialog = (GtkWidget*)gtk_builder_get_object(builder, "dialog_input");
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
	                                               "Imię",
	                                               renderer,
	                                               "text", 0,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,
	                                               "Nazwisko",
	                                               renderer,
	                                               "text", 1,
	                                               NULL);

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), TRUE);

	liststore = gtk_list_store_new (2,G_TYPE_STRING, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), GTK_TREE_MODEL(liststore));
	Set_Icons();

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
	gchar *path = g_build_path("/", basepath, "window_main.ui", NULL);

	gtk_builder_add_from_resource(builder, path, &error);

	gtk_builder_connect_signals(builder, app);

	Show_Splash_Screen(3000);
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
	gint status = 0;
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
	g_application_register(G_APPLICATION(app), NULL, NULL);
	
	clear_app();
	}else
		status = -1;

	return status;



}
