#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

#define RESPONSE_OK 	0


int init_database();
int insert_data();
int read_data();

sqlite3 *db;

int main(void) {
    
    int rc = sqlite3_open("test7.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    int response = init_database();
   
    if (response == RESPONSE_OK)
    {
		printf("Baza danych poprawnie zainicjowana!\n");
	} else {
		printf("Nie udało się zainicjować bazy danych!\n");
		return -1;
	}
	response = insert_data(db);
    if (response == RESPONSE_OK)
    {
		printf("Dodano rekordy!\n");
	} else {
		printf("Nie dodano rekordów.\n");
		return -1;
	}
    
    response = read_data();
    if (response != RESPONSE_OK)
    {
		printf("Nie udało się odczytać rekordów z bazy danych.\n");
	}
    sqlite3_close(db);
    return 0;
}

int init_database()
{
	sqlite3_stmt *res;
    
     char *sql = "CREATE TABLE IF NOT EXISTS Friends(Id INTEGER PRIMARY KEY, Name TEXT);";
          
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK){
		printf("Nie udało się przygotować zapytania...\n");
		return -1;
	}
    
    if (sqlite3_step(res) == SQLITE_DONE ) {
        fprintf(stdout, "Tabela Friends utworzona z sukcesem\n");
    }
    sqlite3_reset(res);
    sqlite3_finalize(res);
    return 0;
}


int insert_data()
{
	sqlite3_stmt *res = NULL;
	char *sql = "INSERT INTO Friends (Name) VALUES(?1);";
    
    
	int rc = sqlite3_prepare_v2(db, sql, strlen(sql), &res, NULL);
    if (rc != SQLITE_OK){
		printf("Nie udało się przygotować zapytania...\n");
		return -1;
	}
    char *names[] = {"Paweł", "Marek", "Janusz"};
    int i = 0;
    for ( i = 0; i < 3; i++){
			rc = sqlite3_bind_text(res, 1, names[i], strlen(names[i]), NULL);
			if (rc != SQLITE_OK)
				return 1;
			if (sqlite3_step(res) != SQLITE_DONE) {
				sqlite3_finalize(res);
				sqlite3_close(db);
		        return -1;
			}
		sqlite3_reset(res);
		sqlite3_clear_bindings(res);
	}
	sqlite3_finalize(res);
		
	return 0;
}

int read_data()
{
	sqlite3_stmt *res = NULL;
	
		
	char *sql = "SELECT * FROM Friends";
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK){
		printf("Nie udało się skompilować zapytania...\n");
		return -1;
	}
    while(sqlite3_step(res) == SQLITE_ROW){
		printf("ID: %d\t%s\n", sqlite3_column_int(res, 0), sqlite3_column_text(res,1));
	}
	sqlite3_finalize(res);
//-----------------------------------------------------------
    int last_id = sqlite3_last_insert_rowid(db);
    printf("Ostatni klucz z wprowadzonych rekordów %d\n", last_id);
//-----------------------------------------------------------   
	
	return 0;
}
