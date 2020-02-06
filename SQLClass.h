#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "sqlite3.h"
#include <iostream>
#include "wx/wx.h"

class SQLClass
{
public:
	SQLClass();
	~SQLClass();

	void addAccount(std::string user, std::string password);
	void addPath(std::string user, std::string path);
	bool IsPathExists(std::string path, std::string user);
	bool IsUserExists(std::string user, std::string password);
	void updateUsername(std::string user, std::string password);
	void updatePassword(std::string user, std::string password);
	void removePath(std::string user, std::string path);

private:
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);
	static int isInTable(void* NotUsed, int argc, char** argv, char** azColName);
	sqlite3* db;				//Data Base
	int rc;						//Return Code
};

//https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm