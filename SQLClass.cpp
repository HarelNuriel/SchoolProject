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
	std::string sql = "INSERT INTO Accounts(Username, Password) VALUES ('" + user + "','" + password + "');"; //' -- "
	const char* sqlScript = sql.c_str();;
	
	rc = sqlite3_exec(db, sqlScript, callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		throw("Error: %s\n", sqlite3_errmsg(db));
	}

}

void SQLClass::addPath(std::string user, std::string path)
{
	char* errMsg;
	std::string sql = "INSERT INTO Paths(Username, Password) VALUES ('" + user + "','" + path + "');";
	const char* sqlScript = sql.c_str();

	rc = sqlite3_exec(db, sqlScript, callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		throw("Error: %s\n", sqlite3_errmsg(db));
	}

}

bool SQLClass::IsPathExists(std::string path, std::string user)
{
	char* errMsg;
	std::string sql = "SELECT " + path + " from Paths";
	const char* sqlScript = sql.c_str();
	bool data = false;

	rc = sqlite3_exec(db, sqlScript, isExistsInTable, (void*)data, &errMsg);

	if (rc != SQLITE_OK) {
		return false;
	}

	return true;
}

bool SQLClass::IsUserExists(std::string user, std::string password)
{
	char* errMsg;
	std::string sql = "SELECT " + password + " from Accounts WHERE Username = " + user;
	const char* sqlScript = sql.c_str();
	const char* data = password.c_str();

	rc = sqlite3_exec(db, sqlScript, isExistsInTable, (void*)data, &errMsg);

	if (rc != SQLITE_OK) {
		return false;
	}

	return true;
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

int SQLClass::isExistsInTable(void* data, int argc, char** argv, char** azColName)
{
	if (argv[0] == (char*)data)
		return 0;
	else
		return 1;
}