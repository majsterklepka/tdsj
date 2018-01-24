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

int main(int argc, char *argv[]) {
	char * locale_info = setlocale(LC_ALL, "");
	char *locale = "pl_PL.UTF-8";
	char *saved_locale = strdup(locale_info);
	double HbA1c = 0;
	float average_glycemia;
	if (strcmp(saved_locale, locale) != 0) {
		printf("I setup application language to : %s\n", locale);
		locale_info = setlocale(LC_ALL, locale);
		if (locale_info == NULL) {
			printf(
					"You don't have Polish locale installed!!!\nPlease, install it!\nExit!\n");
			return EXIT_FAILURE;
		}
	}
	char *info_line[] =
			{ "Witaj w aplikacji Cukrzyk!\n", "==========================\n",
					"Autor programu: Majster Klepka\n", "Licencja: GPL-3.0\n",
					"Prawa Autorskie: (C) 2018 Paweł Sobótka\n",
					"Użycie: \nwprowadź swój własny poziom \%HbA1c otrzymany z labolatorium analiz medycznych\n",
					"a jeśli go jeszcze nie posiadasz...\n",
					"to zgłoś się do lekarza POZ po skierowanie na badanie\n",
					"i niezwłocznie wykonaj badanie!\n",
					"----------------------------\n" };
	int i = 0;
	while (*(info_line + i) != NULL){
		printf("%s", info_line[i]);
		i++;
	}
	printf("wprowadź \%HbA1c: ");
	scanf("%lf", &HbA1c);
	average_glycemia = 28.7 * HbA1c - 46.7;
	printf("Twój wynik średniej trzymiesięcznej glikemii\nto %.2f mg/dL\n", average_glycemia);
	printf("----------------------------\n");
	printf("Dziękuję za skorzystanie z tego krótkiego programu....\nAutor.\n");

	return EXIT_SUCCESS;
}

