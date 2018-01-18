/*
 * example01.c
 *
 *	GtkTreeView example from
 *	https://en.wikibooks.org/wiki/GTK%2B_By_Example/Tree_View/Columns_and_Renderers
 *
 *  Created on: 18 sty 2018
 *      Author: majsterklepka
 *     License: GPL-3.0
 */

#include <gtk/gtk.h>

enum
{
  COL_FIRST_NAME = 0,
  COL_LAST_NAME,
  COL_YEAR_BORN,
  NUM_COLS
} ;

static GtkTreeModel *
create_and_fill_model (void)
{
  GtkTreeStore  *treestore;
  GtkTreeIter    toplevel, child;

  treestore = gtk_tree_store_new(NUM_COLS,
                                 G_TYPE_STRING,
                                 G_TYPE_STRING,
                                 G_TYPE_UINT);

  /* Append a top level row and leave it empty */
  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel,
                     COL_FIRST_NAME, "Jovita",
                     COL_LAST_NAME, "Incognito",
                     -1);

  /* Append a second top level row, and fill it with some data */
  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel,
                     COL_FIRST_NAME, "Ewelina",
                     COL_LAST_NAME, "Average",
                     COL_YEAR_BORN, (guint) 1962,
                     -1);

  /* Append a child to the second top level row, and fill in some data */
  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_FIRST_NAME, "Magdalena",
                     COL_LAST_NAME, "Average",
                     COL_YEAR_BORN, (guint) 1985,
                     -1);

  return GTK_TREE_MODEL(treestore);
}

void
age_cell_data_func (GtkTreeViewColumn *col,
                    GtkCellRenderer   *renderer,
                    GtkTreeModel      *model,
                    GtkTreeIter       *iter,
                    gpointer           user_data)
{
  guint  year_born;
  guint  year_now = 2018;
  gchar  buf[64];

  gtk_tree_model_get(model, iter, COL_YEAR_BORN, &year_born, -1);

  if (year_born <= year_now && year_born > 0)
  {
    guint age = year_now - year_born;

    g_snprintf(buf, sizeof(buf), "%u years old", age);

    g_object_set(renderer, "foreground-set", FALSE, NULL); /* print this normal */
  }
  else
  {
    g_snprintf(buf, sizeof(buf), "age unknown");

    /* make green */
    g_object_set(renderer, "foreground", "Green", "foreground-set", TRUE, NULL);
  }

  g_object_set(renderer, "text", buf, NULL);
}


static GtkWidget *
create_view_and_model (void)
{
  GtkTreeViewColumn   *col;
  GtkCellRenderer     *renderer;
  GtkWidget           *view;
  GtkTreeModel        *model;

  view = gtk_tree_view_new();

  /* --- Column #1 --- */

  col = gtk_tree_view_column_new();

  gtk_tree_view_column_set_title(col, "Imię");

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect 'text' property of the cell renderer to
   *  model column that contains the first name */
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_FIRST_NAME);


  /* --- Column #2 --- */

  col = gtk_tree_view_column_new();

  gtk_tree_view_column_set_title(col, "Nazwisko");

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect 'text' property of the cell renderer to
   *  model column that contains the last name */
  gtk_tree_view_column_add_attribute(col, renderer, "text", COL_LAST_NAME);

  /* set 'weight' property of the cell renderer to
   *  bold print (we want all last names in bold) */
  g_object_set(renderer,
               "weight", PANGO_WEIGHT_BOLD,
               "weight-set", TRUE,
               NULL);


  /* --- Column #3 --- */

  col = gtk_tree_view_column_new();

  gtk_tree_view_column_set_title(col, "Wiek");

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect a cell data function */
  gtk_tree_view_column_set_cell_data_func(col, renderer, age_cell_data_func, NULL, NULL);


  model = create_and_fill_model();

  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

  g_object_unref(model); /* destroy model automatically with view */

  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
                              GTK_SELECTION_NONE);

  return view;
}


int
main (int argc, char **argv)
{
  GtkWidget *window;
  GtkWidget *view;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "file: example01.c");
  gtk_widget_set_size_request(window, 260, 100);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  view = create_view_and_model();

  gtk_container_add(GTK_CONTAINER(window), view);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}



