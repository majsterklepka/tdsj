/*
 * main.c
 *
 *  Created on: 16 kwi 2019
 *      Author: majsterklepka
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>


enum
{
  COL_LP,
  COL_NAME,
  COL_AGE,
  COL_ADDRESS,
  COL_SALARY,
  NUM_COLS
} ;


void static parseStory(xmlDocPtr doc, xmlNodePtr cur, GtkListStore *store)
{
	xmlChar *data;
	GtkTreeIter iter;
	gtk_list_store_append (store, &iter);
	if ((!xmlStrcmp(cur->name, (const xmlChar *)"data"))) {
	    data = xmlGetProp(cur, (const xmlChar *)"rowid");
	    gtk_list_store_set (store, &iter,COL_LP, data,-1);
	    xmlFree(data);
	    }
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"name"))) {
		    data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			gtk_list_store_set (store, &iter,COL_NAME, data,-1);
			xmlFree(data);
		    }
		 if ((!xmlStrcmp(cur->name, (const xmlChar *)"age"))) {
		    data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    gtk_list_store_set (store, &iter,COL_AGE, data,-1);
		    xmlFree(data);
		    }
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"address"))) {
		    data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    gtk_list_store_set (store, &iter,COL_ADDRESS, data,-1);
		    xmlFree(data);
		    }
		 if ((!xmlStrcmp(cur->name, (const xmlChar *)"salary"))) {
		    data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    gtk_list_store_set (store, &iter,COL_SALARY, data,-1);
		    xmlFree(data);
		    }
 	cur = cur->next;
	}

    return;

}




void static read_data(GtkListStore *store)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile("example_data.xml");

	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "data_records")) {
		fprintf(stderr,"document of the wrong type, root node != data_records");
		xmlFreeDoc(doc);
		return;
	}

	cur = cur->xmlChildrenNode;

		while (cur != NULL) {
		parseStory(doc, cur, store);

		cur = cur->next;

		}

	xmlFreeDoc(doc);
	return;
}


static GtkWidget *create_view()
{
	GtkCellRenderer     *renderer;
	GtkWidget           *view;
	GtkListStore *store;
	store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

	view = gtk_tree_view_new ();

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
												   -1,
												   "lp",
												   renderer,
												   "text", COL_LP,
												   NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
												   -1,
												   "Dane odbiorcy",
												   renderer,
												   "text", COL_NAME,
												   NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
												   -1,
												   "Wiek",
												   renderer,
												   "text", COL_AGE,
												   NULL);


	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
												   -1,
												   "Adres",
												   renderer,
												   "text", COL_ADDRESS,
												   NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
												   -1,
												   "Kwota",
												   renderer,
												   "text", COL_SALARY,
												   NULL);


	gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(view), GTK_TREE_VIEW_GRID_LINES_BOTH);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), TRUE);
	read_data(store);

	gtk_tree_view_set_model (GTK_TREE_VIEW (view), GTK_TREE_MODEL(store));

	return view;
}


int
main (int argc, char **argv)
{
	GtkWidget *window;
	GtkWidget *view;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "XML reader by Paweł Sobótka");
	g_signal_connect (window, "delete_event", gtk_main_quit, NULL);
	view = create_view();


	gtk_container_add(GTK_CONTAINER(window), view);
	gtk_widget_show_all (window);

	gtk_main ();

	return 0;
}


