#pragma once

#include "sqlite3.h"
#include <iostream>

class SQLClass
{
public:
	SQLClass();

	void addUser();
	bool IsUserExists(std::string user);

private:
	sqlite3* db;				//Data Base
	int rc;						//Return Code
};

