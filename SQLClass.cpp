#include "SQLClass.h"

SQLClass::SQLClass()
{
	char* errMsg = 0;
	rc = sqlite3_open("DataBase.db", &db);

	if (rc)
	{
		throw("Error: Can't open database: %s\n", sqlite3_errmsg(db));
	}

	char sqlScript[] = "CREATE TABLE IF NOT EXISTS Accounts ( Username varchar PRIMARY KEY NOT NULL, Password varchar NOT NULL );" \
		"CREATE TABLE IF NOT EXISTS Paths ( Username varchar NOT NULL, Path varchar PRIMARY KEY NOT NULL );";

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
	std::string sql = "INSERT INTO Accounts (Username, Password) VALUES ('" + user + "','" + password + "');";
	const char* sqlScript = sql.c_str();

	rc = sqlite3_exec(db, sqlScript, isInTable, 0, &errMsg);

	if (rc != SQLITE_OK) {
		throw(sqlite3_errmsg(db));
	}
}

void SQLClass::addPath(std::string user, std::string path)
{
	char* errMsg;
	std::string sql = "INSERT INTO Paths (Username, Path) VALUES ('" + user + "','" + path + "');";
	const char* sqlScript = sql.c_str();

	rc = sqlite3_exec(db, sqlScript, isInTable, 0, &errMsg);

	if (rc != SQLITE_OK) {
		throw(sqlite3_errmsg(db));
	}
}

bool SQLClass::IsPathExists(std::string path, std::string user)
{
	char* errMsg;
	std::string sql = "SELECT COUNT(*) FROM Paths WHERE Username='" + user + "' AND Path='" + path + "';";
	const char* sqlScript = sql.c_str();

	rc = sqlite3_exec(db, sqlScript, isInTable, 0, &errMsg);

	if (rc != SQLITE_OK) {
		return false;
	}
	return true;
}

bool SQLClass::IsUserExists(std::string user, std::string password)
{
	char* errMsg;
	std::string sql = "SELECT COUNT(*) FROM Accounts WHERE Username='" + user + "' AND Password='" + password + "';";
	const char* sqlScript = sql.c_str();

	rc = sqlite3_exec(db, sqlScript, isInTable, 0, &errMsg);

	if (rc != SQLITE_OK) {
		return false;
	}
	return true;
}

std::vector<std::string> SQLClass::updatePassword(std::string user, std::string newPassword)
{
	char data = 'a';
	char* errMsg = &data;
	std::string sql = "SELECT Path FROM Paths WHERE Username='" + user + "';";
	const char* sqlScript = sql.c_str();
	std::vector<std::string> paths;

	rc = sqlite3_exec(db, sqlScript, GetPaths, (void*)&paths, &errMsg);

	if (rc != SQLITE_OK) {
		throw(sqlite3_errmsg(db));
	}

	sql = "UPDATE Accounts SET Password='" + newPassword + "' WHERE Username='" + user + "';";
	sqlScript = sql.c_str();

	rc = sqlite3_exec(db, sqlScript, callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		throw(sqlite3_errmsg(db));
	}

	return paths;
}

void SQLClass::removePath(std::string user, std::string path)
{
	char* errMsg;
	std::string sql = "DELETE FROM Paths WHERE Path='" + path + "';";
	const char* sqlScript = sql.c_str();

	rc = sqlite3_exec(db, sqlScript, callback, 0, &errMsg);

	if (rc != SQLITE_OK) {
		throw(sqlite3_errmsg(db));
	}
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

int SQLClass::isInTable(void* NotUsed, int argc, char** argv, char** azColName)
{
	if (argv[0][0] == '0')
		return 1;
	return 0;
}

int SQLClass::GetPaths(void* pathsPtr, int argc, char** argv, char** azColName)
{
	std::vector<std::string>* paths = &*reinterpret_cast<std::vector<std::string>*>(pathsPtr);
	for (int i = 0; i < argc; i++) {
		paths->push_back(argv[i]);
	}
	return 0;
}
