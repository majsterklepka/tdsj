#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

#define RESPONSE_OK 	0


int init_database();
int insert_data();
int read_data();
int update_data(char *name, int id);
int delete_data(int id);
void last_key(sqlite3 *db);
int read_data_one(int id);

sqlite3 *db;

int main(void) {
    
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Błąd! Nie udało się otworzyć bazy danych: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    int response = init_database();
   
    if (response != RESPONSE_OK)
    {
		printf("Nie udało się zainicjować bazy danych!\n");
		return -1;
	}
	response = insert_data(db);
    if (response != RESPONSE_OK)
    {
		printf("Nie dodano rekordów.\n");
		return -1;
	}
	    
    response = read_data();
    if (response != RESPONSE_OK)
    {
		printf("Nie udało się odczytać rekordów z bazy danych.\n");
	}
	
	last_key(db);
	
	char *name = "Piotr";
	int id = 2;
	
	response = update_data(name, id);
	if (response != RESPONSE_OK)
    {
		printf("Nie udało się uaktualnić danych w bazie danych.\n");
		return -1;
	}
		
	response = read_data();
    if (response != RESPONSE_OK)
    {
		printf("Nie udało się odczytać rekordów z bazy danych.\n");
	}
	
	response = delete_data(2);
	if (response != RESPONSE_OK)
    {
		printf("Nie udało się usunąć rekordu z bazy danych.\n");
	}
	response = read_data();
	if (response != RESPONSE_OK)
    {
		printf("Nie udało się odczytać rekordów z bazy danych.\n");
	}
	
	response = read_data_one(4);
	if (response != RESPONSE_OK)
    {
		printf("Nie udało się odczytać rekordów z bazy danych.\n");
	}
	
	response = read_data_one(3);
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
		return -1;
	}
    char *names[] = {"Paweł", "Marek", "Janusz", "John", "Remigiusz", NULL};
    int k = 0;
    while (names[k] != NULL)
    {
		k++;
	}
    int i = 0;
    for ( i = 0; i < k; i++){
			rc = sqlite3_bind_text(res, 1, names[i], strlen(names[i]), NULL);
			if (rc != SQLITE_OK)
				return -1;
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
		return -1;
	}
    while(sqlite3_step(res) == SQLITE_ROW){
		printf("ID: %d\t%s\n", sqlite3_column_int(res, 0), sqlite3_column_text(res,1));
	}
	sqlite3_reset(res);
	sqlite3_clear_bindings(res);
	sqlite3_finalize(res);
	
	return 0;
}

int update_data(char * name, int id)
{
	sqlite3_stmt *res = NULL;
	char *sql = "UPDATE Friends SET Name = ?1 WHERE Id = ?2";
	int rc = sqlite3_prepare_v2(db, sql, strlen(sql), &res, 0);
	
	if (rc != SQLITE_OK){
		return -1;
	} else {
		rc = sqlite3_bind_int(res, 2, id);	
		rc = sqlite3_bind_text(res, 1, name, strlen(name), NULL);
	}
	
	if (rc != SQLITE_OK){
		return -1;
	}
	
	if (sqlite3_step(res) != SQLITE_DONE) {
		sqlite3_finalize(res);
		sqlite3_close(db);
		return -1;
	}
	
	sqlite3_reset(res);
	sqlite3_clear_bindings(res);
	sqlite3_finalize(res);

	return 0;
}

int delete_data(int id)
{
	sqlite3_stmt *res = NULL;
	char *sql = "DELETE FROM Friends WHERE Id = ?1";
	int rc = sqlite3_prepare_v2(db, sql, strlen(sql), &res, 0);
	
	if (rc != SQLITE_OK){
		return -1;
	} else {
		rc = sqlite3_bind_int(res, 1, id);
	}
	
	if (rc != SQLITE_OK){
		return -1;
	}
	if (sqlite3_step(res) != SQLITE_DONE) {
		sqlite3_finalize(res);
		sqlite3_close(db);
		return -1;
	}
	sqlite3_reset(res);
	sqlite3_clear_bindings(res);
	sqlite3_finalize(res);
	return 0;
}

int read_data_one(int id)
{
	sqlite3_stmt *res = NULL;
	
	char *sql = "SELECT COUNT(*) FROM Friends WHERE Id = ?1";
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK){
		return -1;
	}
	sqlite3_bind_int(res, 1, id);
	int row = 0;
	while(sqlite3_step(res) != SQLITE_DONE){
		row = sqlite3_column_int(res, 0);
	}
	
	sqlite3_reset(res);
	sqlite3_clear_bindings(res);
	sqlite3_finalize(res);
	
	sql = "SELECT * FROM Friends WHERE Id = ?1";
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    
    if (rc != SQLITE_OK){
		return -1;
	}
	sqlite3_bind_int(res, 1, id);
	
	if (row > 0)
	{
		printf("<=========================>\n");
        while(sqlite3_step(res) == SQLITE_ROW){
		printf("ID: %d\t%s\n", sqlite3_column_int(res, 0), sqlite3_column_text(res,1));
		}
		printf("<=========================>\n");
	}
	
	sqlite3_reset(res);
	sqlite3_clear_bindings(res);
	sqlite3_finalize(res);
	
	return 0;
}

void last_key(sqlite3 *db)
{
	int last_id = sqlite3_last_insert_rowid(db);
    printf("Ostatni wprowadzony wiersz(rekord) danych: %d\n", last_id);
    int response = read_data_one(last_id);
    if (response != RESPONSE_OK)
    {
		printf("Brak danych...\n");
	}
}

