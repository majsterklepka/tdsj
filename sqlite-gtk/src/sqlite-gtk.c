/*
 ============================================================================
 Name        : sqlite-gtk.c
 Author      : Paweł Sobótka
 Version     : 0.0.1
 License     : GNU GPL-3.0
 Description : An Example of SQLite and GTK+
 ============================================================================
 */

#include "function.h"
#include <stdlib.h>

void
fill_model (CustomList *customlist)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;

	sqlite3_open("data/expenses.db", &db);
	if (db == NULL)
	{
		return;
	}



	sqlite3_prepare_v2(db, "select * from expenses", -1, &stmt, NULL);

	while (sqlite3_step(stmt) != SQLITE_DONE) {
		custom_list_append_record(customlist, (const gchar*)sqlite3_column_text(stmt, 1), sqlite3_column_int(stmt, 2));
	}
		sqlite3_finalize(stmt);

		sqlite3_close(db);
}

GtkWidget *
create_view_and_model (void)
{
  GtkTreeViewColumn   *col;
  GtkCellRenderer     *renderer;
  CustomList          *customlist;
  GtkWidget           *view;

  customlist = custom_list_new();
  fill_model(customlist);

  view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(customlist));

  g_object_unref(customlist); /* destroy store automatically with view */

  renderer = gtk_cell_renderer_text_new();
  col = gtk_tree_view_column_new();

  gtk_tree_view_column_pack_start (col, renderer, TRUE);
  gtk_tree_view_column_add_attribute (col, renderer, "text", CUSTOM_LIST_COL_NAME);
  gtk_tree_view_column_set_title (col, "Name");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view),col);

  renderer = gtk_cell_renderer_text_new();
  col = gtk_tree_view_column_new();
  gtk_tree_view_column_pack_start (col, renderer, TRUE);
  gtk_tree_view_column_add_attribute (col, renderer, "text", CUSTOM_LIST_COL_AGE);
  gtk_tree_view_column_set_title (col, "Age");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view),col);

  return view;
}

int
main (int argc, char **argv)
{
  GtkWidget *window, *view, *scrollwin;

  gtk_init(&argc,&argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW(window), 200, 400);
  g_signal_connect(window, "delete_event", gtk_main_quit, NULL);

  scrollwin = gtk_scrolled_window_new(NULL,NULL);

  view = create_view_and_model();

  gtk_container_add(GTK_CONTAINER(scrollwin), view);
  gtk_container_add(GTK_CONTAINER(window), scrollwin);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
