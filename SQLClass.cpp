#include "SQLClass.h"

SQLClass::SQLClass()
{
	char* errMsg = 0;
	rc = sqlite3_open("Accounts.db", &db);

	if (rc)
	{
		//TODO Error message box
		throw("Error: Can't open database: %s\n", sqlite3_errmsg(db));
	}

	char sqlScript[] = "CREATE TABLE IF NOT EXISTS Accounts( Username varchar PRIMARY KEY NOT NULL, Password varchar NOT NULL);" \
					   "CREATE TABLE IF NOT EXISTS Paths(Username varchar NOT NULL, Path varchar PRIMARY KEY NOT NULL);";

	rc = sqlite3_exec(db, sqlScript, callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		throw("Error: %s\n", sqlite3_errmsg(db));
	}
}

SQLClass::~SQLClass()
{
	sqlite3_close(db);
}

void SQLClass::addAccount(std::string user, std::string password)
{
	char* errMsg;
	std::string sql = "SELECT STRING_ESCAPE('" + user + "', 'json') AS User; SELECT STRING_ESCAPE('" + password + "', 'json') AS Password; INSERT INTO Accounts(Username, Password) VALUES ('User','Password');"; //' -- "
	char* sqlScript = new char[sql.length()];
	strcpy(sqlScript, sql.c_str());

	rc = sqlite3_exec(db, sqlScript, callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		throw("Error: %s\n", sqlite3_errmsg(db));
	}

	delete [] sqlScript;
}

void SQLClass::addPath(std::string user, std::string path)
{
}

bool SQLClass::IsPathExists(std::string path)
{
	return false;
}

bool SQLClass::IsUserExists(std::string user)
{
	char* errMsg;
	std::string sql = "SELECT" + user + "from Accounts";
	char* sqlScript = new char[sql.length()];
	strcpy(sqlScript, sql.c_str());
	const char* data = "";

	return sqlite3_exec(db, sqlScript, isUserExists, (void*)data, &errMsg);
}

void SQLClass::updateUsername(std::string user, std::string password)
{
}

void SQLClass::updatePassword(std::string user, std::string password)
{
}

int SQLClass::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int SQLClass::isUserExists(void* data, int argc, char** argv, char** azColName)
{
	return argv[0] ? 1 : 0;
}
