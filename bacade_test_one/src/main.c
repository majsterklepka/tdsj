#define GETTEXT_PACKAGE "bacade"

#include <gtk/gtk.h>
#include <glib/gi18n-lib.h>
#include <langinfo.h>
#include <locale.h>
#include "../bacade-gresource.h"
#include "print.h"
#include <malloc.h>

GtkBuilder *builder;
GtkListStore *liststore;
GResource *resource;
GList *active_prints = NULL;
GtkPrintSettings *settings;
GtkPageSetup *page_setup;

gboolean iterate_func(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, PrintData *print_data)
{
	FormData *form; 

	form = g_new0(FormData, 1);

    
    gchar *addres1, *iban1, *kwota, *addres2, *iban2, *tytul, *waluta, *trans;
    
    gtk_tree_model_get(GTK_TREE_MODEL(liststore), iter, 0, &addres1, 1, &iban1, 2, &kwota, 3, &waluta, 4, &trans, 5, &iban2, 6, &addres2, 7, &tytul, -1);


	form->addres1 = g_strdup(addres1);
	form->iban1 = g_strdup(iban1);
	form->kwota = g_strdup(kwota);
	form->iban2 = g_strdup(iban2);
	form->addres2 = g_strdup(addres2);
	form->waluta = g_strdup(waluta);
	form->tytul = g_strdup(tytul);
	const gchar *template = "WPLATA";
	if (g_strcmp0(trans, template) == 0)
		form->trans = FALSE;
	else
		form->trans = TRUE;
		
	print_data->data = g_list_append(print_data->data, form);
	print_data->data_length += 1;
	
	g_free(addres1);
	g_free(addres2);
	g_free(iban1);
	g_free(iban2);
	g_free(kwota);
	g_free(waluta);
	g_free(trans);

	return FALSE;
}

void begin_print (GtkPrintOperation *operation, GtkPrintContext *context, PrintData *user_data)
{
	gtk_tree_model_foreach(GTK_TREE_MODEL(liststore), (GtkTreeModelForeachFunc)iterate_func, user_data);	
	gtk_print_operation_set_n_pages(operation, user_data->data_length);
	
}

static void draw_page (GtkPrintOperation *operation, GtkPrintContext *context, gint page_nr, PrintData *user_data)
{
	
	PangoLayout *layout;
	cairo_t *cr;
	double x_margin, y_margin; 

    
    double width, height;   
    
    width = gtk_print_context_get_width(context);
	height = gtk_print_context_get_height(context);

	layout = gtk_print_context_create_pango_layout(context);

	pango_layout_set_width(layout, width * PANGO_SCALE);
	pango_layout_set_height(layout, height * PANGO_SCALE);
	
	cr = gtk_print_context_get_cairo_context (context);
	
	x_margin = 10.0;
	y_margin = 20.0;
	FormData *form_data; 
	
	form_data = g_new0(FormData, 1);
	form_data = (FormData*)g_list_nth_data(user_data->data, page_nr);
	if(form_data == NULL)
		g_print("error\n");
			
	draw_form(cr, layout, x_margin, y_margin, form_data);
	
}


void end_print (GtkPrintOperation *operation, GtkPrintContext *context, PrintData *user_data)
{	
}


static void print_done(GtkPrintOperation *op, GtkPrintOperationResult res,
		PrintData *print_data) {
	GError *error = NULL;
	
	GtkWidget *window1 = (GtkWidget*)gtk_builder_get_object(builder, "window1");
	
	if (res == GTK_PRINT_OPERATION_RESULT_ERROR) {

		GtkWidget *error_dialog;

		gtk_print_operation_get_error(op, &error);

		error_dialog = gtk_message_dialog_new(GTK_WINDOW(window1),
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE, _("Error printing file:\n%s"),
				error ? error->message : "no details");
		g_signal_connect(error_dialog, "response",
				G_CALLBACK (gtk_widget_destroy), NULL);
		gtk_widget_show(error_dialog);
	} else if (res == GTK_PRINT_OPERATION_RESULT_APPLY) {
		if (settings != NULL)
			g_object_unref(settings);
		settings = g_object_ref(gtk_print_operation_get_print_settings(op));
	}
	g_free(print_data);

	if (!gtk_print_operation_is_finished(op)) {
		g_object_ref(op);
		active_prints = g_list_append(active_prints, op);

	}
}

static void do_print (void)
{
	GtkPrintOperation *print;
	PrintData *data;
	GError *error = NULL;
	GtkWidget *error_dialog;
	data = g_new0(PrintData, 1);
	
	settings = gtk_print_settings_new();
	page_setup = gtk_page_setup_new();
	
	
	GtkWidget *window1 = (GtkWidget*)gtk_builder_get_object(builder, "window1");

	gint res;

	print = gtk_print_operation_new();
	gtk_print_operation_set_embed_page_setup(print, TRUE);
	gtk_print_operation_set_track_print_status(print, TRUE);
	gtk_print_operation_set_unit(print, GTK_UNIT_MM);

	if (settings != NULL)
  		gtk_print_operation_set_print_settings (print, settings);
  
	if (page_setup != NULL)
  		gtk_print_operation_set_default_page_setup (print, page_setup);
  
	g_signal_connect (print, "begin-print", 
                  G_CALLBACK (begin_print), data);
	g_signal_connect (print, "draw-page", 
                  G_CALLBACK (draw_page), data);
	g_signal_connect(print, "end-print", G_CALLBACK (end_print), data);
	g_signal_connect(print, "done", G_CALLBACK (print_done), data);

 
	res = gtk_print_operation_run (print, 
                               GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, 
                               GTK_WINDOW(window1), 
                               &error);
 
	if (res == GTK_PRINT_OPERATION_RESULT_ERROR)
 	{
   		error_dialog = gtk_message_dialog_new (GTK_WINDOW (window1),
                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                         GTK_MESSAGE_ERROR,
                         GTK_BUTTONS_CLOSE,
                         "Error printing file:\n%s",
                         error->message);
   		g_signal_connect (error_dialog, "response", 
                     G_CALLBACK (gtk_widget_destroy), NULL);
   		gtk_widget_show (error_dialog);
   		g_error_free (error);
 	}
	else if (res == GTK_PRINT_OPERATION_RESULT_APPLY)
 	{
   		if (settings != NULL)
		g_object_unref (settings);
   		settings = g_object_ref(gtk_print_operation_get_print_settings (print));
 }
	g_object_unref(print);

}

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

		gtk_window_set_default_icon_name("bacade-office");

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
	GtkComboBoxText *dialog1_comboboxtext1;
	GtkTreeIter iter;
	const gchar *text_entry1 = NULL, *text_entry2 = NULL, *text_entry3 = NULL, *text_entry4 = NULL, *text_entry5 = NULL, *text_entry6 = NULL, *text_switch1 = NULL;
	gboolean dialog1_switch1_state = FALSE;
	const gchar *text_comboboxtext1 = NULL;

	dialog1_entry1 = (GtkEntry*)gtk_entry_new();
	dialog1_entry2 = (GtkEntry*)gtk_entry_new();
	dialog1_entry3 = (GtkEntry*)gtk_entry_new();
	dialog1_entry4 = (GtkEntry*)gtk_entry_new();
	dialog1_entry5 = (GtkEntry*)gtk_entry_new();
	dialog1_entry6 = (GtkEntry*)gtk_entry_new();
	dialog1_switch1 = (GtkSwitch*)gtk_switch_new();
	dialog1_comboboxtext1 = (GtkComboBoxText*)gtk_combo_box_text_new();
	dialog1_comboboxtext1 = (GtkComboBoxText*)gtk_builder_get_object(builder, "dialog1_comboboxtext1");
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
				text_comboboxtext1 = gtk_combo_box_text_get_active_text(dialog1_comboboxtext1);

				if (dialog1_switch1_state){
					text_switch1 = "PRZELEW";
					text_entry4 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry4));
				}
				else{
					text_switch1 = "WPLATA";
					text_entry4 = "- - -";
				}
				
				gtk_list_store_set (liststore, &iter,
									  0, text_entry1,
									  1, text_entry2,
									  2, text_entry3,
									  3, text_comboboxtext1,
									  4, text_switch1,
									  5, text_entry4,
									  6, text_entry5,
									  7, text_entry6,
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
	GtkWidget *button4;

	window = (GtkWindow *) gtk_application_window_new(GTK_APPLICATION(app));
	window = (GtkWindow *) gtk_builder_get_object(builder, "window1");
	view = gtk_tree_view_new();
	view = (GtkWidget *) gtk_builder_get_object(builder, "treeview");
	button3 = gtk_button_new();
	button3 = (GtkWidget *) gtk_builder_get_object(builder, "button3");
	button4 = gtk_button_new();
	button4 = (GtkWidget *) gtk_builder_get_object(builder, "button4");
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
	                                               "Waluta",
	                                               renderer,
	                                               "text", 3,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,
	                                               "Typ operacji",
	                                               renderer,
	                                               "text", 4,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,
	                                               "IBAN Nadawcy",
	                                               renderer,
	                                               "text", 5,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
	                                               -1,
	                                               "Adres Nadawcy",
	                                               renderer,
	                                               "text", 6,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
		gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
		                                               -1,
		                                               "Tytuł operacji",
		                                               renderer,
		                                               "text", 7,
		                                               NULL);


	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), TRUE);

	liststore = gtk_list_store_new (8,G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), GTK_TREE_MODEL(liststore));

	g_signal_connect(window, "destroy", G_CALLBACK(close_window), app);
	g_signal_connect(button3, "clicked", G_CALLBACK(show_dialog), view);
	g_signal_connect(button4, "clicked", G_CALLBACK(do_print), NULL);
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
