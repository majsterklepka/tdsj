/* main.c
 *
 *	28,7 x % HbA1c – 46,7 = average glycemia
 *  Created on: 24 sty 2018
 *      Author: majsterklepka
 *     License: GPL-3.0
 */

#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <libintl.h>

/*! \file main.c
 source file with main body of aplications
 */

/*! \fn int main(int argc, char *argv[]) 
  processed calculation of average three-month glycemia
 
  appliaction has support for internationalization
  with use of gettext & speaks in two languages
  English and Polish. HbA1c is medical literal symbol
  of processed parametr
 */

/*!\f$average\_glycemia = 28.7 * HbA1c - 46.7\f$ */

/*! output is dispaly in mg/dL unit */

int main(int argc, char *argv[]) {
	
	setlocale(LC_ALL, "");
	bindtextdomain("glikemia", "./locale");
	textdomain("glikemia"); 
	
	double average_glycemia = 0; 
	double HbA1c; 
	
	printf( gettext("Welcome to the App Diabetes!\n"));
	printf(gettext("==========================\n"));
	printf(gettext("Author: Majster Klepka\n"));
	printf(gettext("License: GPL-3.0\n"));
	printf(gettext("Copyright: (C) 2018 Paweł Sobótka\n"));
	printf(gettext("Usage: \n\tenter your own level \%HbA1c obtained from\n\tthe medical analysis laboratory\n"));
	printf(gettext("\tand if you do not have it yet ...\n"));
	printf(gettext("\tthen report to your doctor for a referral\t\n"));
	printf(gettext("\tand take the test immediately!\n"));
	printf("----------------------------\n");
	printf(gettext("enter value of \% HbA1c: "));
	scanf("%lf", &HbA1c);
	average_glycemia = 28.7 * HbA1c - 46.7;
	printf(gettext("Your result of the average three-month glycemia\nthat is: %.2f mg/dL\n"), average_glycemia);
	printf("----------------------------\n");
	printf(gettext("Thank you for using this short program...\nAuthor.\n"));

	return EXIT_SUCCESS;
}

