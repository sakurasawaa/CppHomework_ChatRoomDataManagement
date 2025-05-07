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
    print.setHeaders(str[0], str[1], str[2], str[3], str[4]); //这里可以去给Print增加一个函数重载，直接输入字符串
    res = stmt->executeQuery("SELECT * FROM users");
    while (res->next()) {
        print.addRow(res->getInt("id"), res->getString("name"), res->getString("password"), res->getInt("online"), res->getString("create_at"));
    }
    print.print();
    return ;
}
void listGroups() {
    sql::Statement *stmt = nullptr;
    sql::ResultSet *res = nullptr;
    stmt = con->createStatement();
    res = stmt->executeQuery("DESCRIBE `groups`");
    Print print;
    std::vector<std::string> str;
    while (res->next()) {
        str.push_back(res->getString("Field"));
    }
    print.setHeaders(str[0], str[1], str[2], str[3]);
    res = stmt->executeQuery("SELECT * FROM `groups`");
    while (res->next()) {
        print.addRow(res->getInt("id"), res->getString("name"), res->getInt("create_by"), res->getString("create_at"));
    }
    print.print();
    return ;
}
void listMessage() {
    sql::Statement *stmt = nullptr;
    sql::ResultSet *res = nullptr;
    stmt = con->createStatement();
    res = stmt->executeQuery("DESCRIBE message");
    Print print;
    std::vector<std::string> str;
    while (res->next()) {
        str.push_back(res->getString("Field"));
    }
    print.setHeaders(str[0], str[1], str[2], str[3], str[4]);
    res = stmt->executeQuery("SELECT * FROM message");
    while (res->next()) {
        print.addRow(res->getInt("target"), res->getString("string"), res->getInt("source") ,res->getInt("is_group"), res->getString("time"));
    }
    print.print();
    return ;
}
void createUser() {
    int uid;
    std::string username;
    std::cout << "Username: ";
    std::cin >> username;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    User user(username);
    if (!(uid = user.getUserID())) {
        std::string passwd;
        std::cout << "Passwd: ";
        std::cin >> passwd;
        user.signin(passwd);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "User created! UID:" << user.getUserID() << std::endl;
    }
    else std::cout << "User exist! UID:" << user.getUserID() << std::endl;
    return ;
}
void selectUserID() {
    std::string username;
    std::cout << "Username: ";
    std::cin >> username;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    User user(username);
    if (user.getUserID()) std::cout << "UserID: " << user.getUserID() << std::endl;
    else std::cout << "User not exist!" << std::endl;
    return ;
}
void deleteUser() {
    int uid;
    std::cout << "UserID: ";
    std::cin >> uid;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    User user(uid);
    if (user.signout()) std::cout << "User not exist!" << std::endl;
    else std::cout << "User deleted!" << std::endl;
    return ;
}

void changeName() {
    int uid;
    std::string name;
    std::cout << "UserID: ";
    std::cin >> uid;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    User user(uid);
    if (user.getUserID()) {
        std::cout << "NewName: ";
        std::cin >> name;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!user.changeName(name)) std::cout << "Name changed!" << std::endl;
        else std::cout << "Error!" << std::endl;
    }
    else std::cout << "User not exist!" << std::endl;
}

void changePasswd() {
    int uid;
    std::string passwd;
    std::cout << "UserID: ";
    std::cin >> uid;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    User user(uid);
    if (user.getUserID()) {
        std::cout << "NewPasswd: ";
        std::cin >> passwd;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!user.changePasswd(passwd)) std::cout << "Password changed!" << std::endl;
        else std::cout << "Error!" << std::endl;
    }
    else std::cout << "User not exist!" << std::endl;
}

void listUser() {
    int uid;
    std::cout << "UserID: ";
    std::cin >> uid;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    sql::Statement *stmt = nullptr;
    sql::ResultSet *res = nullptr;
    stmt = con->createStatement();
    std::ostringstream oss;
    oss << "SELECT * FROM " << uid << "_userData";
    res = stmt->executeQuery(oss.str());
    Print print;
    print.setHeaders("object", "is_group", "time");
    while (res->next()) {
        print.addRow(res->getInt("object"), res->getInt("is_group"), res->getString("time"));
    }
    print.print();
    return ;
}

void addFriend() {
    int uid;
    int fid;
    std::cout << "UserID: ";
    std::cin >> uid;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    User user(uid);
    if (user.getUserID()) {
        std::cout << "FriendID: ";
        std::cin >> fid;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!user.addFriend(fid)) std::cout << "Friend added!" << std::endl;
        else std::cout << "Error! or friend not exist!" << std::endl;
    }
    else std::cout << "User not exist!" << std::endl;
}
void deleteFriend() {
    int uid;
    int fid;
    std::cout << "UserID: ";
    std::cin >> uid;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    User user(uid);
    if (user.getUserID()) {
        std::cout << "FriendID: ";
        std::cin >> fid;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!user.deleteFriend(fid)) std::cout << "Friend deleted!" << std::endl;
        else std::cout << "Error! or friend not exist!" << std::endl;
    }
    else std::cout << "User not exist!" << std::endl;
}
void addGroup() {
    int uid;
    int gid;
    std::cout << "UserID: ";
    std::cin >> uid;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    User user(uid);
    if (user.getUserID()) {
        std::cout << "GroupID: ";
        std::cin >> gid;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!user.addGroup(gid)) std::cout << "Group added!" << std::endl;
        else std::cout << "Error! or Group not exist!" << std::endl;
    }
    else std::cout << "User not exist!" << std::endl;
}

void createGroup() {
    int uid;
    int gid;
    std::string gname;
    std::cout << "UserID: ";
    std::cin >> uid;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    User user(uid);
    if (user.getUserID()) {
        std::cout << "GroupName: ";
        std::cin >> gname;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if ((gid = user.createGroup(gname))) std::cout << "[System]Group created! GID:" << gid << std::endl;
        else std::cout << "Error!" << std::endl;
    }
    else std::cout << "User not exist!" << std::endl;
}

void deleteGroup() {
    int uid;
    int gid;
    std::cout << "UserID: ";
    std::cin >> uid;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    User user(uid);
    if (user.getUserID()) {
        std::cout << "GroupID: ";
        std::cin >> gid;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!user.deleteGroup(gid)) std::cout << "Group deleted!" << std::endl;
        else std::cout << "Error! or Group not exist!" << std::endl;
    }
    else std::cout << "User not exist!" << std::endl;
}

void removeGroup() {
    int uid;
    int gid;
    std::cout << "UserID: ";
    std::cin >> uid;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    User user(uid);
    if (user.getUserID()) {
        std::cout << "GroupID: ";
        std::cin >> gid;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!user.removeGroup(gid)) std::cout << "Group deleted!" << std::endl;
        else std::cout << "Error! or Group not exist!" << std::endl;
    }
    else {
        std::cout << "User not exist!" << std::endl;
        sql::PreparedStatement *pre_stmt;
        pre_stmt = con->prepareStatement("DELETE FROM `groups` WHERE create_by = ?");
        pre_stmt->setInt(1, uid);
        pre_stmt->executeUpdate();
        std::clog << "[System]Clean Groups::UserID:" << uid << std::endl; 
    }
}