#include "database_manager.hpp"
using namespace std;

DatabaseManager::DatabaseManager()
{
    try
    {
        driver = sql::mysql::get_mysql_driver_instance();

        // Connect without specifying a schema
        con.reset(driver->connect("tcp://127.0.0.1:3306", "project", "password09"));

        // Create database if it doesn't exist
        unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("CREATE DATABASE IF NOT EXISTS word_game_db");

        // Select the database
        con->setSchema("word_game_db");

        stmt->execute("CREATE TABLE IF NOT EXISTS users ("
                      "id INT AUTO_INCREMENT PRIMARY KEY,"
                      "username VARCHAR(50) UNIQUE NOT NULL,"
                      "password VARCHAR(255) NOT NULL,"
                      "hindi_high_score INT DEFAULT 0,"
                      "bhojpuri_high_score INT DEFAULT 0,"
                      "english_high_score INT DEFAULT 0)");
    }
    catch (sql::SQLException &e)
    {
        cerr << "SQL Exception: " << e.what() << endl;
    }
}

DatabaseManager::~DatabaseManager() = default;

bool DatabaseManager::registerUser(const string &username, const string &password)
{
    try
    {
        unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(
            "INSERT INTO users (username, password) VALUES (?, ?)"));
        pstmt->setString(1, username);
        pstmt->setString(2, password);
        pstmt->execute();
        return true;
    }
    catch (sql::SQLException &e)
    {
        cerr << "SQL Exception: " << e.what() << endl;
        return false;
    }
}

bool DatabaseManager::loginUser(const string &username, const string &password)
{
    try
    {
        unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(
            "SELECT * FROM users WHERE username = ? AND password = ?"));
        pstmt->setString(1, username);
        pstmt->setString(2, password);
        unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        return res->next();
    }
    catch (sql::SQLException &e)
    {
        cerr << "SQL Exception: " << e.what() << endl;
        return false;
    }
}

int DatabaseManager::getHighScore(const string &username, int languageGame)
{
    int highScore = 0;
    try
    {
        // Choose the appropriate column based on languageGame value
        string column = "";
        if (languageGame == 0)
        {
            column = "hindi_high_score";
        }
        else if (languageGame == 1)
        {
            column = "english_high_score";
        }
        else
        {
            column = "bhojpuri_high_score";
        }

        unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(
            "SELECT " + column + " FROM users WHERE username = ?"));
        pstmt->setString(1, username);
        unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next())
        {
            highScore = res->getInt(column);
        }
    }
    catch (sql::SQLException &e)
    {
        cerr << "SQL Exception: " << e.what() << endl;
    }
    return highScore;
}

void DatabaseManager::updateHighScore(const string &username, int newHighScore, int languageGame)
{
    try
    {
        // Choose the appropriate column based on languageGame value
        string column = "";
        if (languageGame == 0)
        {
            column = "hindi_high_score";
        }
        else if (languageGame == 1)
        {
            column = "english_high_score";
        }
        else
        {
            column = "bhojpuri_high_score";
        }

        unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(
            "UPDATE users SET " + column + " = ? WHERE username = ?"));
        pstmt->setInt(1, newHighScore);
        pstmt->setString(2, username);
        pstmt->executeUpdate();
    }
    catch (sql::SQLException &e)
    {
        cerr << "SQL Exception: " << e.what() << endl;
    }
}
