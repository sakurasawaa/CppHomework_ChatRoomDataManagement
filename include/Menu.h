#pragma once
#include "Print.h"
#include <limits>

class BaseMenu {
    public:
        enum STATE {
            EXIT = -2,  //标志位必须小于0
            RETURN
        };
        virtual void show() = 0;
        virtual void run() = 0;
        virtual void handleChoice() = 0;
        virtual void setSubMenu(BaseMenu &) = 0;
        virtual std::vector<BaseMenu *> getSubMenus() = 0;
        virtual void setHeader(std::string) = 0;
        virtual std::string getHeader() = 0;
        virtual void setAlignment(size_t) = 0;
        virtual void setMaxWidth(size_t) = 0;
    protected:
        std::vector<BaseMenu *> subMenus;
        size_t mcnt = 0;
        int choice = 0;
};

class MainMenu : public BaseMenu, protected Print {
    public:
        void show() {
            print();
        }
        void run() {
            while(true) {
                show();
                handleChoice();
                if (choice == RETURN) return; 
                if (choice == EXIT) return; //输入流关闭，所有choice标志位变成EXIT
            }
        }
        void handleChoice() {
skip:
            std::cout << "input> " ;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (std::cin.eof()) {
                choice = EXIT;
                // std::cin.clear();
                return;
            }
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                goto skip;
            }
            choice -= 1;
            if (choice == RETURN) return;
            if (!(choice >= 0 && choice < static_cast<int>(subMenus.size()))) {
                std::cout << "No matching input !" <<std::endl;
                goto skip;
            }
            subMenus.at(choice)->run();
            std::cout << std::endl;
        }
        void setSubMenu(BaseMenu &menu) {
            subMenus.push_back(&menu);
            mcnt += 1;
            std::ostringstream oss;
            oss << '(' << mcnt << ") ";
            addRow(oss.str() + menu.getHeader());
        }
        std::vector<BaseMenu *> getSubMenus() {
            return subMenus;
        }
        void setHeader(std::string s) {
            setHeaders(s);
        }
        std::string getHeader() {
            const std::vector<std::string> headers = getHeaders();
            return headers.empty() ? "": headers[0];
        }
        void setAlignment(size_t alig) {
            setAlignments({(Align)alig});
        }
        void setMaxWidth(size_t size) {
            setMaxWidths({size});
        }
        MainMenu(std::string s) {
                setHeader(s);
                setAlignments({LEFT});
                setMaxWidths({25});
        }
};  

class SubMenu : public MainMenu {
    public:
        SubMenu(BaseMenu &menu, std::string s) : MainMenu(s) {
            menu.setSubMenu(*this);
        }
};


class FuncMenu : private MainMenu {
    public:
        void run() {
            if(func) func();
            std::cout << "Press any key to continue! > ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        };
        FuncMenu(BaseMenu &menu, std::string s, void (*func)()) : MainMenu(s) {
            menu.setSubMenu(*this);
            this->func = func;
        };
    protected:
        void (*func)();
        
};