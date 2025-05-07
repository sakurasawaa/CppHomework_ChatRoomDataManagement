#include <string>
#include <sstream>
#include "Sql.h"
#include <limits>

class User
{
    public:
        int signin();
        int signin(std::string);
        int signout();
        int createGroup(std::string);
        int deleteGroup(int);
        int removeGroup(int);
        int addFriend(int);
        int deleteFriend(int);
        int addGroup(int);
        int changeName(std::string);
        int changePasswd(std::string);
        int getUserID();
        std::string getUserNmae();
        int sendToUser();
        int sendToGroup();
        User(int);
        User(std::string);
        ~User();
    protected:
        bool iExist();
        bool userExist(int);
        bool groupExist(int);
    private:
        sql::Statement *stmt = nullptr;
        sql::PreparedStatement *prep_stmt = nullptr;
        sql::ResultSet *res = nullptr;
        int id = 0;
        std::string name;
        int passwd = 0;
};
User::User(int id) {
    this->id = id;
    std::ostringstream oss;
    oss << "SELECT name FROM users WHERE id = '" << id << "' LIMIT 1";
    stmt = con->createStatement();
    res = stmt->executeQuery(oss.str());
    if (res->next()) {
        name = res->getString("name");
        // std::cout << "Name: " << name << std::endl;
    }
    else {
        this->id = 0;
    }
}
User::User(std::string name) {
    this->name = name;
    std::ostringstream oss;
    oss << "SELECT id FROM users WHERE name = '" << name << "' LIMIT 1";
    stmt = con->createStatement();
    res = stmt->executeQuery(oss.str());
    if (res->next()) {
        id = res->getInt("id");
        // std::cout << "ID: " << id << std::endl;
    }
}
User::~User() {
    if (stmt) delete stmt;
    if (prep_stmt) delete prep_stmt;
    if (res) delete res;
}


int User::signin() { //废弃
    std::ostringstream oss;
    if (id) std::cout << "Users exist!" << std::endl;
    else {
        std::cout << "Password: ";
        std::cin >> passwd;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        prep_stmt = con->prepareStatement("INSERT INTO users (name, password) VALUES (?, ?)");
        prep_stmt->setString(1, name);
        prep_stmt->setInt(2, passwd);
        prep_stmt->executeUpdate();
        prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID()");
        res = prep_stmt->executeQuery();
        if (res->next()) {
            id = res->getInt(1);
            std::clog << "[System]Signin::UserID: " << id << std::endl;
            oss.str("");
            oss << "CREATE TABLE " << id << "_userData (object INT, is_group tinyint, time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)";
            stmt->executeUpdate(oss.str());
        }
        return id;
    }
    return 0;
}
int User::signin(std::string passwd) {
    std::ostringstream oss;
    if (id) std::clog << "[System]Users exist! UID:" << id << std::endl;
    else {
        prep_stmt = con->prepareStatement("INSERT INTO users (name, password) VALUES (?, ?)");
        prep_stmt->setString(1, name);
        prep_stmt->setString(2, passwd);
        prep_stmt->executeUpdate();
        prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID()");
        res = prep_stmt->executeQuery();
        if (res->next()) {
            id = res->getInt(1);
            std::clog << "[System]Signin::UserID: " << id << std::endl;
            oss.str("");
            oss << "CREATE TABLE " << id << "_userData (object INT, is_group tinyint, time TIMESTAMP DEFAULT CURRENT_TIMESTAMP)";
            stmt->executeUpdate(oss.str());
        }
        return id;
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
    // std::cout << oss.str() << std::endl;
    prep_stmt = con->prepareStatement(oss.str());
    res = prep_stmt->executeQuery();
    if (res->next()) return true;
    else{
        std::clog << "[System]User profile not exist:" << std::endl;
        return false;
    }
}
bool User::userExist(int uid) {
    std::ostringstream oss;
    oss << "SELECT 1 FROM `users` WHERE id = '" << uid << "' LIMIT 1";
    // std::cout << oss.str() << std::endl;
    stmt = con->createStatement();
    res = stmt->executeQuery(oss.str());
    if (res->next()) return true;
    else return false;
}
bool User::groupExist(int gid) {
    std::ostringstream oss;
    oss << "SELECT 1 FROM `groups` WHERE id = '" << gid << "' LIMIT 1";
    // std::cout << oss.str() << std::endl;
    stmt = con->createStatement();
    res = stmt->executeQuery(oss.str());
    if (res->next()) return true;
    else return false;
}

int User::createGroup(std::string gname) {
    if (!id) return 1;
    std::ostringstream oss;
    int gid = 0;
    oss << "SELECT id FROM `groups` WHERE name = '" << gname << "' LIMIT 1";
    // std::cout << oss.str() << std::endl;
    stmt = con->createStatement();
    res = stmt->executeQuery(oss.str());
    if (res->next()) {
        gid = res->getInt("id");
        std::clog << "[System]Groups exist! GID:" << gid << std::endl;
        return 0;
    }
    else {
        prep_stmt = con->prepareStatement("INSERT INTO `groups` (name, create_by) VALUES (?, ?)");
        prep_stmt->setString(1, gname);
        prep_stmt->setInt(2, id);
        prep_stmt->executeUpdate();
        prep_stmt = con->prepareStatement("SELECT LAST_INSERT_ID()");
        res = prep_stmt->executeQuery();
        if (res->next()) {
            gid = res->getInt(1);
            addGroup(gid);
            std::clog << "[System]Group created GID:" << gid << std::endl;
        }
        return gid;
    }
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

int User::deleteFriend(int uid) {
    std::ostringstream oss;
    oss << "DELETE FROM " << id << "_userData WHERE object = ? AND is_group = 0";
    prep_stmt = con->prepareStatement(oss.str());
    prep_stmt->setInt(1, uid);
    if (prep_stmt->executeUpdate())
    {
        std::clog << "[" << id << "]Friend deleted: " << uid << std::endl;
        return 0;
    } 
    else {
        std::clog << "[" << id << "]Friend not exist!" << std::endl;
        return 1;
    }
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

int User::deleteGroup(int gid) {
    std::ostringstream oss;
    oss << "DELETE FROM " << id << "_userData WHERE object = ? AND is_group = 1";
    prep_stmt = con->prepareStatement(oss.str());
    prep_stmt->setInt(1, gid);
    if (prep_stmt->executeUpdate())
    {
        std::clog << "[User]Group deleted: " << gid << std::endl;
        return 0;
    } 
    else {
        std::clog << "[User]Group not exist!" << std::endl;
        return 1;
    }
}

int User::removeGroup(int gid) {
    std::ostringstream oss;
    prep_stmt = con->prepareStatement("SELECT 1 FROM `groups` WHERE id = ? AND create_by = ? LIMIT 1");
    prep_stmt->setInt(1, gid);
    prep_stmt->setInt(2, id);
    res = prep_stmt->executeQuery();
    if (res->next()) {
        prep_stmt = con->prepareStatement("DELETE FROM `groups` WHERE id = ?");
        prep_stmt->setInt(1, gid);
        prep_stmt->executeUpdate();
        std::clog << "[System]Group deleted: " << gid << std::endl;
        return 0;
    }
    else {
        std::clog << "[System]Permission denied!";
        return 1;
    }

}


int User::changeName(std::string name) {
    if (!id) return 1;
    prep_stmt = con->prepareStatement("UPDATE users SET name = ? WHERE id = ?");
    prep_stmt->setString(1, name);
    prep_stmt->setInt(2, id);
    prep_stmt->executeUpdate();
    return 0;
}

int User::changePasswd(std::string passwd) {
    if (!id) return 1;
    prep_stmt = con->prepareStatement("UPDATE users SET password = ? WHERE id = ?");
    prep_stmt->setString(1, passwd);
    prep_stmt->setInt(2, id);
    prep_stmt->executeUpdate();
    return 0;
}

int User::getUserID() {
    return id;
}

std::string User::getUserNmae() {
    return name;
}