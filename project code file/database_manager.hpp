#ifndef DATABASE_MANAGER_HPP
#define DATABASE_MANAGER_HPP

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <string>
#include <memory>
#include <iostream>
using namespace std;

class DatabaseManager
{
private:
    sql::mysql::MySQL_Driver *driver;
    unique_ptr<sql::Connection> con;

public:
    DatabaseManager();
    ~DatabaseManager();
    bool registerUser(const string &username, const string &password);
    bool loginUser(const string &username, const string &password);
    int getHighScore(const string &username, int languageGame);
    void updateHighScore(const string &username, int newHighScore, int languageGame);
};

#endif // DATABASE_MANAGER_HPP
