/*
 * print.c
 *
 * Created on: nie, 24 mar 2019, 04:38:58 CET 
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
 
#include <gtk/gtk.h>
#include <pango/pangocairo.h>
#include <glib/gi18n.h>
#include <locale.h>
#include <langinfo.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "print.h"


double start = 4.0;
double step = 4.80;
double pole_height = 6.00;

void
check_transfer_payment(cairo_t *cr, double x_margin, double y_margin,
		gboolean check)
{
		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_set_line_width(cr, 1.0);
		if (check)
		{
			cairo_move_to(cr, x_margin + start + 6 * step,
					y_margin + 24.5 + pole_height + 2.5);
			cairo_line_to(cr, x_margin + start + 7 * step,
					y_margin + 24.5 + 2*pole_height + 2.5);
			cairo_stroke(cr);
			cairo_move_to(cr, x_margin + start + 6 * step,
						y_margin + 24.5 + 2*pole_height + 2.5);
			cairo_line_to(cr, x_margin + start + 7 * step,
						y_margin + 24.5 + pole_height + 2.5);
			cairo_stroke(cr);
		}else{
			cairo_move_to(cr, x_margin + start + 7 * step,
						y_margin + 24.5 + pole_height + 2.5);
			cairo_line_to(cr, x_margin + start + 8 * step,
						y_margin + 24.5 + 2*pole_height + 2.5);
			cairo_stroke(cr);
			cairo_move_to(cr, x_margin + start + 7 * step,
						y_margin + 24.5 + 2*pole_height + 2.5);
			cairo_line_to(cr, x_margin + start + 8 * step,
						y_margin + 24.5 + pole_height + 2.5);
			cairo_stroke(cr);
		}

}

void
draw_form_description(cairo_t *cr, PangoLayout *layout, double x_margin,
		double y_margin, gchar *waluta)
{
		PangoFontDescription *desc;

		gint i;

		desc = pango_font_description_from_string("Sans Bold 10");
		pango_layout_set_font_description(layout, desc);
		pango_font_description_free(desc);

		cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
		pango_layout_set_width(layout, 5 * PANGO_SCALE);
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
		gchar *wp = _("PT");
		
		for (i = 0; i < 2; i++) {
			cairo_move_to(cr, x_margin + start + (6 + i) * step,
					y_margin + 24.5 + pole_height + 3.6);
			pango_layout_set_text(layout, g_utf8_substring(wp, i, i + 1), -1);
			pango_cairo_show_layout(cr, layout);
		}
		
				
		// -------------------------------------------------

		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		desc = pango_font_description_from_string("Sans 12");
		pango_layout_set_font_description(layout, desc);
		pango_font_description_free(desc);

		for (i = 0; i < 3; i++) {
			cairo_move_to(cr, x_margin + start + (10 + i) * step,
					y_margin + 24.5 + pole_height + 3.3);
			pango_layout_set_text(layout, g_utf8_substring(waluta, i, i + 1), -1);
			pango_cairo_show_layout(cr, layout);
		}

		desc = pango_font_description_from_string("Sans Bold 5");
		pango_layout_set_font_description(layout, desc);
		pango_font_description_free(desc);
		pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);

		gchar *pole_01 = _("01. name and address of the recipient");
		gchar *pole_02 = _("02. name and address of the recipient continued");
		gchar * pole_03 = _("03. number of accounts banking recipient");
		gchar *pole_04 = _("04.");
		gchar *pole_05 = _("05. currency");
		gchar *pole_06 = _("06. amount");
		gchar *pole_07 = _("07. number of accounts bank's liability / amount in words");
		gchar *pole_08 = _("08. name and address of the obligor");
		gchar *pole_09 = _("09. name and address of the obligee continued");
		gchar * pole_10 = _("10. the title of payment / transfer");
		gchar *pole_11 = _("11. the title of payment / transfer continued");
		gchar *left_side = _("transfer order / cash payment");
		gchar *right_side = _("part for the obligated bank");
		gchar *right_side_1 = _("part for the obligor");
		gchar *footer = _("12. stamp, date and signature of the principal");
		gchar *oplata = _("13. commission");
		gchar *copyrightsholder = "Bank Cash Deposit\nv1.50.1 GNU GPLv3\n©2017-2019 Paweł Sobótka";

		pango_layout_set_width(layout, ((g_utf8_strlen(pole_01, -1)) * 1.2) * PANGO_SCALE);
		cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
		cairo_rectangle(cr, x_margin + start, y_margin + 4.6, (g_utf8_strlen(pole_01, -1)) * 1.2, 2.0);
		cairo_fill(cr);
		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_move_to(cr, x_margin + start, y_margin + 4.6);
		pango_layout_set_text(layout, pole_01, -1);
		pango_cairo_show_layout(cr, layout);

		cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
		pango_layout_set_width(layout,
				((g_utf8_strlen(pole_02, -1)) * 1.1) * PANGO_SCALE);
		cairo_rectangle(cr, x_margin + start, y_margin + 12.6,
				(g_utf8_strlen(pole_02, -1)) * 1.2, 2.0);
		cairo_fill(cr);
		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_move_to(cr, x_margin + start, y_margin + 12.6);
		pango_layout_set_text(layout, pole_02, -1);
		pango_cairo_show_layout(cr, layout);

		cairo_move_to(cr, x_margin + start + 0.8, y_margin + 21.00);
		pango_layout_set_width(layout,
				((g_utf8_strlen(pole_03, -1)) * 1.2) * PANGO_SCALE);
		pango_layout_set_text(layout, pole_03, -1);
		pango_cairo_show_layout(cr, layout);

		pango_layout_set_width(layout, 15 * PANGO_SCALE);
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
		cairo_move_to(cr, x_margin + start + 0.8 + 25.00, y_margin + 30.50);
		pango_layout_set_text(layout, pole_04, 15 * PANGO_SCALE);
		pango_cairo_show_layout(cr, layout);

		pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
		cairo_move_to(cr, x_margin + start + 0.8 + 49.00, y_margin + 30.50);
		pango_layout_set_width(layout, ((g_utf8_strlen(pole_05, -1)) * 1.2) * PANGO_SCALE);
		pango_layout_set_text(layout, pole_05, -1);
		pango_cairo_show_layout(cr, layout);

		cairo_move_to(cr, x_margin + start + 0.8 + 67.00, y_margin + 30.8);
		pango_layout_set_width(layout, ((g_utf8_strlen(pole_06, -1)) * 1.2) * PANGO_SCALE);
		pango_layout_set_text(layout, pole_06, -1);
		pango_cairo_show_layout(cr, layout);

		cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
		pango_layout_set_width(layout, ((g_utf8_strlen(pole_07, -1)) * 1.2) * PANGO_SCALE);
		cairo_rectangle(cr, x_margin + start, y_margin + 40.00, (g_utf8_strlen(pole_07, -1)) * 1.2, 2.0);
		cairo_fill(cr);
		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_move_to(cr, x_margin + start, y_margin + 40.00);
		pango_layout_set_text(layout, pole_07, -1);
		pango_cairo_show_layout(cr, layout);

		cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
		pango_layout_set_width(layout, ((g_utf8_strlen(pole_08, -1)) * 1.2) * PANGO_SCALE);
		cairo_rectangle(cr, x_margin + start, y_margin + 48.00,
				(g_utf8_strlen(pole_08, -1)) * 1.2, 2.0);
		cairo_fill(cr);
		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_move_to(cr, x_margin + start, y_margin + 48.00);
		pango_layout_set_text(layout, pole_08, -1);
		pango_cairo_show_layout(cr, layout);

		cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
		pango_layout_set_width(layout, ((g_utf8_strlen(pole_09, -1)) * 1.2) * PANGO_SCALE);
		cairo_rectangle(cr, x_margin + start, y_margin + 54.80,	(g_utf8_strlen(pole_09, -1)) * 1.2, 2.0);
		cairo_fill(cr);
		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_move_to(cr, x_margin + start, y_margin + 54.80);
		pango_layout_set_text(layout, pole_09, -1);
		pango_cairo_show_layout(cr, layout);

		cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
		pango_layout_set_width(layout, ((g_utf8_strlen(pole_10, -1)) * 1.2) * PANGO_SCALE);
		cairo_rectangle(cr, x_margin + start, y_margin + 61.80,	(g_utf8_strlen(pole_10, -1)) * 1.2, 2.0);
		cairo_fill(cr);
		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_move_to(cr, x_margin + start, y_margin + 61.80);
		pango_layout_set_text(layout, pole_10, -1);
		pango_cairo_show_layout(cr, layout);

		cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
		pango_layout_set_width(layout, ((g_utf8_strlen(pole_11, -1)) * 1.2) * PANGO_SCALE);
		cairo_rectangle(cr, x_margin + start, y_margin + 69.00, (g_utf8_strlen(pole_11, -1)) * 1.2, 2.0);
		cairo_fill(cr);
		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_move_to(cr, x_margin + start, y_margin + 69.00);
		pango_layout_set_text(layout, pole_11, -1);
		pango_cairo_show_layout(cr, layout);

		cairo_move_to(cr, x_margin + start + 2.00, y_margin + 95.00);
		pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
		pango_layout_set_width(layout, ((g_utf8_strlen(footer, -1)) * 1.2) * PANGO_SCALE);
		pango_layout_set_text(layout, footer, ((g_utf8_strlen(footer, -1)) * 1.2) * PANGO_SCALE);
		pango_cairo_show_layout(cr, layout);

		cairo_move_to(cr, x_margin + start + 70.00, y_margin + 90.00);
		pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
		pango_layout_set_width(layout, ((g_utf8_strlen(oplata, -1)) * 1.2) * PANGO_SCALE);
		pango_layout_set_text(layout, oplata, ((g_utf8_strlen(oplata, -1)) * 1.2) * PANGO_SCALE);
		pango_cairo_show_layout(cr, layout);
		
		cairo_move_to(cr, x_margin + start + 64.50, y_margin + 78.50);
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
		pango_layout_set_width(layout, 34 * PANGO_SCALE);
		pango_layout_set_text(layout, copyrightsholder, ((g_utf8_strlen(copyrightsholder, -1)) * 1.2) * PANGO_SCALE);
		pango_cairo_show_layout(cr, layout);
		
		cairo_move_to(cr, x_margin + start + 70.00, y_margin + 95.00);
		cairo_line_to(cr, x_margin + start + 0.8 + 70.00 + 15.00, y_margin + 95.00);
		cairo_stroke(cr);

		cairo_move_to(cr, x_margin + start / 4.0, y_margin + 63.00);
		cairo_rotate(cr, -1.57);
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
		pango_layout_set_width(layout, ((g_utf8_strlen(left_side, -1)) * 1.2) * PANGO_SCALE);
		pango_layout_set_text(layout, left_side, ((g_utf8_strlen(left_side, -1)) * 1.2) * PANGO_SCALE);
		pango_cairo_show_layout(cr, layout);
		cairo_rotate(cr, 1.57);

		cairo_move_to(cr, x_margin + (start / 4.00) + 135.00, y_margin + 63.00);
		cairo_rotate(cr, -1.57);
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);

		if (y_margin < 95.00) {
			pango_layout_set_width(layout, ((g_utf8_strlen(right_side, -1)) * 1.2) * PANGO_SCALE);
			pango_layout_set_text(layout, right_side, ((g_utf8_strlen(right_side, -1)) * 1.2) * PANGO_SCALE);
			pango_cairo_show_layout(cr, layout);
		} else {
			pango_layout_set_width(layout,((g_utf8_strlen(right_side_1, -1)) * 1.2) * PANGO_SCALE);
			pango_layout_set_text(layout, right_side_1, ((g_utf8_strlen(right_side_1, -1)) * 1.2) * PANGO_SCALE);
			pango_cairo_show_layout(cr, layout);
		}

		cairo_rotate(cr, 1.57);

		cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
		cairo_rectangle(cr, x_margin + start + 119.50, y_margin + 76.80, 10.00,
				7.00);
		cairo_fill(cr);

		desc = pango_font_description_from_string("Sans Bold 12");
		pango_layout_set_font_description(layout, desc);
		pango_font_description_free(desc);

		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		cairo_move_to(cr, x_margin + start + 119.50, y_margin + 77.80);
		pango_layout_set_width(layout, 10 * PANGO_SCALE);
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
		pango_layout_set_text(layout, "07", 10 * PANGO_SCALE);
		pango_cairo_show_layout(cr, layout);

}

void
draw_description_one(cairo_t *cr, PangoLayout *layout, double x_margin,
		double y_margin, gchar *text)
{
		PangoFontDescription *desc;

		gint i;
		gint len;

		desc = pango_font_description_from_string("Courier 12");
		pango_layout_set_font_description(layout, desc);
		pango_font_description_free(desc);

		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		pango_layout_set_width(layout, 5 * PANGO_SCALE);
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
		if ( text != NULL ) {
			len = g_utf8_strlen(text, -1);
		}else{
			len = 0;
		}
		gint w = 0;
		gchar *c_out;

		if (len > 54) {
			len = 54;
		}

		for (i = 0; i < len; i++) {
			if (i < 27) {
				cairo_move_to(cr, x_margin + start + step * i, y_margin + 7.0);
				if (g_unichar_iswide(*(text + i)))
					c_out = g_utf8_substring(text, i, i + 2);
				else
					c_out = g_utf8_substring(text, i, i + 1);
				pango_layout_set_text(layout, c_out, -1);
				pango_cairo_show_layout(cr, layout);
			} else {
				cairo_move_to(cr, x_margin + start + step * w,
						y_margin + 7.0 + pole_height + 2.2);
				if (g_unichar_iswide(*(text + i)))
					c_out = g_utf8_substring(text, i, i + 2);
				else
					c_out = g_utf8_substring(text, i, i + 1);
				pango_layout_set_text(layout, c_out, -1);
				pango_cairo_show_layout(cr, layout);
				w++;
			}
		}

}

void
draw_description_two(cairo_t *cr, PangoLayout *layout, double x_margin,
		double y_margin, gchar *text)
{
		PangoFontDescription *desc;

		gint i;
		gint len;

		desc = pango_font_description_from_string("Courier 12");
		pango_layout_set_font_description(layout, desc);
		pango_font_description_free(desc);

		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		pango_layout_set_width(layout, 5 * PANGO_SCALE);
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
		if ( text != NULL )
		{
			len = g_utf8_strlen(text, -1);
		}else{
			len = 0;
		}
		gint w = 0;
		gchar *c_out;

		if (len > 54) {
			len = 54;
		}

		for (i = 0; i < len; i++) {
			if (i < 27) {
				cairo_move_to(cr, x_margin + start + step * i, y_margin + 7.0);
				if (g_unichar_iswide(*(text + i)))
					c_out = g_utf8_substring(text, i, i + 2);
				else
					c_out = g_utf8_substring(text, i, i + 1);
				pango_layout_set_text(layout, c_out, -1);
				pango_cairo_show_layout(cr, layout);
			} else {
				cairo_move_to(cr, x_margin + start + step * w,
						y_margin + 7.0 + pole_height + 1.2);
				if (g_unichar_iswide(*(text + i)))
					c_out = g_utf8_substring(text, i, i + 2);
				else
					c_out = g_utf8_substring(text, i, i + 1);
				pango_layout_set_text(layout, c_out, -1);
				pango_cairo_show_layout(cr, layout);
				w++;
			}
		}

}

void
draw_iban(cairo_t *cr, PangoLayout *layout, double x_margin,
		double y_margin, gchar *text)
{
		PangoFontDescription *desc;

		gint i;
		gint len;
		float cor = 0.6;
		
		desc = pango_font_description_from_string("Sans 12");
		pango_layout_set_font_description(layout, desc);
		pango_font_description_free(desc);

		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		pango_layout_set_width(layout, 4 * PANGO_SCALE);
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
		if ( text != NULL )
		{	
			len = g_utf8_strlen(text, -1);
		}else{
			len = 0;
		}
		gchar *c_out;

		if (len > 28) {
			len = 28;
		}

		for (i = 0; i < len; i++) {
				cairo_move_to(cr, x_margin + start + (step - cor) * i , y_margin + 6.3);
				if (g_unichar_iswide(*(text + i)))
					c_out = g_utf8_substring(text, i, i + 2);
				else
					c_out = g_utf8_substring(text, i, i + 1);
				pango_layout_set_text(layout, c_out, -1);
				pango_cairo_show_layout(cr, layout);
		}
}

void
draw_amount(cairo_t *cr, PangoLayout *layout, double x_margin,
		double y_margin, gchar *text)
{
		PangoFontDescription *desc;

		gint i;
		gint len;

		desc = pango_font_description_from_string("Courier 12");
		pango_layout_set_font_description(layout, desc);
		pango_font_description_free(desc);

		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		pango_layout_set_width(layout, 5 * PANGO_SCALE);
		pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
		if ( text != NULL )
		{	
			len = g_utf8_strlen(text, -1);
		}else{
			len = 0;
		}
		
		gchar *c_out;

		if (len > 12) {
			len = 12;
		}

		for (i = 0; i < len; i++) {
				cairo_move_to(cr, x_margin + start + step * i, y_margin + 7.0);
				if (g_unichar_iswide(*(text + i)))
					c_out = g_utf8_substring(text, i, i + 2);
				else
					c_out = g_utf8_substring(text, i, i + 1);
				pango_layout_set_text(layout, c_out, -1);
				pango_cairo_show_layout(cr, layout);
		}
}

void
draw_form(cairo_t *cr, PangoLayout *layout, double x_margin, double y_margin, FormData *form_data)
{
			
		gint i, l, start_1;
				
		double cr_width = 140.00;
		double cr_height = 200.00;
				
		x_margin += 0.3;
		y_margin += 0.3;
		
		for (l = 0; l < 2; l++) {

			y_margin += l * (cr_height / 2.00);

			cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
			cairo_rectangle(cr, x_margin, y_margin, cr_width, cr_height);
			cairo_fill(cr);
			cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
			cairo_rectangle(cr, x_margin + start + 1.2, y_margin + 4.00, cr_width * 0.93,
					0.83 * (cr_height / 2.00));
			cairo_fill(cr);

			cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
			cairo_set_line_width(cr, 0.2);
			cairo_rectangle(cr, x_margin, y_margin, cr_width, cr_height / 2.00);
			cairo_stroke(cr);

			cairo_set_line_width(cr, 1.2);
			cairo_move_to(cr, x_margin + start, y_margin + 12.00);
			cairo_rel_line_to(cr, 0.0, -8.00);
			cairo_rel_line_to(cr, 8.0, 0.0);

			cairo_move_to(cr, x_margin + cr_width * 0.92, y_margin + 4.00);
			cairo_rel_line_to(cr, 8.00, 0.00);
			cairo_rel_line_to(cr, 0, 8.00);

			cairo_move_to(cr, x_margin + start,
					y_margin + 0.69 * (cr_height / 2.00));
			cairo_rel_line_to(cr, 0.00, 8.00);
			cairo_rel_line_to(cr, 8.00, 0.0);
			cairo_stroke(cr);

			// -----------------------------------------------------
			start += 1.20;
			cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
			cairo_rectangle(cr, x_margin + start, y_margin + 5.30, 27 * step,
					pole_height);
			cairo_rectangle(cr, x_margin + start, y_margin + 13.10, 27 * step,
					pole_height);
			cairo_rectangle(cr, x_margin + start, y_margin + 20.50 + 2.5 + 0.4,
					26 * step, pole_height);
			cairo_rectangle(cr, x_margin + start + 14 * step,
					y_margin + 24.5 + pole_height + 2.5 + 0.4, step * 12.00,
					pole_height);
			cairo_rectangle(cr, x_margin + start + 6 * step,
					y_margin + 24.5 + pole_height + 2.5, 2 * step, pole_height);
			cairo_rectangle(cr, x_margin + start + 10 * step,
					y_margin + 24.5 + pole_height + 2.5, 3 * step, pole_height);
			cairo_rectangle(cr, x_margin + start, y_margin + 41.20, 26 * step,
					pole_height);
			cairo_rectangle(cr, x_margin + start, y_margin + 49.30, 27 * step,
					pole_height);
			cairo_rectangle(cr, x_margin + start, y_margin + 56.30, 27 * step,
					pole_height);
			cairo_rectangle(cr, x_margin + start, y_margin + 63.30, 27 * step,
					pole_height);
			cairo_rectangle(cr, x_margin + start, y_margin + 70.30, 27 * step,
					pole_height);
			cairo_fill(cr);
			// ------------------------------------------------------

			cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
			cairo_set_line_width(cr, 0.30);
			cairo_move_to(cr, x_margin + start, y_margin + pole_height + 5.50);
			cairo_rel_line_to(cr, 0.00, -pole_height);
			for (i = 0; i < 27; i++) {
				cairo_move_to(cr, x_margin + start + step * i, y_margin + 5.50);
				cairo_rel_line_to(cr, 0.00 + step, 0.00);
				cairo_rel_line_to(cr, 0.00, pole_height);
				cairo_rel_line_to(cr, -step, 0.00);
			}

			cairo_move_to(cr, x_margin + start + 6 * step,
					y_margin + 24.5 + 2.5 + pole_height);
			cairo_rel_line_to(cr, 0.00, pole_height);
			for (i = 0; i < 2; i++) {
				cairo_move_to(cr, x_margin + start + step * 6 + step * i,
						y_margin + 24.5 + 2.5 + pole_height);
				cairo_rel_line_to(cr, 0.00 + step, 0.00);
				cairo_rel_line_to(cr, 0.00, pole_height);
				cairo_rel_line_to(cr, -step, 0.00);
			}
			cairo_stroke(cr);

			cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
			cairo_set_line_width(cr, 0.3);
			for (i = 0; i < 2; i++) {
				cairo_move_to(cr, x_margin + start + step * 11 + step * i,
						y_margin + 24.5 + 2.5 + pole_height);
				cairo_rel_line_to(cr, 0.0, pole_height);
			}
			cairo_stroke(cr);

			cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
			cairo_move_to(cr, x_margin + start, y_margin + 11.20 + 2.00);
			cairo_rel_line_to(cr, 0.00, pole_height);
			for (i = 0; i < 27; i++) {
				cairo_move_to(cr, x_margin + start + step * i,
						y_margin + 11.2 + pole_height + 2);
				cairo_rel_line_to(cr, 0.00 + step, 0.00);
				cairo_rel_line_to(cr, 0.00, -pole_height);
				cairo_rel_line_to(cr, -step, 0.00);
			}
			cairo_stroke(cr);

			cairo_set_line_width(cr, 0.8);
			cairo_rectangle(cr, x_margin + start, y_margin + 20.5, step * 26.00,
					pole_height + 2.50);
			cairo_stroke(cr);

			// -------------------------------------------------		
			cairo_set_line_width(cr, 0.8);
			cairo_rectangle(cr, x_margin + start + 14 * step,
					y_margin + 24.5 + pole_height, step * 12.00,
					pole_height + 2.50);
			cairo_stroke(cr);

			cairo_set_line_width(cr, 0.3);
			cairo_move_to(cr, x_margin + start + 14 * step,
					y_margin + 24.5 + pole_height + 2.50);
			cairo_rel_line_to(cr, 0.00, pole_height);
			i = 0;
			while (i < 12) {
				cairo_move_to(cr, x_margin + start + 14 * step + step * i,
						y_margin + 24.5 + pole_height + 2.50);
				cairo_rel_line_to(cr, 0.00 + step, 0.00);
				cairo_rel_line_to(cr, 0.00, pole_height);
				cairo_rel_line_to(cr, -step, 0.00);
				i++;
			}
			cairo_stroke(cr);
			// -------------------------------------------------
			cairo_move_to(cr, x_margin + start, y_margin + 49.00);
			cairo_rel_line_to(cr, 0.00, pole_height);
			for (i = 0; i < 27; i++) {
				cairo_move_to(cr, x_margin + start + step * i, y_margin + 49.00);
				cairo_rel_line_to(cr, 0.00 + step, 0.00);
				cairo_rel_line_to(cr, 0.00, pole_height);
				cairo_rel_line_to(cr, -step, 0.00);
			}
			// -------------------------------------------------
			cairo_move_to(cr, x_margin + start, y_margin + 56.00);
			cairo_rel_line_to(cr, 0.00, pole_height);
			for (i = 0; i < 27; i++) {
				cairo_move_to(cr, x_margin + start + step * i, y_margin + 56.00);
				cairo_rel_line_to(cr, 0.00 + step, 0.00);
				cairo_rel_line_to(cr, 0.00, pole_height);
				cairo_rel_line_to(cr, -step, 0.00);
			}
			// --------------------------------------------------
			cairo_move_to(cr, x_margin + start, y_margin + 63.00);
			cairo_rel_line_to(cr, 0.00, pole_height);
			for (i = 0; i < 27; i++) {
				cairo_move_to(cr, x_margin + start + step * i, y_margin + 63.00);
				cairo_rel_line_to(cr, 0.00 + step, 0.00);
				cairo_rel_line_to(cr, 0.00, pole_height);
				cairo_rel_line_to(cr, -step, 0.00);
			}
			// --------------------------------------------------
			cairo_move_to(cr, x_margin + start, y_margin + 70.00);
			cairo_rel_line_to(cr, 0.00, pole_height);
			for (i = 0; i < 27; i++) {
				cairo_move_to(cr, x_margin + start + step * i, y_margin + 70.00);
				cairo_rel_line_to(cr, 0.00 + step, 0.00);
				cairo_rel_line_to(cr, 0.00, pole_height);
				cairo_rel_line_to(cr, -step, 0.00);
			}
			cairo_stroke(cr);

			cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
			cairo_rectangle(cr, x_margin + start, y_margin + 78.00, 14.00 * step,
					20.00);
			cairo_fill(cr);
			cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
			cairo_rectangle(cr, x_margin + start, y_margin + 78.00, 14.00 * step,
					20.00);
			cairo_stroke(cr);

			cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
			cairo_rectangle(cr, x_margin + start + 20 * step, y_margin + 78.00,
					20.00, 20.00);
			cairo_fill(cr);
			cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
			cairo_rectangle(cr, x_margin + start + 20 * step, y_margin + 78.00,
					20.00, 20.00);
			cairo_stroke(cr);
			
			draw_description_one(cr, layout, x_margin, y_margin, form_data->addres1);
			draw_iban(cr, layout, x_margin,y_margin + 17.5, form_data->iban1);
			draw_amount(cr, layout, x_margin + 14 * step, y_margin + 27.50, form_data->kwota);
			draw_iban(cr, layout, x_margin, y_margin + 36.00, form_data->iban2);
			draw_description_two(cr, layout, x_margin, y_margin + 44.00, form_data->addres2);
			draw_description_two(cr, layout, x_margin, y_margin + 58.00, form_data->tytul);
			draw_form_description(cr, layout, x_margin, y_margin, form_data->waluta);
			check_transfer_payment(cr, x_margin, y_margin, form_data->trans);
		}

}
