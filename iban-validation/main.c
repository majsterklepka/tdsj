/*
 * main.c
 *
 *  Created on: 29 mar 2018
 *      Author: Paweł Sobótka
 */

#include "iban_val.h"
#define V(cc, exp) if (!strncmp(iban, cc, 2)) return len == exp

void app_init(App *app)
{
	app->builder = gtk_builder_new();

	gtk_builder_add_from_file(app->builder, "interface.glade", NULL);

	app->objects = gtk_builder_get_objects(app->builder);
	gtk_builder_connect_signals(app->builder, app);

}

GObject *get_ui_element(App *app, const gchar *name)
{

	const gchar *widget;
	GSList *list;

	list = app->objects;

	do{
		widget = gtk_buildable_get_name(list->data);
		if (g_strcmp0(widget, name) == 0){
			return list->data;
		}
	}while((list = g_slist_next(list)));

	g_slist_free(list);

	return NULL;
}

void
insert_text_handler (GtkEditable *editable,
                     const gchar *text,
                     gint         length,
                     gint        *position,
                     gpointer     data)
{
	gchar *result = g_utf8_strup (text, length);


	g_signal_handlers_block_by_func (editable,
                               (gpointer) insert_text_handler, data);
	gtk_editable_insert_text (editable, result, length, position);
	g_signal_handlers_unblock_by_func (editable,
                                     (gpointer) insert_text_handler, data);

	g_signal_stop_emission_by_name (editable, "insert_text");

	g_free (result);
}

void string_space(const char *input, char *output)
{
    int i,j;
    j = 0;
    if ( output == 0 ){
		fprintf(stderr, "virtual memory exceeded!\n");
		return;
	}

    for	(i = 0; i < strlen(output); i++)
    {
		output[i] = '\0';
    }
    for (i = 0; i<strlen(input); i++)
    {
        if (!isspace(input[i])){
          output[j]=input[i];
          j++;
	}
    }
    output[j]='\0';
}

int valid_cc(const char *iban, int len)
{
    V("AL", 28); V("AD", 24); V("AT", 20); V("AZ", 28); V("BE", 16); V("BH", 22); V("BA", 20); V("BR", 29);
    V("BG", 22); V("CR", 21); V("HR", 21); V("CY", 28); V("CZ", 24); V("DK", 18); V("DO", 28); V("EE", 20);
    V("FO", 18); V("FI", 18); V("FR", 27); V("GE", 22); V("DE", 22); V("GI", 23); V("GR", 27); V("GL", 18);
    V("GT", 28); V("HU", 28); V("IS", 26); V("IE", 22); V("IL", 23); V("IT", 27); V("KZ", 20); V("KW", 30);
    V("LV", 21); V("LB", 28); V("LI", 21); V("LT", 20); V("LU", 20); V("MK", 19); V("MT", 31); V("MR", 27);
    V("MU", 30); V("MC", 27); V("MD", 24); V("ME", 22); V("NL", 18); V("NO", 15); V("PK", 24); V("PS", 29);
    V("PL", 28); V("PT", 25); V("RO", 24); V("SM", 27); V("SA", 24); V("RS", 22); V("SK", 24); V("SI", 19);
    V("ES", 24); V("SE", 24); V("CH", 21); V("TN", 24); V("TR", 26); V("AE", 23); V("GB", 22); V("VG", 24);

    return 0;
}


int iban_validation(const char *iban)
{
	int i, j, l, sz;
	unsigned long int resp;
	mpz_t n;//biginteger
	mpz_t o;//biginteger
	int flag;
	l = 0;

	sz = strlen(iban);

	for (i = 0; i < sz; i++) {
		if (!isdigit(iban[i]) && !isupper(iban[i]))
			return 0;
		l += isupper(iban[i]);
	}

	if (!valid_cc(iban, sz))
		return 0;

    char *number = (char*)malloc(sz*sizeof(char));
	if ( number == 0 ){
		fprintf(stderr, "virtual memory exceeded!\n");
		return -1;
	}
    strcpy(number, iban + 4);
    strncpy(number + sz - 4, iban, 4);

    char *trans = (char*)malloc((sz + l)*sizeof(char));
	if ( trans == 0 ){
		fprintf(stderr, "virtual memory exceeded!\n");
		return -1;
	}
    trans[sz + l + 1] = 0;

	for (i = j = 0; i <strlen(number); ++i, ++j) {
		if (isdigit(number[i]))
			trans[j] = number[i];
		else {
			trans[j]   = (number[i] - 55) / 10 + '0';
			trans[++j] = (number[i] - 55) % 10 + '0';
		}
	}
	trans[j] = '\0';

	mpz_init(n);
	mpz_set_ui(n, 0);
	mpz_init(o);
	mpz_set_ui(o, 0);
	free(number);

	flag = mpz_set_str(n, trans, 10);
  	assert(flag == 0);

   	mpz_mod_ui(o,n, 97UL);// sum modulus

	resp = mpz_get_ui(o);//integer output

	mpz_clear(n);
	mpz_clear(o);
	free(trans);

	if (resp == 1){
		return 1;
	}

    return 0;
}


void entry1_changed_cb(GtkEditable *editable, gpointer user_data)
{

}

void show_icon(gint input, App *app)
{
	gchar *image_1 = g_build_path(G_DIR_SEPARATOR_S, "./icons", "ikona-ok-48x48.png", NULL);
	gchar *image_2 = g_build_path(G_DIR_SEPARATOR_S, "./icons", "ikona-error-48x48.png", NULL);
	UI_ELEMENT(GtkImage, image1);
	if (input == 0)
	{
		gtk_widget_hide(GTK_WIDGET(image1));
	}else if( input == 1){
		GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(image_1, NULL);
				gtk_image_set_from_pixbuf(image1, pixbuf);
				gtk_widget_show(GTK_WIDGET(image1));
	}else{
		GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(image_2, NULL);
				gtk_image_set_from_pixbuf(image1, pixbuf);
				gtk_widget_show(GTK_WIDGET(image1));
	}
}

void show_text(int input, App *app)
{
	UI_ELEMENT(GtkLabel, label1);
	const gchar *text1 = _("To nie jest numer IBAN");
	const gchar *text2 = _("To jest poprawny numer IBAN");
	const gchar *text3 = _("Wprowadź numer IBAN w pole powyżej\npamiętaj aby było to 28 znaków\nz dwuliterowym kodem kraju przed ciągiem cyfr\nmożesz wprowadzić IBAN rozdzielony spacjami na grupy...");
	const gchar *text4 = _("program wygenerował błąd");
	switch(input){
		case 0:
			gtk_label_set_text(label1, text1);
			break;
		case 1:
			gtk_label_set_text(label1, text2);
			break;
		case 2:
			gtk_label_set_text(label1, text4);
			break;
		default:
			gtk_label_set_text(label1, text3);
	}
}

void button1_clicked_cb(GtkWidget * widget, App *app)
{
	UI_ELEMENT(GtkEntry, entry1);
	gchar *text = (char*)malloc(40*sizeof(char));
	string_space(gtk_entry_get_text(entry1), text);
	int response = iban_validation(text);
	if (response == 1){
		show_text(response, app);
		show_icon(1, app);
	}else if (response == 0){
		show_text(response, app);
		show_icon(2, app);
	}else {
		show_text(2, app);
		show_icon(2, app);
	}
	g_free(text);

}



void button2_clicked_cb(GtkWidget * widget, App *app)
{
	UI_ELEMENT(GtkEntry, entry1);
	gtk_entry_set_text(entry1, " ");
	show_text(3,app);
	show_icon(0, app);

}

void button3_clicked_cb(GtkWidget * widget, App *app)
{
	g_application_quit(G_APPLICATION(app->application));
}

static void activate(GApplication *application, App *app) {

	UI_ELEMENT(GtkWindow, window);
	UI_ELEMENT(GtkButton, button1);
	UI_ELEMENT(GtkButton, button2);
	UI_ELEMENT(GtkButton, button3);
	UI_ELEMENT(GtkImage, image1);
	UI_ELEMENT(GtkLabel, label2);
	UI_ELEMENT(GtkLinkButton, linkbutton1);
	UI_ELEMENT(GtkEditable, entry1);

	gtk_editable_set_editable(entry1, TRUE);

	gtk_button_set_label(button1, _("Weryfikuj"));
	gtk_button_set_label(button2, _("Wyczyść"));
	gtk_button_set_label(button3, _("Zamknij"));

	gtk_label_set_text(label2, "(C)2018 Paweł Sobótka");
	gtk_button_set_label(GTK_BUTTON(linkbutton1), "MajsterKlepka GitHub Pages");
	gtk_link_button_set_uri(linkbutton1, "https://majsterklepka.github.io");

	show_text(3,app);

	gtk_widget_hide(GTK_WIDGET(image1));

	gtk_application_add_window(GTK_APPLICATION(app->application), window);
	gtk_widget_show(GTK_WIDGET(window));

}

int main(int argc, char **argv) {
	int status;
	App *app;
	app = (App*) g_new(App, 1);


	app->application = gtk_application_new("io.github.majsterklepka", G_APPLICATION_FLAGS_NONE);
	g_application_register(G_APPLICATION(app->application), NULL, NULL);

	app_init(app);

	g_signal_connect(app->application, "activate", G_CALLBACK (activate), app);
	status = g_application_run(G_APPLICATION(app->application), argc, argv);

	g_object_unref(app->application);
	g_object_unref(app->builder);
	g_slist_free(app->objects);

	return status;
}
