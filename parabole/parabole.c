/*
 * parabole.c
 *
 *  Created on: 13 sty 2018
 *      Author: majsterklepka
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char ** argv){


	FILE *fp;
	long double x1, x2;
	const char* file= "output.txt";
	long double y, x;
	long int ilosc = 0, k = 0;
	long double przedzial;
	long double krok;
	fp = fopen(file, "w");

	printf("podaj zakres x(x1 x2): ");

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

	fclose(fp);

	return EXIT_SUCCESS;
}
