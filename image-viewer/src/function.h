/*
 * function.h
 *
 *  Created on: 14 cze 2018
 *      Author: Paweł Sobótka
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <gtk/gtk.h>
#include <glib/gi18n.h>

void on_open_image(GtkButton* button, gpointer user_data);

GtkWidget* create_window(void);

#endif /* FUNCTION_H_ */
