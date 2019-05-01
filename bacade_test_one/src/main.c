#define GETTEXT_PACKAGE "bacade"

#include <gtk/gtk.h>
#include <glib/gi18n-lib.h>
#include <langinfo.h>
#include <locale.h>
#include "../bacade-gresource.h"
#include "print.h"

gboolean close_screen(gpointer data)
{
  gtk_widget_destroy((GtkWidget*)data);	
  return(FALSE);
}

void Set_Icons()
{
	gint dim[] = {256, 128, 48, 32, 24, 22, 0};
	GList *list = NULL;
	gint i = 0;
	GError *error = NULL;
	
		while(dim[i] != 0 )
		{
		gchar * path = g_strdup_printf("/org/majsterklepka/bacade/data/bacade%d.png", dim[i]);
		GdkPixbuf *icon = gdk_pixbuf_new_from_resource(path, &error);
		list = g_list_append(list, icon);
		i++;		
		}
		gtk_window_set_default_icon_list(list);
		g_list_foreach(list, (GFunc) g_object_unref, NULL);
		g_list_free(list);
}

gboolean show_screen(gpointer data)
{
  gtk_widget_show_all((GtkWidget*)data);
  return(FALSE);
}

int Show_Splash(GApplication *app, gpointer data, int time)
{
	GtkWindow *splash = (GtkWindow*) gtk_application_window_new(GTK_APPLICATION(app)); 
	gtk_window_set_decorated(splash, FALSE);
	gtk_window_set_type_hint(splash, GDK_WINDOW_TYPE_HINT_SPLASHSCREEN);
	gtk_window_set_position(GTK_WINDOW(splash), GTK_WIN_POS_CENTER_ALWAYS);
 	gtk_window_set_resizable(GTK_WINDOW(splash), FALSE);
	gtk_window_set_default_size(splash, 600, 400);
	GtkWidget *image = gtk_image_new_from_resource("/org/majsterklepka/bacade/data/splashscreen.png");
	gtk_container_add(GTK_CONTAINER(splash), image);
	gtk_window_set_keep_above(splash, TRUE);
	gtk_widget_show_all(GTK_WIDGET(splash));
	g_timeout_add(time, close_screen, splash);
	g_timeout_add(time - 100, show_screen, data);
	return 0;
}

void close_window(GtkWidget *widget,  gpointer user_data)
{
	g_application_quit(G_APPLICATION(user_data));
}

void activate(GApplication *app, gpointer user_data){

	GtkBuilder *builder;
	GError *error = NULL;
	GtkWindow *window;
	gchar *title;

	
	builder = gtk_builder_new();
	
	gtk_builder_add_from_resource(builder, "/org/majsterklepka/bacade/window_main.ui", &error);

	gtk_builder_connect_signals(builder, NULL);
			
	window = (GtkWindow *) gtk_application_window_new(GTK_APPLICATION(app));
	window = (GtkWindow *) gtk_builder_get_object(builder, "window1");
	
	title = g_strdup_printf("%s %s", g_get_application_name(), "1.50.1");
	gtk_window_set_title(GTK_WINDOW(window), title);
	
	Set_Icons();
		
	g_signal_connect(window, "destroy", G_CALLBACK(close_window), app);
	
	g_object_unref(builder);

	Show_Splash(app, window, 4000);

}


int main(int argc, char **argv)
{

	GtkApplication *app;
	gint status;
	GResource *resource;
	GAppInfo *info;
	GError *error = NULL; 
	
	resource = bacade_get_resource();
	g_resources_register(resource);

	/*setlocale (LC_ALL, "");
 	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
 	textdomain (GETTEXT_PACKAGE);
 	*/
 	
 	g_set_application_name("Bank Cash Deposit");
	g_set_prgname(g_get_application_name());	

 	info = g_app_info_create_from_commandline("gres", g_get_application_name(), G_APP_INFO_CREATE_NONE, &error);
 	
	app = gtk_application_new("org.majsterklepka.bacade", 0);
	
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_application_register(G_APPLICATION(app), NULL, NULL);
	
	return status;



}
