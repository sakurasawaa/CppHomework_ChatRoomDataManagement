#include "User.h"
#include "Sql.h"
#include "Print.h"

void listUsers() {
    sql::Statement *stmt = nullptr;
    sql::ResultSet *res = nullptr;
    stmt = con->createStatement();
    res = stmt->executeQuery("DESCRIBE users");
    Print print;
    std::vector<std::string> str;
    while (res->next()) {
        str.push_back(res->getString("Field"));
    }
    print.setHeaders(str[0], str[1], str[2], str[3], str[4]);
    res = stmt->executeQuery("SELECT * FROM users");
    while (res->next()) {
        print.addRow(res->getInt("id"), res->getString("name"), res->getString("password"), res->getInt("online"), res->getString("create_at"));
    }
    print.print();
}

sql::mysql::MySQL_Driver *driver;
sql::Connection *con;
int main() {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://0.0.0.0:3306", "root", "12345678");
        con->setSchema("DATA");
    }
    catch (const sql::SQLException &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    listUsers();
}