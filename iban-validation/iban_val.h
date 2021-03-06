/*
 * iban_val.h
 *
 *  Created on: 30 mar 2018
 *      Author: majsterklepka
 */

#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmp.h>
#include <assert.h>


#ifndef IBAN_VAL_H_
#define IBAN_VAL_H_

#define UI_ELEMENT(TYPE, ELEMENT)	TYPE *ELEMENT = (TYPE *) get_ui_element(app, #ELEMENT);

typedef struct app_ {
	GtkBuilder *builder;
	GSList *objects;
	GtkApplication *application;
}App;


void app_init(App *);

GObject *get_ui_element(App*, const gchar *);

#endif /* IBAN_VAL_H_ */
