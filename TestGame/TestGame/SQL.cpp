/*
* Example to connect to MariaDB(MySQL)
*/
#include "SQL.h"


using namespace std;

sqlConnect::sqlConnect()
{
	// Initialize constants
	MY_HOSTNAME = "localhost";
	MY_DATABASE = "authentication";
	MY_USERNAME = "root";
	MY_PASSWORD = "eggeg";
	MY_SOCKET = NULL;

}

bool sqlConnect::insert(std::string user,std::string hash,std::string salty)
{
	//formulate executable sql command
	std::string command = "INSERT INTO Pass (username, pw,salt) VALUES ('" + user + "', '" + hash + "','" + salty+"')";
	try {
		// Format a MySQL object
		conn = mysql_init(NULL);

		// Establish a MySQL connection
		if (!mysql_real_connect(
			conn,
			MY_HOSTNAME, MY_USERNAME,
			MY_PASSWORD, MY_DATABASE,
			MY_PORT_NO, MY_SOCKET, MY_OPT)) {
			cerr << mysql_error(conn) << endl;
			return false;
		}

		// Execute a sql statement
		if (mysql_query(conn, command.c_str())) {
			cerr << mysql_error(conn) << endl;
			return false;
		}
		//commit
		mysql_commit(conn);

		// Close a MySQL connection
		mysql_close(conn);
	}
	catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;
		return false;
	}
	return true;
}
std::vector<std::string> sqlConnect::fetch(std::string user)
{
	//create command to execute
	std::string command = "Select pw,salt From pass where Binary username = '" + user + "'";
	std::cout << "command: " << command << std::endl;

	//fill error vector
	error.push_back("-1");
	error.push_back("-1");

	try {
		// Format a MySQL object
		conn = mysql_init(NULL);

		// Establish a MySQL connection
		if (!mysql_real_connect(
			conn,
			MY_HOSTNAME, MY_USERNAME,
			MY_PASSWORD, MY_DATABASE,
			MY_PORT_NO, MY_SOCKET, MY_OPT)) {
			cerr << mysql_error(conn) << endl;
			return error;
		}

		// Execute a sql statement
		if (mysql_real_query(conn, command.c_str(),unsigned long(command.size()))) {
			cerr << mysql_error(conn) << endl;
			std::cout << "error line " << __LINE__ << std::endl;
			return  error;
		}
		res = mysql_use_result(conn);
		if (mysql_num_fields(res) > 0)
		{
			row = mysql_fetch_row(res);
			if (row == NULL)
			{
				std::cout << "error line 90" << std::endl;
				return error;
			}
			pw = row[0];
			salt = row[1];
		}
		myVector.push_back(pw);
		myVector.push_back(salt);
		for (unsigned int i = 0; i < myVector.size(); i++) 
		{
			std::cout << myVector[i] << std::endl;
		}
		// Close a MySQL connection
		mysql_close(conn);
	}
	catch (char *e) {
		cerr << "[EXCEPTION] " << e << endl;
		std::cout << "error line 107" << std::endl;
		return error;
	}
	return myVector;
}