/*
 * print.h
 *
 * Created on: nie, 24 mar 2019, 04:39:23 CET 
 *    License: GPLv3 or later
 *     Author: Paweł Sobótka <48721262935pl@gmail.com>
 *  Copyright: © Paweł Sobótka, 2017-2019, all rights reserved
 *        URL: https://github.com/majsterklepka/BaCaDe.git
 *    Company: mgr inż. Paweł Sobótka, self-employed, individual creator 
 *    Address: POLAND, masovian, Szydłowiec, 26-500
 * NIP(taxid): 799-169-51-12
 *
 * This file is part of Bank Cash Deposit.
 *
 *   Bank Cash Deposit is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Bank Cash Deposit is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Bank Cash Deposit.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PRINT_H_
#define PRINT_H_


typedef struct {
	gchar *text_01;
	gchar *text_02;
	gchar *text_03;
	gchar *text_04;
	gchar *text_05;
	gchar *text_06;
	gboolean check;
	gint opt;
	PangoLayout *layout;
} PrintData;

void check_transfer_payment(cairo_t *cr, double x_margin, double y_margin, gboolean check);

void draw_form_description(cairo_t *cr, PangoLayout *layout, double x_margin, double y_margin, gint opt);

void draw_description_one(cairo_t *cr, PangoLayout *layout, double x_margin, double y_margin, gchar *text);

void draw_description_two(cairo_t *cr, PangoLayout *layout, double x_margin, double y_margin, gchar *text);

void draw_iban(cairo_t *cr, PangoLayout *layout, double x_margin, double y_margin, gchar *text);

void draw_amount(cairo_t *cr, PangoLayout *layout, double x_margin, double y_margin, gchar *text);

void draw_form(GtkPrintOperation *operation, GtkPrintContext *context, int page_nr, PrintData *print_data);



#endif /* PRINT_H_ */

