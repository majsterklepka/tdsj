#define GETTEXT_PACKAGE "bacade"

#include <gtk/gtk.h>
#include <glib/gi18n-lib.h>
#include <langinfo.h>
#include <locale.h>
#include "../bacade-gresource.h"
#include "print.h"
#include <malloc.h>

GtkBuilder *builder;
GResource *resource;
GList *active_prints;
GtkPrintSettings *settings;
GtkWidget *window;
GtkWidget *treeview;

enum{
	COL_ADDRESS1,
	COL_IBAN1,
	COL_AMOUNT,
	COL_CURRENCY,
	COL_TRANSFER,
	COL_IBAN2,
	COL_ADDRESS2,
	COL_TITLE,
	NUM_COLS
};

static void
quit_app (GSimpleAction *action, GVariant *parametr, gpointer user_data)
{
  GList *list, *next;
  GtkWindow *win;

  g_print ("Going down...\n");

  list = gtk_application_get_windows (GTK_APPLICATION (g_application_get_default ()));
  while (list)
    {
      win = list->data;
      next = list->next;

      gtk_widget_destroy (GTK_WIDGET (win));

      list = next;
    }
}


gboolean iterate_func(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, PrintData *print_data)
{
	FormData *form; 

	form = g_new0(FormData, 1);
	
	GtkListStore *liststore = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
	
    
    gchar *addres1, *iban1, *kwota, *addres2, *iban2, *tytul, *waluta, *trans;
    
    gtk_tree_model_get(GTK_TREE_MODEL(liststore), iter, 0, &addres1, 1, &iban1, 2, &kwota, 3, &waluta, 4, &trans, 5, &iban2, 6, &addres2, 7, &tytul, -1);


	form->addres1 = g_strdup(addres1);
	form->iban1 = g_strdup(iban1);
	form->kwota = g_strdup(kwota);
	form->iban2 = g_strdup(iban2);
	form->addres2 = g_strdup(addres2);
	form->waluta = g_strdup(waluta);
	form->tytul = g_strdup(tytul);
	const gchar *template = " - ";
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
	
	GtkListStore *liststore = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
	
	gtk_tree_model_foreach(GTK_TREE_MODEL(liststore), (GtkTreeModelForeachFunc)iterate_func, user_data);
	if (user_data->data_length > 0)	
		gtk_print_operation_set_n_pages(operation, user_data->data_length);
	else{
		GtkWidget *window = (GtkWidget*)gtk_builder_get_object(builder, "window1");
		GtkWidget *error_dialog;

		error_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE, _("Printing Error!\nNo content add to print queue!"));
		g_signal_connect(error_dialog, "response",
				G_CALLBACK (gtk_widget_destroy), NULL);
		gtk_widget_show(error_dialog);
		
	}
		
		
}

static void draw_page (GtkPrintOperation *operation, GtkPrintContext *context, gint page_nr, PrintData *user_data)
{
	g_return_if_fail(user_data->data_length > 0 );
	
	PangoLayout *layout;
	cairo_t *cr;
	double x_margin, y_margin; 

    double width, height;
    
    double cr_width = 140.00;
    double cr_height = 200.00;   
    
    width = gtk_print_context_get_width(context);
	height = gtk_print_context_get_height(context);
	
	x_margin = (width - cr_width) / 2.00;
	y_margin = (height - cr_height) / 2.00;
	if (x_margin < 0){
		cr_width = width - 0.6;
		x_margin = 0;
	}
	if (y_margin < 0){
			cr_height = height - 0.6;
			y_margin = 0;
		}
	x_margin += 0.3;
	

	layout = gtk_print_context_create_pango_layout(context);

	pango_layout_set_width(layout, width * PANGO_SCALE);
	pango_layout_set_height(layout, height * PANGO_SCALE);
	
	cr = gtk_print_context_get_cairo_context (context);
	
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
		PrintData *print_data) 
{
	
	GError *error = NULL;	
	
	if (res == GTK_PRINT_OPERATION_RESULT_ERROR) {

		GtkWidget *error_dialog;

		gtk_print_operation_get_error(op, &error);

		error_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_CLOSE, _("Error printing file:\n%s"),
				error ? error->message : "no details");
		g_signal_connect(error_dialog, "response",
				G_CALLBACK (gtk_widget_destroy), NULL);
		gtk_widget_show(error_dialog);
	} else if (res == GTK_PRINT_OPERATION_RESULT_APPLY) {
		if (settings != NULL){
			g_object_unref (settings);
			settings = g_object_ref(gtk_print_operation_get_print_settings(op));
		}
	}
	g_free(print_data);

	if (!gtk_print_operation_is_finished(op)) {
		g_object_ref(op);
		active_prints = g_list_append(active_prints, op);
	}
	
}

 void do_print(GSimpleAction *action, GVariant *parametr, gpointer user_data)
{
	GtkPrintOperation *print;
	PrintData *data;
	GError *error = NULL;
	GtkWidget *error_dialog;
	data = g_new0(PrintData, 1);
	
	settings = gtk_print_settings_new();
	GtkPageSetup *page_setup = gtk_page_setup_new();
	
	GtkTreeModel *treemodel = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
	gint n = gtk_tree_model_iter_n_children(treemodel, NULL);
	
	g_return_if_fail(n > 0);
		
		
	gint res;

	gchar *job_name = g_strdup_printf("%s: wydruk %d", g_get_application_name(), (g_list_length(active_prints) + 1));

	print = gtk_print_operation_new();
	gtk_print_operation_set_job_name (print, job_name);
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
                               GTK_WINDOW(window), 
                               &error);
 
	if (res == GTK_PRINT_OPERATION_RESULT_ERROR)
 	{
   		error_dialog = gtk_message_dialog_new (GTK_WINDOW (window),
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
   		if (settings != NULL){
			g_object_unref (settings);
			settings = g_object_ref(gtk_print_operation_get_print_settings (print));
		}
 }
	

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

		gtk_window_set_default_icon_list(list);

		g_list_foreach(list, (GFunc) g_object_unref, NULL);
		g_list_free(list);
}



void dialog1_response2(GtkDialog *dialog, gint resp_id, gpointer user_data)
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
	GtkTreeModel *model;
	GtkTreeSelection *selection;
	
	
	GtkListStore *liststore = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
	
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

	const gchar *text_entry1 = NULL, *text_entry2 = NULL, *text_entry3 = NULL, *text_entry4 = NULL, *text_entry5 = NULL, *text_entry6 = NULL, *text_switch1 = NULL;
	gboolean dialog1_switch1_state = FALSE;
	const gchar *text_comboboxtext1 = NULL;

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
				text_entry1 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry1));
				text_entry2 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry2));
				text_entry3 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry3));
				text_entry5 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry5));
				text_entry6 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry6));
				dialog1_switch1_state = gtk_switch_get_state(dialog1_switch1);
				text_comboboxtext1 = gtk_combo_box_text_get_active_text(dialog1_comboboxtext1);

				if (dialog1_switch1_state){
					text_switch1 = "OK";
					text_entry4 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry4));
				}
				else{
					text_switch1 = "- - -";
					text_entry4 = "- - -";
				}
				if (gtk_tree_selection_get_selected(selection, &model, &iter) && g_utf8_strlen(text_entry1, -1) > 4)
				{
						gtk_list_store_set (liststore,  &iter,
									  0, text_entry1,
									  1, text_entry2,
									  2, text_entry3,
									  3, text_comboboxtext1,
									  4, text_switch1,
									  5, text_entry4,
									  6, text_entry5,
									  7, text_entry6,
									  -1);
					gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(liststore));
				}
				gtk_widget_destroy(GTK_WIDGET(dialog));
				break;
				case GTK_RESPONSE_NO:
					gtk_entry_set_text(dialog1_entry1, " ");
					gtk_entry_set_text(dialog1_entry2, " ");
					gtk_entry_set_text(dialog1_entry3, " ");
					gtk_entry_set_text(dialog1_entry4, " ");
					gtk_entry_set_text(dialog1_entry5, " ");
					gtk_entry_set_text(dialog1_entry6, " ");
					gtk_switch_set_state(dialog1_switch1, FALSE);
					gtk_combo_box_set_active(GTK_COMBO_BOX(dialog1_comboboxtext1), 0);
					break;
		default:
				gtk_widget_destroy(GTK_WIDGET(dialog));
				break;
		}
	
}

static void dialog1_remove_row(GSimpleAction *action, GVariant *parametr, gpointer user_data)
{
	GtkTreeIter iter;
	
	GtkTreeModel *model;
	
	GtkTreeSelection *selection;
	
	GtkListStore *liststore = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
	
	g_return_if_fail(liststore != NULL);
	
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	
	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gtk_list_store_remove(liststore, &iter);
		gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(liststore));
		if(gtk_tree_model_get_iter_first(GTK_TREE_MODEL(liststore), &iter))
			gtk_tree_selection_select_iter(selection, &iter);
		
	}
	gtk_widget_show(GTK_WIDGET(treeview));	
	
}


void dialog1_response1(GtkDialog *dialog, gint resp_id, gpointer user_data)
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
	
	GtkListStore *liststore = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
	
	
	const gchar *text_entry1 = NULL, *text_entry2 = NULL, *text_entry3 = NULL, *text_entry4 = NULL, *text_entry5 = NULL, *text_entry6 = NULL, *text_switch1 = NULL;
	gboolean dialog1_switch1_state = FALSE;
	const gchar *text_comboboxtext1 = NULL;

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
					text_switch1 = "OK";
					text_entry4 = gtk_entry_get_text(GTK_ENTRY(dialog1_entry4));
				}
				else{
					text_switch1 = "- - -";
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
	
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(liststore));
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
	gtk_tree_selection_select_iter(selection, &iter);
	gtk_widget_hide(GTK_WIDGET(dialog));


}
static void dialog1_edit(GSimpleAction *action, GVariant *parametr, gpointer user_data)
{
	GError *error = NULL;
	GtkWidget *dialog;
	GtkWidget *button;
	GApplication *app = g_application_get_default();
	const gchar *basepath = g_application_get_resource_base_path(app);
	gchar *path = g_build_path("/", basepath, "Gtk", "dialog1.ui", NULL);
	gtk_builder_add_from_resource(builder, path, &error);
	gtk_builder_connect_signals(builder, app);
	dialog = (GtkWidget*)gtk_builder_get_object(builder, "dialog1");
	button = (GtkWidget*)gtk_builder_get_object(builder, "dialog1_button3");
	
	gtk_window_set_title(GTK_WINDOW(dialog), "Edycja danych przelewu/wpłaty");
	g_signal_connect(dialog, "response", G_CALLBACK(dialog1_response2), user_data);
	
	
	GtkEntry *dialog1_entry1;
	GtkEntry *dialog1_entry2;
	GtkEntry *dialog1_entry3;
	GtkEntry *dialog1_entry4;
	GtkEntry *dialog1_entry5;
	GtkEntry *dialog1_entry6;
	GtkSwitch *dialog1_switch1;
	GtkComboBoxText *dialog1_comboboxtext1;
	
	GtkTreeIter iter;
	GtkTreeModel     *model;
	GtkTreeSelection *selection;
	
	
	GtkListStore *liststore = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));

	const gchar *text_entry1 = NULL, *text_entry2 = NULL, *text_entry3 = NULL, *text_entry4 = NULL, *text_entry5 = NULL, *text_entry6 = NULL, *text_switch1 = NULL;
	gboolean dialog1_switch1_state = FALSE;
	const gchar *text_comboboxtext1 = NULL;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

	
	g_return_if_fail((gtk_tree_selection_get_selected(selection, &model, &iter)));

	g_signal_connect(dialog, "response", G_CALLBACK(dialog1_response2), app);
	gtk_tree_model_get (model, &iter, 0, &text_entry1, 1, &text_entry2, 2, &text_entry3, 3, &text_comboboxtext1, 4, &text_switch1, 5, &text_entry4, 6, &text_entry5, 7, &text_entry6, -1);
    
	
	dialog1_comboboxtext1 = (GtkComboBoxText*)gtk_builder_get_object(builder, "dialog1_comboboxtext1");
	dialog1_entry1 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry1");
	dialog1_entry2 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry2");
	dialog1_entry3 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry3");
	dialog1_entry4 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry4");
	dialog1_entry5 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry5");
	dialog1_entry6 = (GtkEntry*)gtk_builder_get_object(builder, "dialog1_entry6");
	dialog1_switch1 = (GtkSwitch*)gtk_builder_get_object(builder, "dialog1_switch1");
	
	gtk_entry_set_text(dialog1_entry1, text_entry1);
	gtk_entry_set_text(dialog1_entry2, text_entry2);
	gtk_entry_set_text(dialog1_entry3, text_entry3);
	gtk_entry_set_text(dialog1_entry4, text_entry4);
	gtk_entry_set_text(dialog1_entry5, text_entry5);
	gtk_entry_set_text(dialog1_entry6, text_entry6);
	gint index = 0;
	
	if (g_strcmp0(text_comboboxtext1, "PLN") == 0)
			index = 0;
	if (g_strcmp0(text_comboboxtext1, "USD") == 0)
			index = 1;
	if (g_strcmp0(text_comboboxtext1, "EUR") == 0)
			index = 4;
	if (g_strcmp0(text_comboboxtext1, "GBP") == 0)
			index = 2;
	if (g_strcmp0(text_comboboxtext1, "CHF") == 0)
			index = 3;			
			
	gtk_combo_box_set_active(GTK_COMBO_BOX(dialog1_comboboxtext1), index);
	
	if(g_strcmp0(text_switch1, "OK") == 0)
		gtk_switch_set_state(dialog1_switch1, TRUE);
	else
		gtk_switch_set_state(dialog1_switch1, FALSE);
	
	
	gtk_dialog_run(GTK_DIALOG(dialog));
}
static void dialog1_enter(GSimpleAction *action, GVariant *parametr, gpointer user_data)
{
	GError *error = NULL;
	GtkWidget *dialog;
	GtkWidget *button;
	GApplication *app = g_application_get_default();
	const gchar *basepath = g_application_get_resource_base_path(app);
	gchar *path = g_build_path("/", basepath, "Gtk", "dialog1.ui", NULL);
	gtk_builder_add_from_resource(builder, path, &error);
	gtk_builder_connect_signals(builder, NULL);
	dialog = (GtkWidget*)gtk_builder_get_object(builder, "dialog1");
	button = (GtkWidget*)gtk_builder_get_object(builder, "dialog1_button3");
	gtk_widget_hide(button);
	gtk_window_set_title(GTK_WINDOW(dialog), "Dane do przelewu/wpłaty");
	g_signal_connect(dialog, "response", G_CALLBACK(dialog1_response1), user_data);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(GTK_WIDGET(dialog));
}


void close_window(GtkWidget *widget,  gpointer user_data)
{
	g_application_quit(G_APPLICATION(user_data));
}

void new_window(GtkApplication *app)
{
	GtkToolItem *toolitem;
	GtkCellRenderer *renderer;
	GtkToolbar *toolbar;
	GtkWidget *scrolledwindow;
	GtkGrid *grid;
	GtkListStore *liststore;
	GtkWidget *icon;
	GtkWidget *linkbutton;
	GtkStyleContext *style_context;
	
	window = gtk_application_window_new(app);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER_ALWAYS);
	gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
	gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
	gtk_window_set_title(GTK_WINDOW(window), "Bank Cash Deposit");
	
	toolbar = GTK_TOOLBAR(gtk_toolbar_new());
	style_context = gtk_widget_get_style_context (GTK_WIDGET(toolbar));
	gtk_style_context_add_class (style_context, GTK_STYLE_CLASS_PRIMARY_TOOLBAR);
	scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scrolledwindow), 600);
	gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolledwindow), 300);
	gtk_widget_set_vexpand(scrolledwindow, TRUE);
	gtk_widget_set_hexpand(scrolledwindow, TRUE);
	treeview = gtk_tree_view_new();
	grid = GTK_GRID(gtk_grid_new());
	
	liststore = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	
	icon = gtk_image_new_from_icon_name("gtk-add", GTK_ICON_SIZE_SMALL_TOOLBAR);
	toolitem = gtk_tool_button_new(icon, "Dodaj");
	gtk_tool_item_set_tooltip_text(toolitem, gtk_tool_button_get_label(GTK_TOOL_BUTTON(toolitem)));
	gtk_toolbar_insert(toolbar, GTK_TOOL_ITEM(toolitem), 0);
	gtk_actionable_set_action_name(GTK_ACTIONABLE(toolitem), "win.add");
	gtk_widget_show(GTK_WIDGET(toolitem));
	
	icon = gtk_image_new_from_icon_name("gtk-remove", GTK_ICON_SIZE_SMALL_TOOLBAR);
	toolitem = gtk_tool_button_new(icon, "Usuń");
	gtk_tool_item_set_tooltip_text(toolitem, gtk_tool_button_get_label(GTK_TOOL_BUTTON(toolitem)));
	gtk_toolbar_insert(toolbar, GTK_TOOL_ITEM(toolitem), 1);
	gtk_actionable_set_action_name(GTK_ACTIONABLE(toolitem), "win.remove");
	gtk_widget_show(GTK_WIDGET(toolitem));
	
	icon = gtk_image_new_from_icon_name("gtk-edit", GTK_ICON_SIZE_SMALL_TOOLBAR);
	toolitem = gtk_tool_button_new(icon, "Popraw");
	gtk_tool_item_set_tooltip_text(toolitem, gtk_tool_button_get_label(GTK_TOOL_BUTTON(toolitem)));
	gtk_toolbar_insert(toolbar, GTK_TOOL_ITEM(toolitem), 2);
	gtk_actionable_set_action_name(GTK_ACTIONABLE(toolitem), "win.correct");
	gtk_widget_show(GTK_WIDGET(toolitem));
	
		
	icon = gtk_image_new_from_icon_name("filefind", GTK_ICON_SIZE_SMALL_TOOLBAR);
	toolitem = gtk_tool_button_new(icon, "Podgląd");
	gtk_tool_item_set_tooltip_text(toolitem, gtk_tool_button_get_label(GTK_TOOL_BUTTON(toolitem)));
	gtk_toolbar_insert(toolbar, GTK_TOOL_ITEM(toolitem), 3);
	gtk_actionable_set_action_name(GTK_ACTIONABLE(toolitem), "win.view");
	gtk_widget_show(GTK_WIDGET(toolitem));
	
	icon = gtk_image_new_from_icon_name("gtk-print", GTK_ICON_SIZE_SMALL_TOOLBAR);
	toolitem = gtk_tool_button_new(icon, "Drukuj");
	gtk_tool_item_set_tooltip_text(toolitem, gtk_tool_button_get_label(GTK_TOOL_BUTTON(toolitem)));
	gtk_toolbar_insert(toolbar, GTK_TOOL_ITEM(toolitem), 4);
	gtk_actionable_set_action_name(GTK_ACTIONABLE(toolitem), "win.print");
	gtk_widget_show(GTK_WIDGET(toolitem));
	
	linkbutton = gtk_link_button_new_with_label("http://majsterklepka.github.io",  "MajsterKlepka GitHub Page(s)");
	gtk_widget_set_halign(GTK_WIDGET(linkbutton), GTK_ALIGN_CENTER);
	gtk_widget_set_valign(GTK_WIDGET(linkbutton), GTK_ALIGN_CENTER);
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
	                                               -1,
	                                               "Adres Odbiorcy",
	                                               renderer,
	                                               "text", COL_ADDRESS1,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
	                                               -1,
	                                               "IBAN Odbiorcy",
	                                               renderer,
	                                               "text", COL_IBAN1,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
	                                               -1,
	                                               "Kwota",
	                                               renderer,
	                                               "text", COL_AMOUNT,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
	                                               -1,
	                                               "Waluta",
	                                               renderer,
	                                               "text", COL_CURRENCY,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
	                                               -1,
	                                               "Przelew",
	                                               renderer,
	                                               "text", COL_TRANSFER,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
	                                               -1,
	                                               "IBAN Nadawcy",
	                                               renderer,
	                                               "text", COL_IBAN2,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
	                                               -1,
	                                               "Adres Nadawcy",
	                                               renderer,
	                                               "text", COL_ADDRESS2,
	                                               NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview),
		                                               -1,
		                                               "Tytuł operacji",
		                                               renderer,
		                                               "text", COL_TITLE,
		                                               NULL);


	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview), TRUE);

	const GActionEntry entries[] = {
		{"add", dialog1_enter, NULL, NULL, NULL},
		{"correct", dialog1_edit, NULL, NULL, NULL},
		{"remove", dialog1_remove_row, NULL, NULL, NULL},
		{"view", dialog1_edit, NULL, NULL, NULL},
		{"print", do_print, NULL, NULL, NULL}
	};

	g_action_map_add_action_entries(G_ACTION_MAP(window), entries, G_N_ELEMENTS(entries), GTK_WINDOW(window));



	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(liststore));
	gtk_container_add(GTK_CONTAINER(scrolledwindow), GTK_WIDGET(treeview));
	gtk_widget_show(GTK_WIDGET(scrolledwindow));
	gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(toolbar), 0, 0, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), scrolledwindow, 0, 2, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(linkbutton), 1,3, 1, 1);
	gtk_widget_show(GTK_WIDGET(grid));
	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));
	
	gtk_widget_show_all(GTK_WIDGET(window));

	
}




void activate (GApplication *app, gpointer user_data)
{
	new_window(GTK_APPLICATION(app));
}

void startup(GApplication *app, gpointer user_data){

	GError *error = NULL;

	resource = bacade_get_resource();
	g_application_set_resource_base_path(app, "/org/majsterklepka/bacade");
	g_resources_register(resource);
	
	builder = gtk_builder_new();


	const GActionEntry entries[] = {
		{"quit", quit_app, NULL, NULL, NULL}
	};

	g_action_map_add_action_entries(G_ACTION_MAP(app), entries, G_N_ELEMENTS(entries), app);
	
	active_prints = NULL;

	Set_Icons();

	Show_Splash_Screen(app, 3000);
}


void clear_app()
{
	g_resources_unregister(resource);
	g_resource_unref(resource);
	
	/*GtkListStore *liststore = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
	if (liststore != NULL)	
		gtk_list_store_clear(liststore);
	*/
	if(active_prints != NULL)
		g_list_free1(active_prints);				
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
	g_signal_connect(app, "shutdown", G_CALLBACK(quit_app), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_application_register(G_APPLICATION(app), cancellable, &error);
	
	clear_app();
	}else
		status = -1;

	return status;



}
