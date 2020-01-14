#include "SQLClass.h"

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

SQLClass::SQLClass()
{
	char* errMsg;
	rc = sqlite3_open("Accounts.db", &db);

	if (rc)
	{
		//TODO Error message box
		throw("Error: Can't open database: %s\n", sqlite3_errmsg(db));
	}

	char sqlScript[] = "CREATE TABLE IF NOT EXISTS Accounts( Username varchar PRIMARY KEY NOT NULL, Password varchar NOT NULL);";

	rc = sqlite3_exec(db, sqlScript, callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
	}
}

void SQLClass::addUser()
{

}

bool SQLClass::IsUserExists(std::string user)
{
	return false;
}
