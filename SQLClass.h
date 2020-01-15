#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "sqlite3.h"
#include <iostream>

class SQLClass
{
public:
	SQLClass();
	~SQLClass();

	void addAccount(std::string user, std::string password);
	void addPath(std::string user, std::string path);
	bool IsPathExists(std::string path);
	bool IsUserExists(std::string user);
	void updateUsername(std::string user, std::string password);
	void updatePassword(std::string user, std::string password);

private:
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);
	static int isUserExists(void* data, int argc, char** argv, char** azColName);
	sqlite3* db;				//Data Base
	int rc;						//Return Code
};

//https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm