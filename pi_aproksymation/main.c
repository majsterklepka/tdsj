/*
 * main.c
 *
 *  Created on: 13 sty 2018
 *      Author: majsterklepka
 */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int
main(void){
	long double x,y;
	long double T=0, R, ilosc, pi;

	srand(time(NULL));

	printf("ile punktów? : ");
	scanf("%Lf", &ilosc);

	for (R = 0; R <= ilosc; R++){
		x = (double)rand()/RAND_MAX;
		y = (double)rand()/RAND_MAX;
		if ((x*x + y*y)<=1)
			T++;
		}
	pi = (4*T)/R;
	printf("Liczba PI = %Lf\n", pi);
	return EXIT_SUCCESS;

}

