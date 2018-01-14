/*
 * parabole.c
 *
 *  Created on: 13 sty 2018
 *      Author: majsterklepka
 *     License: GPL-3.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int file_exist(const char *file);

int
main(int argc, char ** argv){

	FILE *fp;
	long double x1, x2;
	char* file = malloc(32*sizeof(char));
	long double y, x;
	long int ilosc = 0, k = 0;
	long double przedzial;
	long double krok;
	printf("Program generuje serię danych\ndla równania y= x^2\nAutor: Majster Klepka\nLicencja: GPL-3.0\n---\n");
	printf("podaj nazwę pliku wyjściowego : ");
	scanf("%s", file);

	if (file_exist(file)){
		printf("plik o nazwie %s istnieje!\nKończę działanie programu...", file);
		return EXIT_FAILURE;
	}

	fp = fopen(file, "w");

	printf("podaj zakres zmiennej x(x1 x2): ");

	scanf("%Lf %Lf", &x1, &x2);

	przedzial = x2 - x1;

	printf("podaj liczbę kroków: ");
	scanf("%ld", &ilosc);
	krok = przedzial / ilosc;

	x = x1;
	while(k <= ilosc){
		y = x*x;
		fprintf(fp, "%Lf\t%Lf\n", x, y);
		x += krok;
		k += 1;
	}

	printf("Zakończono gewnerowanie serii danych...\n");
	fclose(fp);
	free(file);

	return EXIT_SUCCESS;
}

int file_exist(const char *file){
	FILE *fop;
	int odp = 0;
	fop = fopen(file, "r");

	if (fop != NULL){
		odp = 1;
		fclose(fop);
	}else
		odp = 0;

	return odp;
}
