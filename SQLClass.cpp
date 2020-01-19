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
	sqlite3_stmt* sql;
	sqlite3_prepare_v2(
		db,
		"INSERT INTO Accounts(Username, Password) VALUES (?, ?)",
		-1,
		&sql,
		NULL
	);

	sqlite3_bind_text(sql, 1, user.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(sql, 2, password.c_str(), -1, SQLITE_TRANSIENT);

	sqlite3_step(sql);
}

void SQLClass::addPath(std::string user, std::string path)
{
	char* errMsg;
	sqlite3_stmt* sql;
	sqlite3_prepare_v2(
		db,
		"INSERT INTO Paths(Username, Path) VALUES (?, ?)",
		-1,
		&sql,
		NULL
	);

	sqlite3_bind_text(sql, 1, user.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(sql, 2, path.c_str() , -1, SQLITE_TRANSIENT);

	sqlite3_step(sql);
}

bool SQLClass::IsPathExists(std::string path, std::string user)
{
	sqlite3_stmt* query = NULL;
	int ret = 0;
	do // avoid nested if's
	{
		// initialize engine
		if (SQLITE_OK != (ret = sqlite3_initialize()))
		{
			printf("Failed to initialize library: %d\n", ret);
			break;
		}
		// open connection to a DB
		if (SQLITE_OK != (ret = sqlite3_open_v2("test.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)))
		{
			printf("Failed to open conn: %d\n", ret);
			break;
		}
		// prepare the statement
		if (SQLITE_OK != (ret = sqlite3_prepare_v2(db, "SELECT ? FROM Paths WHERE Username = ?", -1, &query, NULL)))
		{
			printf("Failed to prepare insert: %d, %s\n", ret, sqlite3_errmsg(db));
			break;
		}
		sqlite3_bind_text(query, 1, user.c_str(), -1, SQLITE_TRANSIENT);	//Binding the values to the statement
		sqlite3_bind_text(query, 2, path.c_str(), -1, SQLITE_TRANSIENT);
		// step to 1st row of data
		if (SQLITE_ROW != (ret = sqlite3_step(query))) // see documentation, this can return more values as success
		{
			printf("Failed to step: %d, %s\n", ret, sqlite3_errmsg(db));
			break;
		}
		// ... and print the value of column 0 (expect 2012 here)
		const unsigned char* a = sqlite3_column_text(query, 0);
		printf("Value from sqlite: %s", sqlite3_column_text(query, 0));

	} while (false);
	// cleanup
	if (NULL != query) sqlite3_finalize(query);
	if (NULL != db) sqlite3_close(db);
	sqlite3_shutdown();
	return ret;
}

bool SQLClass::IsUserExists(std::string user, std::string password)
{
	sqlite3_stmt* query = NULL;
	int ret = 0;
	// initialize engine
	if (SQLITE_OK != (ret = sqlite3_initialize()))
	{
		printf("Failed to initialize library: %d\n", ret);
	}
	// open connection to a DB
	if (SQLITE_OK != (ret = sqlite3_open_v2("test.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)))
	{
		printf("Failed to open conn: %d\n", ret);
	}
	// prepare the statement
	if (SQLITE_OK != (ret = sqlite3_prepare_v2(db, "SELECT ? FROM Accounts WHERE Username = ?", -1, &query, NULL)))
	{
		printf("Failed to prepare insert: %d, %s\n", ret, sqlite3_errmsg(db));
	}
	//Binding the values to the statement
	sqlite3_bind_text(query, 1, password.c_str(), -1, SQLITE_TRANSIENT);
	sqlite3_bind_text(query, 2, user.c_str(), -1, SQLITE_TRANSIENT);
	// step to 1st row of data
	if (SQLITE_ROW != (ret = sqlite3_step(query))) // see documentation, this can return more values as success
	{
		printf("Failed to step: %d, %s\n", ret, sqlite3_errmsg(db));
	}
	// ... and print the value of column 0 (expect 2012 here)
	const unsigned char* a = sqlite3_column_text(query, 0);

	// cleanup
	if (NULL != query) sqlite3_finalize(query);
	if (NULL != db) sqlite3_close(db);
	sqlite3_shutdown();
	return a == NULL;
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
	char* a = (char*)data;
	for (int i = 0; i < strlen(a); i++) {
		if (argv[0][i] != a[i])
			return 1;
	}
	return 0;
}