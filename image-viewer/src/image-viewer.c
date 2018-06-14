/*
 ============================================================================
 Name        : image-viewer.c
 Author      : Paweł Sobótka
 Version     : 0.0.1
 Copyright   : GNU GPL-3.0
 Description : image viewer in gtk+
 ============================================================================
 */

#include "function.h"

int main(int argc, char *argv[]) {
	GtkWidget *window;

	gtk_init(&argc, &argv);

	window = create_window();

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
