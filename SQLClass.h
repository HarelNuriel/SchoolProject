#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "sqlite3.h"
#include <iostream>
#include <fstream>
#include "AES128.h"

#define KEY_SIZE 16

class SQLClass
{
public:
	SQLClass();
	~SQLClass();

	void addAccount(std::string user, std::string password);
	void addPath(std::string user, std::string path);
	bool IsPathExists(std::string path, std::string user);
	bool IsUserExists(std::string user, std::string password);
	char** updateUsername(std::string user, std::string password, std::string prevUser);
	char** updatePassword(std::string user, std::string password, std::string newPassword);
	void removePath(std::string user, std::string path);
	int pathCount(std::string user);

//private:
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);
	static int isInTable(void* NotUsed, int argc, char** argv, char** azColName);
	static int GetPaths(void* NotUsed, int argc, char** argv, char** azColName);
	static int GetCount(void* NotUsed, int argc, char** argv, char** azColName);
	sqlite3* db;				//Data Base
	int rc;						//Return Code
};

//https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm