#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

#define DB_CON "tcp://0.0.0.0:3306"
#define DB_NAME "ChatRoom"
#define DB_USER "chatroom"
#define DB_PASS "chatroom"

sql::mysql::MySQL_Driver *driver;
sql::Connection *con;

int conToSql() {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(DB_CON, DB_USER, DB_PASS);
        con->setSchema(DB_NAME);
    }
    catch (const sql::SQLException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}