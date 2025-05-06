#include "Menu.h"
#include "Func.h"

int menu() {
    MainMenu menu("ChatRoomDataManagement");

    SubMenu UsersData(menu, "UsersData");
    SubMenu GroupsData(menu, "GroupsData");
    SubMenu MessageData(menu, "MessageData");

    FuncMenu UsersData_listUsers(UsersData, "listUsers", listUsers);
    FuncMenu UsersData_createUser(UsersData, "createUser", createUser); 
    FuncMenu UsersData_selectUserID(UsersData, "selectUserID", selectUserID); 
    FuncMenu UsersData_deleteUser(UsersData, "deleteUser", deleteUser);
    FuncMenu UsersData_changeName(UsersData, "changeName", changeName);
    FuncMenu UsersData_changePasswd(UsersData, "changePasswd", changePasswd);
    
    SubMenu UsersData_UserData(UsersData, "UserData");
    FuncMenu UsersData_UserData_listUser(UsersData_UserData, "listUser", listUser);
    FuncMenu UsersData_UserData_addFriend(UsersData_UserData, "addFriend", addFriend);
    FuncMenu UsersData_UserData_deleteFriend(UsersData_UserData, "deleteFriend", deleteFriend);
    FuncMenu UsersData_UserData_addGroup(UsersData_UserData, "addGroup", addGroup);
    FuncMenu UsersData_UserData_deleteGroup(UsersData_UserData, "deleteGroup", deleteGroup);
    

    FuncMenu GroupsData_listGroups(GroupsData, "listGroups", listGroups);
    FuncMenu GroupsData_removeGroup(GroupsData, "removeGroup", removeGroup);

    FuncMenu MessageData_listMessage(MessageData, "listMessage", listMessage);
    
    menu.run();
    return 0;
}