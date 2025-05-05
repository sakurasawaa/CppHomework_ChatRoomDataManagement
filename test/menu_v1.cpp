#include "include/Print.h"

class Menu {
public:
    virtual void show() = 0;
    virtual void handleChoice(int choice) = 0;
    virtual std::string getTitle() const = 0;
    virtual ~Menu() = default;
protected:
    Menu* preMenu = nullptr;
    std::vector<Menu*> subMenus;
    size_t cnt = 0;
};

class MainMenu : public Menu, public Print {
public:
    void show() override {
        print();
    }
    void handleChoice(int choice) override {
    }
    
    std::string getTitle() const override {
        auto title = getHeaders();
        return title.empty() ? "Unnamed" : title[0];
    }

    template<typename... Args>
    void setSubMenus(Args*... args) {
        (subMenus.push_back(args), ...);
        (addRow(args->getTitle()), ...);
    }

    template<typename... Args>
    MainMenu(Args&&... args) {
        setHeaders(std::forward<Args>(args)...);
        setAlignments({LEFT});
        setMaxWidths({20});
    }
};

class SubMenu : public MainMenu {
    public:
    template<typename... Args>
    SubMenu(Menu& m, Args&&... args) : MainMenu(std::forward<Args>(args)...)
    {
        auto mainMenu = dynamic_cast<MainMenu*>(&m);
        if (mainMenu) {
            mainMenu->setSubMenus(this);
            preMenu = mainMenu; // 同时记录
        }
    }
};

// int main() {
//     MainMenu systemMenu("System");

//     systemMenu.show();
// }