#pragma once
#include <iostream>
#include <mysql.h>  // require libmysqlclient-dev
#include <string>
#include <mysqld_error.h>
#include <vector>

class sqlConnect
{
public:

	sqlConnect();           // Constructor
	bool insert(std::string user, std::string hash,std::string salt);  
	std::vector<std::string> fetch(std::string user);
	std::vector<std::string> myVector;
	std::vector<std::string> error;
private:
	const char* MY_HOSTNAME;
	const char* MY_DATABASE;
	const char* MY_USERNAME;
	const char* MY_PASSWORD;
	const char* MY_SOCKET;
	enum {
		MY_PORT_NO = 3306,
		MY_OPT = 0
	};
	MYSQL     *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	std::string pw;
	std::string salt;
};