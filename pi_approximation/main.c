/*
 * main.c
 *
 *  Created on: 13 sty 2018
 *      Author: majsterklepka
 *     License: GPL-3.0
 */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main(void) {
	long double x, y;
	long double T = 0, R, ilosc, pi;

	srand(time(NULL));
	printf("Program aproksymuje wartość liczby PI\n");
	printf("robi to z wykorzystaniem metody Monte-Carlo\n");
	printf("Autor: Majster Klepka\nLicencja: GPL-3.0\n---\n");

	printf("ile punktów? : ");
	scanf("%Lf", &ilosc);

	for (R = 0; R <= ilosc; R++) {
		x = (double) rand() / RAND_MAX;
		y = (double) rand() / RAND_MAX;
		if ((x * x + y * y) <= 1)
			T++;
	}
	pi = (4 * T) / R;
	printf("---\nLiczba PI = %Lf\n---\n", pi);
	return EXIT_SUCCESS;

}

