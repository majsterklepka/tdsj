/*
 * main.c
 *	28,7 x % HbA1c – 46,7 = average glycemia
 *  Created on: 24 sty 2018
 *      Author: majsterklepka
 *     License: GPL-3.0
 */

#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char * locale_info = setlocale(LC_ALL, "");
	char *locale = "pl_PL.UTF-8";
	char *saved_locale = strdup(locale_info);
	if (strcmp(saved_locale, locale) != 0){
		printf("I setup application language to : %s\n", locale);
		locale_info = setlocale(LC_ALL, locale);
		if (locale_info == NULL){
			printf("You don't have Polish locale installed!!!\nPlease, install it!\nExit!\n");
			return EXIT_FAILURE;
		}
	}
	printf ("ustawiono język aplikacji i kodowanie znaków\nna: \t\t\t\t%s\n", locale_info);

	return EXIT_SUCCESS;
}

