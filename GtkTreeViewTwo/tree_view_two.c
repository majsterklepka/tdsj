/*
 * tree_view_two.c
 *
 *  Created on: 14 sty 2018
 *      Author: majsterklepka
 *     License: GPL-3.0
 */

#include <gtk/gtk.h>

enum {
	LIST_ITEM = 0, N_COLUMNS
};

void init_list(GtkWidget *list) {

	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkListStore *store;

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Elementy listy...",
			renderer, "text", LIST_ITEM, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));
	g_object_unref(store);
}

void add_to_list(GtkWidget *list, const gchar *str) {

	GtkListStore *store;
	GtkTreeIter iter;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, LIST_ITEM, str, -1);

}

void on_changed(GtkWidget *widget, gpointer label) {
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model,
			&iter)) {
		gtk_tree_model_get(model, &iter, LIST_ITEM, &value, -1);
		gtk_label_set_text(GTK_LABEL(label), value);
		g_free(value);
	}
}

int main(int argc, char **argv) {

	GtkWidget *window;
	GtkWidget *list;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkTreeSelection *selection;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	list = gtk_tree_view_new();

	gtk_window_set_title(GTK_WINDOW(window), "GtkTreeView - Spis cudzołożnic");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_window_set_default_size(GTK_WINDOW(window), 350, 200);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), TRUE);
	label = gtk_label_new("");
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
	gtk_box_pack_end(GTK_BOX(vbox), label, TRUE, TRUE, 2);
	gtk_box_set_homogeneous(GTK_BOX(vbox), FALSE);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	init_list(list);
	add_to_list(list, "Marlenka Misiaczek");
	add_to_list(list, "Beatka Kwiatuszek");
	add_to_list(list, "Agatka...");
	add_to_list(list, "Natalka...");
	add_to_list(list, "Iwonka...");

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

	g_signal_connect(selection, "changed", G_CALLBACK(on_changed), label);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
			NULL);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}

