#include "SQLClass.h"

SQLClass::SQLClass()
{
	char* errMsg = 0;
	rc = sqlite3_open("DataBase.db", &db);

	// Checking is the database was successfully opened
	if (rc)
	{
		throw("Error: Can't open database: %s\n", sqlite3_errmsg(db));
	}

	char sqlScript[] = "CREATE TABLE IF NOT EXISTS Accounts ( Username varchar PRIMARY KEY NOT NULL, Password varchar NOT NULL );" \
		"CREATE TABLE IF NOT EXISTS Paths ( Username varchar NOT NULL, Path varchar PRIMARY KEY NOT NULL );";

	// Executing the sqlscript
	rc = sqlite3_exec(db, sqlScript, callback, 0, &errMsg);

	// Checking if sqlite3_exec returned ok 
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

	// Executing the sqlscript
	rc = sqlite3_exec(db, sqlScript, isInTable, 0, &errMsg);

	// Checking if sqlite3_exec returned ok 
	if (rc != SQLITE_OK) {
		throw(sqlite3_errmsg(db));
	}
}

void SQLClass::addPath(std::string user, std::string path)
{
	char* errMsg;
	std::string sql = "INSERT INTO Paths (Username, Path) VALUES ('" + user + "','" + path + "');";
	const char* sqlScript = sql.c_str();

	// Executing the sqlscript
	rc = sqlite3_exec(db, sqlScript, isInTable, 0, &errMsg);

	// Checking if sqlite3_exec returned ok 
	if (rc != SQLITE_OK) {
		throw(sqlite3_errmsg(db));
	}
}

bool SQLClass::IsPathExists(std::string path, std::string user)
{
	char* errMsg;
	std::string sql = "SELECT COUNT(*) FROM Paths WHERE Username='" + user + "' AND Path='" + path + "';";
	const char* sqlScript = sql.c_str();

	// Executing the sqlscript
	rc = sqlite3_exec(db, sqlScript, isInTable, 0, &errMsg);

	// If the funcction isIntable returned false tha return false esle, return true
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

	// Executing the sqlscript
	rc = sqlite3_exec(db, sqlScript, isInTable, 0, &errMsg);

	// If the funcction isIntable returned false tha return false esle, return true
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

	// Executing the sql script and giving the callback function (GetPaths) the pointer to the paths 
	// vector
	rc = sqlite3_exec(db, sqlScript, GetPaths, (void*)&paths, &errMsg);

	// Checking if sqlite3_exec returned ok 
	if (rc != SQLITE_OK) {
		throw(sqlite3_errmsg(db));
	}

	// Making a new sql script
	sql = "UPDATE Accounts SET Password='" + newPassword + "' WHERE Username='" + user + "';";
	sqlScript = sql.c_str();

	// Executing the new script
	rc = sqlite3_exec(db, sqlScript, callback, 0, &errMsg);

	// Checking if sqlite3_exec returned ok 
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

	// Executing the sql script
	rc = sqlite3_exec(db, sqlScript, callback, 0, &errMsg);

	// Checking if sqlite3_exec returned ok
	if (rc != SQLITE_OK) {
		throw(sqlite3_errmsg(db));
	}
}

int SQLClass::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	// Generic callback functionto print the values to the console
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int SQLClass::isInTable(void* NotUsed, int argc, char** argv, char** azColName)
{
	// If the count is 0 there are no user that matched the script in the table
	if (argv[0][0] == '0')
		return 1;
	return 0;
}

int SQLClass::GetPaths(void* pathsPtr, int argc, char** argv, char** azColName)
{
	// Interpreting the void* into std::vector* to use the vector and store values in it
	std::vector<std::string>* paths = &*reinterpret_cast<std::vector<std::string>*>(pathsPtr);
	for (int i = 0; i < argc; i++) {
		// Pushing the paths into the queue
		paths->push_back(argv[i]);
	}
	return 0;
}
