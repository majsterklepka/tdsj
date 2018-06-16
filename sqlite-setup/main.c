/*
 * main.c
 *
 *  Created on: 16 cze 2018
 *      Author: Paweł Sobótka
 *     License: GNU GPL-3.0
 *Descriptions: Setup SQLite example application
 */

#include "sqlite-setup.h"


int main(void)
{
	sqlite3 *db;
	int rc;
	char *sql_query;
	char *errMsg;

	printf("Oprogramowanie zainicjuje bazę danych opartą o SQLite\nUtworzy tabelę gdy takiej tabeli nie znajdzie\n");

	/*otwarcie, utworzenie pliku bazy danych*/
	rc = sqlite3_open("database.db", &db);
	if (rc){
		printf("Nie można utworzyć/otworzyć bazy danych: %s\n", sqlite3_errmsg(db));
		printf("Kończę działanie aplikacji z kodem wyjścia: %d\n", rc);
		return EXIT_FAILURE;
	}

	printf("Teraz aplikacja sprawdzi,\nczy w bazie danych jest  tabela \"majsterklepka\"...");

	/*jedna z możliwości sprawdzenia czy w bazie danych istnieje tabela*/
	sql_query= "CREATE TABLE IF NOT EXISTS majsterklepka (ROWID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name CHAR(50), address CHAR(100), zip CHAR(10), phone CHAR(14));";

	rc = sqlite3_exec(db, sql_query, NULL, 0, NULL);

	/*jeśli nie będzie żadnego błędu..
	 * to oznacza, że tabela istnieje!
	 * gdy wystąpi błąd...
	 * to oznacza, że tabeli nie ma w bazie danych
	 */
	if (rc == SQLITE_OK ){
		fprintf(stdout, "OK!\n");
	}

	/*polecenie języka sql umożliwiające
	 *sprawdzenie tabeli
	 *funkcja readata wyświetli ją w konsoli
	 */
	sql_query= "PRAGMA table_info(majsterklepka);";
	rc = sqlite3_exec(db, sql_query, readdata, 0, &errMsg);
	printf("Sprawdzanie poprawności tabeli \"majsterklepka\"...");
	if (rc == SQLITE_OK )
	{
		printf("OK!\n");
	}else{
		printf("Błąd!\nError code: %s\n", errMsg);
	}


	sqlite3_close(db);

	return EXIT_SUCCESS;

}

int readdata(void *NotUsed, int argc, char **argv, char **colName)
{
	int i;
	NotUsed = 0;
	for (i = 0; i < argc; i++)
	{
		printf("%s\t", argv[i] ? argv[i] : "NULL");
	}
	printf("\n");

	return EXIT_SUCCESS;
}
