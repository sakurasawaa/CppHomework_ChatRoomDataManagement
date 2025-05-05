#include <string>
#include "../include/Sql.h"
#include "../include/Print.h"
#include <sstream>

class User
{
    public:
        int signin();
        int signout();
        int createGroup(std::string);
        int addFriend(int);
        int delFriend(int);
        int addGroup(int);
        int delGroup(int);
        int sendToUser();
        int sendToGroup();
        User(int);
        User(std::string);
        ~User();
    protected:
        bool iExist();
        bool userExist(int);
        bool groupExist(int);
        bool useriExist(int);
        bool groupiExist(int);
    private:
        sql::Statement *stmt = nullptr;
        sql::PreparedStatement *prep_stmt = nullptr;
        sql::ResultSet *res = nullptr;
        int id = 0;
        std::string name;
        int passwd;
};
User::User(int id) {
    this->id = id;
}
User::User(std::string name) {
    this->name = name;
    std::ostringstream oss;
    oss << "SELECT id FROM users WHERE name = '" << name << "' LIMIT 1";
    stmt = con->createStatement();
    res = stmt->executeQuery(oss.str());
    if (res->next()) {
        id = res->getInt(1);
        std::cout << "ID: " << id << std::endl;
    }
}
User::~User() {
    if (stmt) delete stmt;
    if (prep_stmt) delete prep_stmt;
    if (res) delete res;
}


int User::signin() {
    std::ostringstream oss;
    // oss << "SELECT 1 FROM users WHERE name = '" << name << "' LIMIT 1";
    // std::cout << oss.str() << std::endl;
    // stmt = con->createStatement();
    // res = stmt->executeQuery(oss.str());
    if (id) std::cout << "Users exist!" << std::endl;
    else {
        std::cout << "Password: ";
        std::cin >> passwd;
        //这里注意清空输入缓冲区
        prep_stmt = con->prepareStatement("INSERT INTO users (name, password) VALUES (?, ?)");
        prep_stmt->setString(1, name);
        prep_stmt->setInt(2, passwd);
        prep_stmt->executeUpdate();
        prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID()");
        res = prep_stmt->executeQuery();
        if (res->next()) {
            id = res->getInt(1);
            std::cout << "ID: " << id << std::endl;
            oss.str("");
            oss << "CREATE TABLE " << id << "_userData (object INT, is_group tinyint, time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)";
            stmt->executeUpdate(oss.str());
        }
    }
    return 0;
}
int User::signout() {
    if (!userExist(id)) return 1;
    std::ostringstream oss;
    oss << "DROP TABLE " << id << "_userData";
    stmt = con->createStatement();
    stmt->executeUpdate(oss.str());
    oss.str("");
    oss << "DELETE FROM users WHERE id = " << id;
    stmt->executeUpdate(oss.str());
    return 0;
}

bool User::iExist() {
    std::ostringstream oss;
    oss << "SHOW TABLES LIKE '" << id << "_userData'";
    std::cout << oss.str() << std::endl;
    prep_stmt = con->prepareStatement(oss.str());
    res = prep_stmt->executeQuery();
    if (res->next()) return true;
    else{
        std::cout << "User not exist" << std::endl;
        return false;
    }
}
bool User::userExist(int uid) {
    std::ostringstream oss;
    oss << "SELECT 1 FROM `users` WHERE id = '" << uid << "' LIMIT 1";
    std::cout << oss.str() << std::endl;
    stmt = con->createStatement();
    res = stmt->executeQuery(oss.str());
    if (res->next()) return true;
    else return false;
}
bool User::groupExist(int gid) {
    std::ostringstream oss;
    oss << "SELECT 1 FROM `groups` WHERE id = '" << gid << "' LIMIT 1";
    std::cout << oss.str() << std::endl;
    stmt = con->createStatement();
    res = stmt->executeQuery(oss.str());
    if (res->next()) return true;
    else return false;
}
bool User::useriExist(int uid) {
    std::ostringstream oss;
    oss << "SELECT 1 FROM `" << id <<"_userData` WHERE id = '" << uid << "' AND is_group = 0 LIMIT 1";
    std::cout << oss.str() << std::endl;
    stmt = con->createStatement();
    res = stmt->executeQuery(oss.str());
    if (res->next()) return true;
    else return false;
}
bool User::groupiExist(int gid) {
    std::ostringstream oss;
    oss << "SELECT 1 FROM `" << id <<"_userData` WHERE id = '" << gid << "' AND is_group = 1 LIMIT 1";
    std::cout << oss.str() << std::endl;
    stmt = con->createStatement();
    res = stmt->executeQuery(oss.str());
    if (res->next()) return true;
    else return false;
}

int User::createGroup(std::string gname) {
    if (!iExist()) return 1;
    std::ostringstream oss;
    int gid;
    oss << "SELECT 1 FROM `groups` WHERE name = '" << gname << "' LIMIT 1";
    std::cout << oss.str() << std::endl;
    stmt = con->createStatement();
    res = stmt->executeQuery(oss.str());
    if (res->next()) std::cout << "Groups exist!" << std::endl;
    else {
        prep_stmt = con->prepareStatement("INSERT INTO `groups` (name, create_by) VALUES (?, ?)");
        prep_stmt->setString(1, gname);
        prep_stmt->setInt(2, id);
        prep_stmt->executeUpdate();
        prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID()");
        res = prep_stmt->executeQuery();
        if (res->next()) {
            gid = res->getInt(1);
            std::cout << "GID: " << gid << std::endl;
            addGroup(gid);
        }
        
    }
    
    return 0;
}

int User::addGroup(int gid) {
    if (!iExist() || !groupExist(gid)) return 1;
    std::ostringstream oss;
    oss << "INSERT INTO " << id << "_userData (object, is_group) VALUES (?, ?)";
    std::cout << oss.str() << std::endl;
    prep_stmt = con->prepareStatement(oss.str());
    prep_stmt->setInt(1, gid);
    prep_stmt->setBoolean(2, 1);
    prep_stmt->executeUpdate();

    return 0;
}
int User::addFriend(int uid) {
    if (!iExist() || !userExist(uid)) return 1;
    std::ostringstream oss;
    oss << "INSERT INTO " << id << "_userData (object, is_group) VALUES (?, ?)";
    std::cout << oss.str() << std::endl;
    prep_stmt = con->prepareStatement(oss.str());
    prep_stmt->setInt(1, uid);
    prep_stmt->setBoolean(2, 0);
    prep_stmt->executeUpdate();
    return 0;
}

int User::delFriend(int uid) {
    return 0;
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
    User user01("test01");
    user01.signin();
    User user02("test02");
    user02.signin();
    User user03("test03");
    user03.signin();
    User user04("test04");
    user04.signin();
    User user05("test05");
    user05.signin();
    // user.createGroup("test1");
    return 0;
}