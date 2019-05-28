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
	GList *data;
	gint data_length;
} PrintData;

typedef struct {
	gchar *addres1;
	gchar *iban1;
	gchar *kwota;
	gchar *iban2;
	gchar *addres2;
	gchar *tytul;
	gchar *waluta;
	gboolean trans;	
} FormData;


void draw_form(cairo_t *cr, PangoLayout *layout, double x_margin, double y_margin, FormData *form_data);


#endif /* PRINT_H_ */

