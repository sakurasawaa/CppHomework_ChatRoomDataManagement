#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

sql::mysql::MySQL_Driver *driver;
sql::Connection *con;

int conToSql() {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://0.0.0.0:3306", "root", "12345678");
        con->setSchema("DATA");
    }
    catch (const sql::SQLException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}